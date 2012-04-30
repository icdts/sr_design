#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "debug.h"
#include "input_image.h"

float subpixel_register(input_image *hr_image, input_image *lr_image, int ds, int sigma); 
