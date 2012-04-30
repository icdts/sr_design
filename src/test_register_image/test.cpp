#include "register_image.h"
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
	Mat result;
	Mat cim1;
	Mat cim2;
	Mat cresult;

	int shifts[4][2] = {
			{0,0}};
			/*,
			{-5,0},
			{0,5},
			{0,-5}
	};*/

	for(int i = 0; i < 1; i++){
		im1 = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
		im2 = imread(argv[2], CV_LOAD_IMAGE_GRAYSCALE);
		cim1 = imread(argv[1]);
		cim2 = imread(argv[2]);

		/*	
		im1 = rotate90(im1);
		im2 = rotate90(im2);
		cim1 = rotate90(cim1);
		cim2 = rotate90(cim2);
		*/

		im1.convertTo(im1,CV_32FC1);
		im2.convertTo(im2,CV_32FC1);
		cim1.convertTo(cim1,CV_32FC3);
		cim2.convertTo(cim2,CV_32FC3);

		im2 = shiftMat(im2,shifts[i][0],shifts[i][1]);
		cim2 = shiftMat(cim2,shifts[i][0],shifts[i][1]);

		debug("Calling register_image");

		debug("REGISTER BW");
		result = register_image(im1,im2);
		debug("");
		debug("");
		debug("REGISTER COLOR");
		cresult = register_image(cim1,cim2);
		debug("");
		debug("");

		im1.convertTo(im1,CV_8U);
		im2.convertTo(im2,CV_8U);
		cim1.convertTo(cim1,CV_8U);
		cim2.convertTo(cim2,CV_8U);

		result.convertTo(result,CV_8U);
		cresult.convertTo(cresult,CV_8U);

		debug("Showing output");
		imshow("im1",im1);
		imshow("im2",im2);
		imshow("result",result);

		imshow("cim1",cim1);
		imshow("cim2",cim2);
		imshow("cresult",cresult);

		waitKey(0);
	}
	return 0;
}