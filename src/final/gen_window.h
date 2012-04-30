#ifdef _MSC_VER
#else
	#include <opencv/cv.h>
	#include <opencv2/core/core.hpp>
	#include <opencv2/highgui/highgui.hpp>
#endif
#include <iostream>
#include "debug.h"
#include <vector>

using namespace cv;

Mat gen_window(int, int, float, float, int );
