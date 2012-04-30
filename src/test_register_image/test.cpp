#include "../final/register_image.h"
#include "../final/debug.h"

using namespace std;
using namespace cv;

Mat rotate90(Mat input){
	Mat im;

	input.copyTo(im);
	im = im.t();
	flip(im,im,1);
	return im;
}

int main(int argc, char const *argv[]){
	debug("Reading Images");
	Mat im1;
	Mat im2;
	Mat cim1;
	Mat cim2;
	input_image input_im1;
	input_image input_im2;
	input_image input_cim1;
	input_image input_cim2;

	im1 = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	im2 = imread(argv[2], CV_LOAD_IMAGE_GRAYSCALE);
	cim1 = imread(argv[1]);
	cim2 = imread(argv[2]);

	im1.convertTo(im1,CV_32FC1);
	im2.convertTo(im2,CV_32FC1);
	cim1.convertTo(cim1,CV_32FC3);
	cim2.convertTo(cim2,CV_32FC3);

	im1.copyTo(input_im1.file);
	im2.copyTo(input_im2.file);
	cim1.copyTo(input_cim1.file);
	cim2.copyTo(input_cim2.file);

	debug("Calling register_image");

	debug("REGISTER BW");
	register_image(&input_im1,&input_im2);
	debug("");
	debug("");
	debug("REGISTER COLOR");
	register_image(&input_cim1,&input_cim2);
	debug("");
	debug("");

	im1.convertTo(im1,CV_8U);
	im2.convertTo(im2,CV_8U);
	cim1.convertTo(cim1,CV_8U);
	cim2.convertTo(cim2,CV_8U);

	input_im1.file.convertTo(input_im1.file,CV_8U);
	input_im2.file.convertTo(input_im2.file,CV_8U);
	input_cim1.file.convertTo(input_cim1.file,CV_8U);
	input_cim2.file.convertTo(input_cim2.file,CV_8U);

	debug("Showing output");
	imshow("im1",im1);
	imshow("im2",im2);
	imshow("im1",input_im1.file);
	imshow("im2",input_im2.file);

	imshow("cim1",cim1);
	imshow("cim2",cim2);
	imshow("cim1",input_cim1.file);
	imshow("cim2",input_cim2.file);

	waitKey(0);
	
	return 0;
}
