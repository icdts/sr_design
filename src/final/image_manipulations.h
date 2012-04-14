#ifdef _MSC_VER
#else
	#include <opencv/cv.h>
	#include <opencv2/core/core.hpp>
	#include <opencv2/highgui/highgui.hpp>
#endif
#include <iostream>

IplImage *cropImage(IplImage *, CvRect);
IplImage *resizeImage(IplImage *, int, int, bool);
IplImage *rotateImage(IplImage *, float);
IplImage *shiftImage(IplImage *, int, int);
IplImage *shiftImageRot(IplImage *, int, int);
IplImage *genShiftDownsampleImage(IplImage *, int, int, int);
void copyRegion(IplImage *, IplImage *, int, int, int, int, int, int, int, int);
