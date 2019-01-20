#
# (c) 2017 Aleph Objects, Inc.
#
# The code in this page is free software: you can
# redistribute it and/or modify it under the terms of the GNU
# General Public License (GNU GPL) as published by the Free Software
# Foundation, either version 3 of the License, or (at your option)
# any later version.  The code is distributed WITHOUT ANY WARRANTY;
# without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.  See the GNU GPL for more details.
#

# Marlin implements an error correcting scheme on the serial connection.
# GCode commands are sent with a line number and a checksum. If Marlin
# detects an error, it requests that the transmission resume from the
# last known good line number.
#
# In addition to this resend mechanism, Marlin also implements flow
# control. Sent commands are acknowleged by "ok" when complete.
# However, to optimize path planning, Marlin contains a command
# buffer and slicing software should send enough commands to keep
# that buffer filled (minus a reserve capacity for emergency commands
# such as an emergency STOP). It is thus necessary to send a certain
# number prior the earlier ones being acknowleged and to track how
# many commands have been sent but not yet acknowleged.
#
# The class MarlinSerialProtocol implements error correction and
# flow control. Occasionally an "ok" from Marlin is garbled during
# serial transmission. This class also implements a watchdog timer
# to recover from such errors.
#
# Note: Marlin does not implement a well defined protocol, so there
# is a lot of complexity here to deal with corner cases.
#
# The prototypical use case for this class is as follows:
#
#   for line in enumerate(gcode):
#     serial.sendCmdLine(line)
#     while(not serial.clearToSend()):
#       serial.readline()
#

import functools
import re
import time

class GCodeHistory:
  """This class implements a history of GCode commands. Right now, we
     keep the entire history, but this probably could be reduced to a
     smaller range if memory use becomes an issue. This class keeps a
     pointer to the position of the first unsent command, which typically
     is the one just recently added, but after a resend request from
     Marlin that position can rewound further back."""
  def __init__(self):
    self.clear()

  def clear(self):
    self.list = [None] # Pad so the first element is at index 1
    self.pos  = 1

  def append(self, cmd):
    self.list.append(cmd)

  def rewindTo(self, position):
    self.pos = max(1,min(position, len(self.list)))

  def getAppendPosition(self):
    """Returns the position at which the next append will happen"""
    return len(self.list)

  def getNextCommand(self):
    """Returns the next unsent command."""
    if(not self.atEnd()):
      res = self.pos, self.list[self.pos]
      self.pos += 1;
      return res

  def atEnd(self):
    return self.pos == len(self.list)

  def position(self):
    return self.pos

  def lastLineSent(self):
    return self.pos - 1

