#ifdef _MSC_VER
#else
	#include <opencv/cv.h>
	#include <opencv2/core/core.hpp>
	#include <opencv2/highgui/highgui.hpp>
#endif
#include <iostream>
#include <string>
#include <vector>

//#define DEBUG
 
void debug(std::string);
void printMatrix(cv::Mat);