//
//  Robot.cpp
//
//  Created by Wang PG on 14-8-11.
//  Copyright (c) 2014年 Wang PG. All rights reserved.
//

#include "Robot.h"

using namespace std;

//得到img这个单通道图像在(x,y)坐标的像素值。
int getPixel(IplImage* img,int x, int y){
	uchar* tem = (uchar*)(img->imageData + y * img->widthStep);
    return (int)tem[x];
}

//设置img这个单通道图像在(x,y)坐标的像素值。
void setPixel(IplImage* img,int x, int y,int value){
	uchar* tem = (uchar*)(img->imageData + y * img->widthStep);
    tem[x] = value;
    return;
}

//将src这个图像按照scale的比例缩放，新生成图像到dst。
//例如：IplImage* des = reSize(img, 0.2);
void reSize(IplImage* src, IplImage* dst, double scale){
    CvSize dst_size;//定义二维化矩形框

    dst_size.width = (int)(src->width * scale);
    dst_size.height = (int)(src->height * scale);//确定新图的矩形框
    dst=cvCreateImage(dst_size,src->depth,src->nChannels);//创建图像头
    cvResize(src,dst,CV_INTER_LINEAR);//使用双线性差值减小图像。
    
    return;
}

//将img这个图像按照k为阈值进行二值化处理，返回一个二值化图像到It。
void threshold(IplImage* dst, IplImage* It, int k){
    IplImage *Igray = cvCreateImage(cvGetSize(dst),IPL_DEPTH_8U,1);
    cvCvtColor(dst,Igray,CV_BGR2GRAY);
    
    IplImage *It = 0;
    It = cvCreateImage(cvSize(Igray->width,Igray->height),
                       IPL_DEPTH_8U, 1);
    cvThreshold(Igray,It,k,255,CV_THRESH_BINARY);

    cvReleaseImage(&Igray);
    return;
}

//得到It图像的PID误差值。
int getError(IplImage* It){
	//int left,right;
    int left_mean = 0, right_mean = 0;
    for (int y = 0; y<30; y++) {
        for (int x = 0; x<It->width; x++) {
            //printf("(%d,%d) : %d\n",x,y,getPixel(It,x,y));
            if (
                getPixel(It, x-3, y)==255 &&
                getPixel(It, x-2, y)==255 &&
                getPixel(It, x-1, y)==255 &&
                getPixel(It, x, y) == 0
                )
                left_mean += x;
            else if (
                     getPixel(It, x-3, y)==0 &&
                     getPixel(It, x-2, y)==0 &&
                     getPixel(It, x-1, y)==0 &&
                     getPixel(It, x, y) == 255
                     ) {
                right_mean += x;
            }
        }
    }
    left_mean /= 30;
    right_mean /= 30;
    
    //printf("(left,right)=(%d, %d)\n",left_mean,right_mean);
    //printf("The picture's width is: %d\n",It->width);
    int error =(left_mean + right_mean)/2 - It->width/2;
    
    if (error <= 100 ) {
        //printf("The error is: %d\n",error);
        return error;
    }
    else{
        //printf("There is something wrong!\nThe error is: 0");
        error = 0;
        return error;
    }
}

//比较两个图像的相似度。
int CompareHist(const char* imagefile1, const char* imagefile2){
	IplImage *image1=cvLoadImage(imagefile1, 0);  
    IplImage *image2=cvLoadImage(imagefile2, 0);  
  
    CvHistogram *Histogram1 = cvCreateHist(1, &HistogramBins, CV_HIST_ARRAY,HistogramRange);  
    CvHistogram *Histogram2 = cvCreateHist(1, &HistogramBins, CV_HIST_ARRAY,HistogramRange);  
  
    cvCalcHist(&image1, Histogram1);  
    cvCalcHist(&image2, Histogram2);  
  
    cvNormalizeHist(Histogram1, 1);  
    cvNormalizeHist(Histogram2, 1);  
  
    // CV_COMP_CHISQR,CV_COMP_BHATTACHARYYA这两种都可以用来做直方图的比较，值越小，说明图形越相似  
    printf("CV_COMP_CHISQR : %.4f\n", cvCompareHist(Histogram1, Histogram2, CV_COMP_CHISQR));  
    //printf("CV_COMP_BHATTACHARYYA : %.4f\n", cvCompareHist(Histogram1, Histogram2, CV_COMP_BHATTACHARYYA));  

    // CV_COMP_CORREL, CV_COMP_INTERSECT这两种直方图的比较，值越大，说明图形越相似  
    //printf("CV_COMP_CORREL : %.4f\n", cvCompareHist(Histogram1, Histogram2, CV_COMP_CORREL));  
    //printf("CV_COMP_INTERSECT : %.4f\n", cvCompareHist(Histogram1, Histogram2, CV_COMP_INTERSECT));  
  
    cvReleaseImage(&image1);  
    cvReleaseImage(&image2);  
    cvReleaseHist(&Histogram1);  
    cvReleaseHist(&Histogram2);  
    return 0;  
}
