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

import functools
import random
import string
import re

class FakeMarlinSerialDevice:
  """This serial class simply pretends to be Marlin by acknowledging
     commands with "ok" and requesting commands to be resent if they
     contain errors"""

  def __init__(self):
    self.line           = 1
    self.replies        = []
    self.pendingOk      = 0
    self.dropCharacters = 0

    self.cumulativeReads     = 0
    self.cumulativeWrites    = 0
    self.cumulativeQueueSize = 0
    self.cumulativeErrors    = 0

  def _enqueue_reply(self, str):
    if str.startswith("ok"):
      self.pendingOk += 1
    self.replies.append(str + '\n')

  def _dequeue_reply(self):
    if len(self.replies):
      reply = self.replies.pop(0)
      if reply.startswith("ok"):
        self.pendingOk -= 1
      return reply
    else:
      return ''

  def _enqueue_okay(self):
    self._enqueue_reply("ok T:10")

  def _computeChecksum(self, data):
    """Computes the GCODE checksum, this is the XOR of all characters in the payload, including the position"""
    return functools.reduce(lambda x,y: x^y, map(ord, data) if isinstance(data, str) else list(data))

  def write(self, data):
    if isinstance(data, str):
      data = data.encode()

    if data.strip() == b"":
      return

    if self.dropCharacters:
      data = data[self.dropCharacters:]
      self.dropCharacters = 0

    hasLineNumber = b"N" in data
    hasChecksum   = b"*" in data

    if not hasLineNumber and not hasChecksum:
      self._enqueue_okay()
      return

    # Handle M110 commands which tell Marlin to reset the line counter
    m = re.match(b'N(\d+)M110\*(\d+)$', data)
    if m:
      self.line = int(m.group(1))

    m = re.match(b'N(\d+)(\D[^*]*)\*(\d+)$', data)
    if m and int(m.group(1)) == self.line and self._computeChecksum(b"N%d%s" % (self.line, m.group(2))) == int(m.group(3)):
      # We have a valid, properly sequenced command with a valid checksum
      self.line += 1
    else:
      # Otherwise, request the command be resent
      self.cumulativeErrors += 1
      self.replies   = []
      self.pendingOk = 0
      self._enqueue_reply("Resend: " + str(self.line))
      # When Marlin issues a resend, it often misses the next few
      # characters. So simulate this here.
      self.dropCharacters = random.randint(0, 5)

    for i in range(0,random.randint(0,4)):
      # Simulate a command that takes a while to execute
      self._enqueue_reply("")
    self._enqueue_okay()

    self.cumulativeWrites     += 1
    self.cumulativeQueueSize  += self.pendingOk

  def readline(self):
    self.cumulativeReads      += 1
    return self._dequeue_reply().encode()

  def flush(self):
    pass

  def close(self):
    print("Average length of commands queue: %.2f" % (float(self.cumulativeQueueSize) / self.cumulativeWrites))
    print("Average reads per write:          %.2f" % (float(self.cumulativeReads)     / self.cumulativeWrites))
    print("Average errors per write:         %.2f" % (float(self.cumulativeErrors)    / self.cumulativeWrites))
    print("Total writes:                     %d"   % self.cumulativeWrites)
    print("Total errors:                     %d"   % self.cumulativeErrors)
    print()
