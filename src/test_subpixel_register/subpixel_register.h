#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

float subpixel_register(IplImage* hr_image, IplImage* lr_image, int ds, int sigma); 