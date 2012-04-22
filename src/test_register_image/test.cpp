#include "register_image.h"
#include "../final/debug.h"

using namespace std;
using namespace cv;

int main(int argc, char const *argv[]){
	debug("Reading Images");
	Mat im1 = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
	Mat im2 = imread(argv[2],CV_LOAD_IMAGE_GRAYSCALE);

	im1.convertTo(im1,CV_32FC1);
	im2.convertTo(im2,CV_32FC1);

	im2 = shiftMat(im2,15,0);
	imshow("im2",im2);
	waitKey(0);

	debug("Calling register_image");
	Mat result = register_image(im1,im2);

	im1.convertTo(im1,CV_8U);
	im2.convertTo(im2,CV_8U);
	result.convertTo(result,CV_8U);

	debug("Showing output");
	imshow("im1",im1);
	imshow("im2",im2);
	imshow("result",result);

	waitKey(0);

	return 0;
}
