import cv2
import os
import serial

THRESHOLD_NUM = 150;

img = cv2.imread( '/mnt/share/road.JPG' )
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY) 
print gray[0]

It = cv2.threshold( gray, THRESHOLD_NUM, 255, cv2.THRESH_BINARY )
for x in xrange(1):
	for y in xrange (0,len(It[1][0])):
		print "(%s,%s): %s" %(x,y,It[1][0][y])

left_mean  = 0
right_mean = 0

for x in xrange(30):# Get the mean of the first 30 lines to reduce error.
	for y in xrange( 3, len(It[1][1]) ):
		if ((It[1][x][y-4]==255) and (It[1][x][y-3]==255) and (It[1][x][y-2]==255) and (It[1][x][y-1]==255) and (It[1][x][y]==0)):
			# Ignore the noise point
			left_mean += y
			#print "left %s" %y
		elif ((It[1][x][y]==255) and (It[1][x][y-1]==0) and (It[1][x][y-2]==0) and (It[1][x][y-3]==0) and (It[1][x][y-4]==0)):
			# Ignore the noise point
			right_mean += y
			#print "right %s \n" %y
			break

left_mean  /= 30;
right_mean /= 30;

# Set the center row of the img as the SETPOINT
error =(left_mean + right_mean)/2 - len(It[1][1])/2

ser = serial.Serial('/dev/ttyAMA0',115200)
print "The error is %s" %error
ser.write(error)
#ser.write(str(error))