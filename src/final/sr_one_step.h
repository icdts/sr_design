#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>
#include "../test_subpixel_register/subpixel_register.h"

cv::Mat sr_one_step(input_image &src, std::vector <input_image> &input);