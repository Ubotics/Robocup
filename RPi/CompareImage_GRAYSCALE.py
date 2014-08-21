#
# Compare Image.py
# Env and Tools: Opencv 2.4.8 and python 2.7
# 
# created by Wang PG on 2014-8-15
# 

import cv2.cv as cv
import time

def compute_histogram(src, bins = 255 ):
    hist = cv.CreateHist([255], cv.CV_HIST_ARRAY, ranges=[(0, 256)])
    cv.CalcHist([src], hist)      #compute histogram
    cv.NormalizeHist(hist, 1.0)   #normalize hist
    return hist

def test():
    src1 = cv.LoadImage("/home/wpg/Desktop/database/target.JPG", 0)
    src2 = cv.LoadImage("/home/wpg/Desktop/database/IMG_2534.JPG", 0)
    hist1 = compute_histogram(src1)
    hist2 = compute_histogram(src2)
    sc = cv.CompareHist(hist1, hist2, cv.CV_COMP_CHISQR)
    print sc

if __name__=="__main__":
    #start = time.time()
    test()
    #end = time.time()
    #duration = end - start
    #print "Time: %s" %duration

