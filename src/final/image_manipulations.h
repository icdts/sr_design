
	#include <opencv/cv.h>
	#include <opencv2/core/core.hpp>
	#include <opencv2/highgui/highgui.hpp>
#include <iostream>

cv::Mat shiftMat(cv::Mat, int, int);
cv::Mat shiftMatRot(cv::Mat, int, int);
cv::Mat genShiftDownsampleMat(cv::Mat, int, int, int);
void copyRegion(cv::Mat&, cv::Mat&, int, int, int, int, int, int, int, int);
