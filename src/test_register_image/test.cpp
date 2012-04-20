#include "register_image.h"

int main(int argc, char const *argv[]){
	cv::Mat im = cv::imread(argv[1]);
	cv::Mat im2 = cv::imread(argv[2]);

	cv::Mat result = register_image(im,im2);

	cv::imshow("im",im);
	cv::imshow("im2",im2);
	cv::imshow("result",result);

	cv::waitKey(0);

	return 0;
}
