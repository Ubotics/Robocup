//
//  Robot.h
//
//  Created by Wang PG on 14-8-11.
//  Copyright (c) 2014年 Wang PG. All rights reserved.
//

#include <iostream>
#include "/usr/local/include/opencv/cv.h"
#include "/usr/local/include/opencv/highgui.h"
#include "/usr/local/include/opencv/cxcore.h"

int getPixel(IplImage* img,int x, int y);
//得到img这个单通道图像在(x,y)坐标的像素值。

void setPixel(IplImage* img,int x, int y,int value);
//设置img这个单通道图像在(x,y)坐标的像素值。

IplImage* reSize(IplImage* img, double scale);
//将img这个图像按照scale的比例缩放，新生成一个图像作为返回值。
//例如：IplImage* des = reSize(img, 0.2);

IplImage* threshold(IplImage* img, int k);
//将img这个图像按照k为阈值进行二值化处理，返回一个二值化图像。

int getError(IplImage* img);
//得到img图像的PID误差值。

int CompareHist(const char* imagefile1, const char* imagefile2);
//比较两个图像的相似度。
