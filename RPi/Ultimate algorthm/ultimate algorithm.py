import cv2
import serial
import os

LEFT            = 'l'
RIGHT           = 'r'
FORWORD         = 'f'
BACKWORD        = 'b'
CLOCKWIZE       = 'c'
UNCLOCKWIZE     = 'C'
HALT_SLOWLY     = 'h'
HALT_SUDDENLY   = 'H'
GRAB_THE_FIRST  = 'g'
GRAB_THE_SECOND = 'G'
TURN_AROUND     = 't'

ROAD_CENTER     = '/home/wpg/Desktop/database/1.JPG'
ROAD_TUBE       = '/home/wpg/Desktop/database/2.JPG'
ROAD_TARGET     = '/home/wpg/Desktop/database/3.JPG'

PIC_ROAD        = '/home/wpg/Desktop/pic/road.JPG'
PIC_TUBE        = '/home/wpg/Desktop/pic/tube.JPG'

Near            = 0.2;

ARDUINO_ADDRESS = '/dev/ttyAMA0' 

def compute_histogram(src, bins = 255 ):
    hist = cv.CreateHist([255], cv.CV_HIST_ARRAY, ranges=[(0, 256)])
    cv.CalcHist([src], hist)      #compute histogram
    cv.NormalizeHist(hist, 1.0)   #normalize hist
    return hist

def IsNearTarget():
    src1 = cv.LoadImage(PIC_ROAD, 0)
    src2 = cv.LoadImage(ROAD_TARGET, 0)
    hist1 = compute_histogram(src1)
    hist2 = compute_histogram(src2)
    sc = cv.CompareHist(hist1, hist2, cv.CV_COMP_CHISQR)
    if sc <= Near:
    	return True
    else:
    	return False

def IsNearCenter():
    src1 = cv.LoadImage(PIC_ROAD, 0)
    src2 = cv.LoadImage(ROAD_CENTER, 0)
    hist1 = compute_histogram(src1)
    hist2 = compute_histogram(src2)
    sc = cv.CompareHist(hist1, hist2, cv.CV_COMP_CHISQR)
    if sc <= Near:
    	return True
    else:
    	return False

def IsNearCenter():
    src1 = cv.LoadImage(PIC_ROAD, 0)
    src2 = cv.LoadImage(ROAD_TUBE, 0)
    hist1 = compute_histogram(src1)
    hist2 = compute_histogram(src2)
    sc = cv.CompareHist(hist1, hist2, cv.CV_COMP_CHISQR)
    if sc <= Near:
    	return True
    else:
    	return False

def GetColor():
	img = cv2.imread(PIC_TUBE)
	length = len(img)
	width = img.size / length
    a,b,c = img[length/2][width/2]
    '''
    #
    #Need debug!!
    #
    '''
    return [a,b,c]

def Grab( mode ):# mode = 1 or 2
	ser = serial.Serial(ARDUINO_ADDRESS,115200)
	if ( mode == 1 ):
		ser.write(GRAB_THE_FIRST)
	if ( mode == 2 ):
		ser.write(GRAB_THE_SECOND)

def TurnLeft( angle ):
	ser = serial.Serial(ARDUINO_ADDRESS,115200)
    ser.write(LEFT)
    ser.write(int(angle))

def TurnRight( angle ):
	ser = serial.Serial(ARDUINO_ADDRESS,115200)
    ser.write(RIGHT)
    ser.write(int(angle))

def TurnClockwise():# Turn 90 degree to right
	ser = serial.Serial(ARDUINO_ADDRESS,115200)
    ser.write(CLOCKWIZE)

def TurnCountClockwise():# Turn 90 degree to left
	ser = serial.Serial(ARDUINO_ADDRESS,115200)
    ser.write(UNCLOCKWIZE)


def StopSlowly():
	ser = serial.Serial(ARDUINO_ADDRESS,115200)
    ser.write(HALT_SLOWLY)

def StopSuddenly():
	ser = serial.Serial(ARDUINO_ADDRESS,115200)
    ser.write(HALT_SUDDENLY)

def GoForward( distance ):
	ser = serial.Serial(ARDUINO_ADDRESS,115200)
    ser.write(GoForward)
    ser.write(int(distance))

def TurnAround():
	ser = serial.Serial(ARDUINO_ADDRESS,115200)
    ser.write(TURN_AROUND)
