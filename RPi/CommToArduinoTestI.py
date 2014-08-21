# Built by Peiguang Wang
# reference from CubicSolver.py, Makeblock LCC
# reference site: git.oschina.com/

import serial

# initial serial address with /dev/ttyAMA0
address = '/dev/ttyAMA0'

# The command that you want to send to Arduino
command = 'Hello, Arduino!'

# Create new object to serial port on Linux 
ser = serial.Serial(address, 115200) 

# write this command to Arduino
ser.write(command)