/* For OpenCV datatypes and functions */
#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

/* For creating vectors of various data types */
#include <vector>

/* For string variables and functions */
#include <string>

/* For Console I/O */
#include <iostream>

/* This class uses input_images */
#include "input_image.h"

/* For debugging purposes */
#include "debug.h"

void load_images(std::string dir,std::vector<input_image> *);