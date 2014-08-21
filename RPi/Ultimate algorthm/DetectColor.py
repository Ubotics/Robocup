import os
import serial
import cv2
import time

COTH = 130

while True:
    os.system('fswebcam /home/pi/1.jpg')
    color = cv2.imread('/home/pi/1.jpg')
    b,g,r =color[160][170]
    if (r >= COTH and g < COTH and b < COTH):
        print 'red'
    if (r < COTH and g >= COTH and b < COTH):
        print 'green'
    if (r < COTH and g < COTH and b >= COTH):
        print 'blue'
    if (r >= COTH and g >= COTH and b >= COTH):
        print 'white'
    if (r < COTH and g < COTH and b < COTH):
        print 'black'
    ser = serial.Serial('/dev/ttyAMA0',115200)
    ser.write('g')
    print color
    del(color)
    time.sleep(1)
