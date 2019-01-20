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

import random
import string

class NoisySerialConnection:
  """Wrapper class which injects character noise into data of
     a serial connection for testing Marlin's error correction"""
  def __init__(self, serial):
    self.serial         = serial
    self.readErrorRate  = 0
    self.writeErrorRate = 0

  def _corruptData(self, data):
    """Introduces a single character error on a string"""
    badChar = random.choice(string.ascii_letters)
    badChar = badChar if isinstance(data, str) else badChar.encode()
    if len(data) == 0:
      return data
    if len(data) == 1:
      return badChar
    charToCorrupt = random.randint(0, len(data) - 1)
    return data[:charToCorrupt] + badChar + data[charToCorrupt+1:]

  def write(self, data):
    if(random.random() < self.writeErrorRate):
      data = self._corruptData(data)
    self.serial.write(data)

  def readline(self):
    data = self.serial.readline()
    if(random.random() < self.readErrorRate):
      data = self._corruptData(data)
    return data

  @property
  def in_waiting(self):
    return self.serial.in_waiting

  @property
  def out_waiting(self):
    return self.serial.out_waiting

  def flush(self):
    self.serial.flush()

  def close(self):
    self.serial.close()

  def setWriteErrorRate(self, badWrites, totalWrites):
    """Inserts a single character error into every badWrites out of totalWrites"""
    self.writeErrorRate = float(badWrites)/float(totalWrites)

  def setReadErrorRate(self, badReads, totalReads):
    """Inserts a single character error into every badReads out of totalReads"""
    self.readErrorRate = float(badReads)/float(totalReads)