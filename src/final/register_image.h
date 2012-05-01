/* For OpenCV datatypes and functions */
#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

/* For standard mathematical functions */
#include <cmath>

/* Generates padded Mat to mitigate boundary effect */
#include "gen_window.h"

/* For various image manipulation functions */
#include "image_manipulations.h"

/* For debugging purposes */
#include "debug.h"

/* The fastest Fourier transform in the west
   We use this for FFTs as opposed to OpenCVs DFT function */
#include "fftw3.h"

/* This code uses input_images */
#include "input_image.h"

#include <sstream>

void register_image(input_image *, input_image *);