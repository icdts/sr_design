#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cmath>
#include "gen_window.h"
#include "image_manipulations.h"
#include "debug.h"
#include "fftw3.h"
#include "input_image.h"
#include <sstream>

void register_image(input_image *, input_image *);