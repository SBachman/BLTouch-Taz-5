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

from __future__ import print_function

import sys

class LoggingSerialConnection:
  """Wrapper class which prints the input and output from a serial connection"""
  def __init__(self, serial, filename):
    self.serial     = serial
    self.verbose    = True
    self.file       = open(filename, 'w+')

  def _log(self, *args, **kwargs):
    if(self.verbose):
      print(*args, file=self.file, **kwargs)
      self.file.flush()

  def write(self, data):
    self._log("> " + data.decode(), end='')
    self.serial.write(data)

  def flush(self):
    self.serial.flush()

  def close(self):
    self.serial.close()

  def readline(self):
    data = self.serial.readline()
    if(data == b""):
      self._log("< Timeout")
    else:
      self._log("< " + data.decode(), end='')
    return data

  @property
  def in_waiting(self):
    return self.serial.in_waiting

  @property
  def out_waiting(self):
    return self.serial.out_waiting