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

/* This code uses image_datas */
#include "image_data.h" 


float subpixel_register(image_data *hr_image, image_data *lr_image, int ds, int sigma); 
