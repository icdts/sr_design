/* For OpenCV data types and functions */
#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

/* For console I/O */
#include <iostream>

/* For creating vectors of various data types */
#include <vector>

/* For various image manipulation functions */
#include "image_manipulations.h"

/* For registering possible subpixels*/
#include "subpixel_register.h"

/* For debug purposes */
#include "debug.h"

cv::Mat sr_one_step(image_data *src, std::vector <image_data> *input);