class MarlinSerialProtocol:
  """This class implements the Marlin serial protocol, such
  as adding a checksum to each line, replying to resend
  requests and keeping the Marlin buffer full"""
  def __init__(self, serial, onResendCallback=None, onDebugMsgCallback=None):
    self.serial                 = serial
    self.marlinBufSize          = 5
    self.marlinReserve          = 1
    self.marlinAvailBuffer      = self.marlinBufSize
    self.marlinPendingCommands  = 0
    self.history                = GCodeHistory()
    self.asap                   = []
    self.slowCommands           = re.compile(b"M109|M190|G28|G29")
    self.slowTimeout            = 100
    self.fastTimeout            = 15
    self.usingAdvancedOk        = False
    self.watchdogTimeout        = time.time()
    self.onResendCallback       = onResendCallback
    self.onDebugMsgCallback = onDebugMsgCallback
    self.restart()

  def _stripCommentsAndWhitespace(self, str):
    return str.split(b';', 1)[0].strip()

  def _replaceEmptyLineWithM105(self, str):
    """Marlin will not accept empty lines, so replace blanks with M115 (Get Extruder Temperature)"""
    return b"M105" if str == b"" else str

  def _computeChecksum(self, data):
    """Computes the GCODE checksum, this is the XOR of all characters in the payload, including the position"""
    return functools.reduce(lambda x,y: x^y, map(ord, data) if isinstance(data, str) else list(data))

  def _addPositionAndChecksum(self, position, cmd):
    """An GCODE with line number and checksum consists of N{position}{cmd}*{checksum}"""
    data = b"N%d%s"    % (position, cmd)
    return b"N%d%s*%d" % (position, cmd, self._computeChecksum(data))

  def _sendImmediate(self, cmd):
      self._adjustStallWatchdogTimer(cmd)
      self.serial.write(cmd + b'\n')
      self.serial.flush()
      self.marlinPendingCommands += 1
      self.marlinAvailBuffer     -= 1

  def _sendToMarlin(self):
    """Sends as many commands as are available and to fill the Marlin buffer.
       Commands are first read from the asap queue, then read from the
       history. Generally only the most recently history command is sent;
       but after a resend request, we may be further back in the history
       than that"""
    while(len(self.asap) and self.marlinBufferCapacity() > 0):
      cmd = self.asap.pop(0);
      self._sendImmediate(cmd)
    while(not self.history.atEnd() and self.marlinBufferCapacity() > 0):
      pos, cmd = self.history.getNextCommand();
      self._sendImmediate(cmd)
      if self.marlinBufferCapacity() > 0:
        # Slow down refill of Marlin buffer, as sending multiple commands
        # in a large burst can cause additional serial errors
        time.sleep(0.01)

  def _isResendRequest(self, line):
    """If the line is a resend command from Marlin, returns the line number. This
       code was taken from Cura 1, I do not know why it is so convoluted."""
    if b"resend" in line.lower() or b"rs" in line:
      try:
        return int(line.replace(b"N:",b" ").replace(b"N",b" ").replace(b":",b" ").split()[-1])
      except:
        if b"rs" in line:
          try:
            return int(line.split()[1])
          except:
            return None

  def _isNoLineNumberErr(self, line):
    """If Marlin encounters a checksum without a line number, it will request
       a resend. This often happens at the start of a print, when a command is
       sent prior to Marlin being ready to listen."""
    if line.startswith(b"Error:No Line Number with checksum"):
      m = re.search(b"Last Line: (\d+)", line);
      if(m):
        return int(m.group(1)) + 1

  def _resetMarlinLineCounter(self):
    """Sends a command requesting that Marlin reset its line counter to match
       our own position"""
    cmd = self._addPositionAndChecksum(self.history.lastLineSent(), b"M110")
    self._sendImmediate(cmd)

  def _stallWatchdog(self, line):
    """Watches for a stall in the print. This can happen if a number of
       okays are lost in transmission. To recover, we send Marlin an invalid
       command (no line number, with an asterisk). Once it requests a resend,
       we will back into a known good state (hopefully!)"""
    if self.marlinPendingCommands > 0:
      if time.time() > self.watchdogTimeout:
        self.marlinAvailBuffer     = self.marlinReserve + 1
        self.marlinPendingCommands = 0
        self._sendImmediate(b"\nM105*\n")
        self.sendNotification("Marlin timeout. Forcing re-sync.")
      elif line == b"":
        self.sendNotification("Marlin timeout in %d seconds" % (self.watchdogTimeout - time.time()))

  def _adjustStallWatchdogTimer(self, cmd):
    """Adjusts the stallWatchdogTimer based on the command which is being sent"""
    estimated_duration = self.slowTimeout if self.slowCommands.search(cmd) else self.fastTimeout
    self.watchdogTimeout = max(self.watchdogTimeout, time.time() + estimated_duration)

  def _resendFrom(self, position):
    """If Marlin requests a resend, we need to backtrack."""
    self.history.rewindTo(position)
    self.marlinPendingCommands = 0
    if not self.usingAdvancedOk:
      # When not using ADVANCED_OK, we have no way of knowing
      # for sure how much buffer space is available, but since
      # Marlin requested a resent, assume the buffer was cleared
      self.marlinAvailBuffer = self.marlinBufSize
    else:
      # When using ADVANCED_OK, assume only one slot is free
      # for the next resent command. As soon as that command is
      # acknowleged, we will be informed of how many buffer slots
      # are actually free.
      self.marlinAvailBuffer = self.marlinReserve + 1
    if self.onResendCallback:
      self.onResendCallback(position)

  def _flushReadBuffer(self):
    while self.serial.readline() != b"":
      pass

  def sendCmdReliable(self, line):
    """Adds command line (can contain comments or blanks) to the queue for reliable
       transmission. Queued commands will be processed during calls to readLine() or
       clearToSend(). A checksum will be added for reliable transmission and Marlin
       will be allowed to request a resend of commands that are incorrectly received."""
    if isinstance(line, str):
      line = line.encode()
    line = self._stripCommentsAndWhitespace(line)
    cmd = self._replaceEmptyLineWithM105(line)
    cmd = self._addPositionAndChecksum(self.history.getAppendPosition(), cmd)
    self.history.append(cmd)

  def sendCmdUnreliable(self, line):
    """Sends a command (can contain comments or blanks) prior to any other
       history commands. Commands will be processed during calls to
       readLine() or clearToSend(). These commands are sent without a
       checksum, so Marlin will not request resends if the command is
       corrupted. Unreliable transmission is appropriate for manual
       interactive commands from an UI that is not part of a print.
    """
    if isinstance(line, str):
      line = line.encode()
    cmd = self._stripCommentsAndWhitespace(line)
    if cmd:
      self.asap.append(cmd)

  def sendCmdEmergency(self, line):
      """Sends an command (can contain comments or blanks) without regards for Marlin buffer.
        This assumes there are reserved locations in the Marlin buffer for such commands."""
      if isinstance(line, str):
        line = line.encode()
      cmd = self._stripCommentsAndWhitespace(line)
      if cmd:
        self._sendImmediate(cmd)

  def sendNotification(self, msg):
    if self.onDebugMsgCallback:
      self.onDebugMsgCallback(msg)

  def _gotOkay(self, line):
    m = re.search(b"ok N(\d+) P(\d+) B(\d+)\n", line)
    if m:
      # If ADVANCED_OK is enabled in Marlin, we can use that
      # info to correct our estimate of many free slots are
      # available in the Marlin command buffer.
      lastLineSeen  = int(m.group(1))
      buffAvailable = int(m.group(3))
      self.marlinPendingCommands = max(0, self.history.lastLineSent() - lastLineSeen)
      self.marlinAvailBuffer     = min(self.marlinBufSize, buffAvailable) - self.marlinPendingCommands
      if not self.usingAdvancedOk:
        self.usingAdvancedOk = True
        self.sendNotification("Marlin supports ADVANCED_OK")
    else:
      # Otherwise, assume each "ok" frees up a single spot in
      # the Marlin buffer.
      self.marlinAvailBuffer     += 1
      self.marlinPendingCommands -= 1

  def _readline(self, blocking):
    """Reads input from Marlin"""
    if blocking or self.serial.in_waiting:
      line = self.serial.readline()
    else:
      line = b""

    if line.startswith(b"ok"):
      self._gotOkay(line)

    # Sometimes Marlin replies with an "Error:", but not an "ok".
    # So if we got an error, followed by a timeout, stop waiting
    # for an "ok" as it probably ain't coming
    if line.startswith(b"ok"):
      self.gotError = False
    elif line.startswith(b"Error:"):
      self.gotError = True;
    elif line == b"" and self.gotError:
      self.gotError = False
      self._gotOkay(line)

    return line

  def readline(self, blocking = True):
    """This reads data from Marlin. If no data is available '' will be returned.
       Unlike _readline, this function will take care of resend requests from Marlin.
    """

    self._sendToMarlin()

    line = self._readline(blocking)

    # Watch for and attempt to recover from complete stalls.
    self._stallWatchdog(line)

    # Handle resend requests from Marlin. This happens when Marlin
    # detects a command with a checksum or line number error.
    resendPos = self._isResendRequest(line) or self._isNoLineNumberErr(line)
    if resendPos:
      # If we got a resend requests, purge lines until input buffer is empty
      # or timeout, but watch for any subsequent resend requests (we must
      # only act on the last).
      while line != b"":
        line = self._readline(False)
        resendPos = self._isResendRequest(line) or self._isNoLineNumberErr(line) or resendPos
      # Process the last received resend request:
      if resendPos > self.history.position():
        # If Marlin is asking us to step forward in time, reset its counter.
        self._resetMarlinLineCounter()
      else:
        # Otherwise rewind to where Marlin wants us to resend from.
        self._resendFrom(resendPos)
      # Report a timeout to the calling code.
      line = b""

    return line

  def clearToSend(self):
    """Returns true if there is any space available for new commands, once previously
       queued commands are sent"""
    self._sendToMarlin()
    return self.marlinBufferCapacity() > 0

  def marlinBufferCapacity(self):
    """Returns how many buffer positions are open in Marlin, excluding reserved locations."""
    return self.marlinAvailBuffer - self.marlinReserve

  def restart(self):
    """Clears all buffers and issues a M110 to Marlin. Call this at the start of every print."""
    self.history.clear()
    self.stallCountdown        = self.fastTimeout
    self.gotError              = False
    self.marlinPendingCommands = 0
    self.marlinAvailBuffer     = self.marlinBufSize
    self._flushReadBuffer()
    self._resetMarlinLineCounter()

  def close(self):
    self.serial.close()