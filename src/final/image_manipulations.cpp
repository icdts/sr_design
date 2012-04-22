#include "image_manipulations.h"

using namespace std;
using namespace cv;

//TODO: Comment better, prolog
cv::Mat shiftMat(cv::Mat input, int vertical, int horizontal){
	cv::Mat src;
	cv::Mat image_shifted;
	bool opposite_direction;

	input.copyTo(src); //Do deep copy, apparently this doesn't happen when passing
	src.copyTo(image_shifted);

	opposite_direction = (vertical < 0);
	if(opposite_direction){
		vertical *= -1;
	}
	for(int i = 1; i <= vertical && i < src.rows; i++){
		if(opposite_direction){
			copyRegion(
				src,image_shifted,
				0, i, src.cols, src.rows - i, 
				0, 0, src.cols, src.rows - i
			);
		}else{
			copyRegion(
				src,image_shifted,
				0, 0, src.cols, src.rows - i,
				0, i, src.cols, src.rows - i 
			);
		}
	}

	//Save changes in prep for second direction
	image_shifted.copyTo(src);

	opposite_direction = (horizontal < 0);
	if(opposite_direction){
		horizontal *= -1;
	}
	for(int i = 1; i <= horizontal && i < src.cols; i++){
		if(opposite_direction){
			copyRegion(
				src,image_shifted,
				i, 0, src.cols - i, src.rows,
				0, 0, src.cols - i, src.rows
			);
		}else{
			copyRegion(
				src,image_shifted,
				0, 0, src.cols - i, src.rows,
				i, 0, src.cols - i, src.rows
			);
		}
	}

	return image_shifted;
}

//TODO: Comment better, prolog
cv::Mat shiftMatRot(cv::Mat input, int vertical, int horizontal){
	cv::Mat src;
	cv::Mat image_shifted; //(size, src.depth(), src.channels());
	bool opposite_direction;

	input.copyTo(src); //Do deep copy, apparently this doesn't happen when passing
	src.copyTo(image_shifted);

	/*	
	image_shifted = cvCreateImage;
	src = cvCreateImage(size, src.depth, src.nChannels);
	cvCopy(src,src);
	*/

	if(vertical != 0 ){
		opposite_direction = (vertical < 0);
		if(opposite_direction){
			vertical *= -1;
		}

		//Copy 1st portion
		if(opposite_direction){
			copyRegion(
				src,image_shifted,
				0, vertical, src.cols, src.rows-vertical,
				0, 0, src.cols, src.rows - vertical
			);
		}else{
			copyRegion(
				src,image_shifted,
				0, 0, src.cols, src.rows - vertical,
				0, vertical, src.cols, src.rows - vertical 
			);
		}

		//Copy 2nd portion
		if(opposite_direction){
			copyRegion(
				src,image_shifted,
				0, 0, src.cols, vertical,
				0, src.rows - vertical, src.cols, vertical
			);
		}else{
			copyRegion(
				src,image_shifted,
				0, src.rows - vertical, src.cols, vertical,
				0, 0, src.cols, vertical
			);
		}

		image_shifted.copyTo(src);
	}

	if( horizontal != 0 ){
		opposite_direction = (horizontal < 0);
		if(opposite_direction){
			horizontal *= -1;
		}
		
		//Copy 1st portion
		if(opposite_direction){
			copyRegion(
				src,image_shifted,
				horizontal, 0, src.cols - horizontal, src.rows,
				0, 0, src.cols - horizontal, src.rows
			);
		}else{
			copyRegion(
				src,image_shifted,
				0, 0, src.cols - horizontal, src.rows,
				horizontal, 0, src.cols - horizontal, src.rows 
			);
		}

		//Copy 2nd portion
		if(opposite_direction){
			copyRegion(
				src,image_shifted,
				0, 0, horizontal, src.rows,
				src.cols - horizontal, 0, horizontal, src.rows 
			);
		}else{
			copyRegion(
				src,image_shifted,
				src.cols - horizontal, 0, horizontal, src.rows,
				0, 0, horizontal, src.rows
			);
		}
	}

	return image_shifted;
}

void copyRegion(cv::Mat &src, cv::Mat &dest, int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2){
	CvRect src_region = cvRect( x1,y1,w1,h1 );
	CvRect dest_region = cvRect( x2,y2,w2,h2 );

	cv::Mat src_roi = cv::Mat(src,src_region);
	cv::Mat dest_roi = cv::Mat(dest,dest_region);

	src_roi.copyTo(dest_roi);
}

/*
	This shfts the image as directed by up_down and left_right and then shrinks 
	the image by a ratio of 1/downsample_ratio
*/
Mat genShiftDownsampleMat(Mat src, int up_down, int left_right, int downsample_ratio){	

  Mat gsds_image;
  int del_width = src.cols % downsample_ratio;
  int del_height = src.rows % downsample_ratio;

	gsds_image = Mat(src, Range(0, src.cols-del_width), Range(0, src.rows-del_height) );

  gsds_image = shiftMat( gsds_image, up_down, left_right);
  resize(gsds_image, gsds_image, Size(), float(1)/float(downsample_ratio), float(1)/float(downsample_ratio),INTER_LINEAR);

	return gsds_image;
}
