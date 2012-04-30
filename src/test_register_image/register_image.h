#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cmath>
#include "../final/gen_window.h"
#include "../final/image_manipulations.h"
#include "../final/debug.h"
#include "fftw3.h"

cv::Mat register_image(cv::Mat, cv::Mat);