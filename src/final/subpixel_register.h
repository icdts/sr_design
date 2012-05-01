/* For OpenCV data types and functions */
#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

/* For console I/O */
#include <iostream>

/* For various image manipulation functions */
#include "image_manipulations.h"

/* For debugging purposes */
#include "debug.h"

/* This class uses input_images */
#include "input_image.h" 


float subpixel_register(input_image *hr_image, input_image *lr_image, int ds, int sigma); 
