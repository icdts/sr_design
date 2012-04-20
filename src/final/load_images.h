#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>

struct input_image {
	std::string name;
	cv::Mat file;
	int vertical_shift;
	int horizontal_shift;
	float prob;
	float * scores[];
};

std::vector<input_image> load_images(std::string dir);