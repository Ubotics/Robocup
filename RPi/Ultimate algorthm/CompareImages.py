import cv2
import serial
import os


src1 = cv2.cv.LoadImage('/home/pi/1.jpg',0)
hist1= cv2.cv.CreateHist([255], cv2.cv.CV_HIST_ARRAY,ranges=[(0,256)])
cv2.cv.CalcHist([src1],hist1)
cv2.cv.NormalizeHist(hist1, 1.0)

ser = serial.Serial('/dev/ttyAMA0',115200)

while True:
    os.system('raspistill -o 2.jpg -t 50 -n -w 700 -h 700')
    src2 = cv2.cv.LoadImage('/home/pi/2.jpg',0)
    hist2= cv2.cv.CreateHist([255], cv2.cv.CV_HIST_ARRAY,ranges=[(0,256)])
    cv2.cv.CalcHist([src2],hist2)
    cv2.cv.NormalizeHist(hist2, 1.0)
    sc= cv2.cv.CompareHist(hist1, hist2, cv2.cv.CV_COMP_CHISQR)
    print sc
    print('wait')
    if sc<0.08:
        ser.write('H')
        ser.write('g')
        print 'grab'
        break
