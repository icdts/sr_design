#ifdef _MSC_VER
#else
	#include <opencv/cv.h>
	#include <opencv2/core/core.hpp>
	#include <opencv2/highgui/highgui.hpp>
#endif
#include <iostream>

cv::Mat shiftImage(cv::Mat, int, int);
cv::Mat shiftImageRot(cv::Mat, int, int);
cv::Mat genShiftDownsampleImage(cv::Mat, int, int, int);
void copyRegion(cv::Mat, cv::Mat, int, int, int, int, int, int, int, int);
