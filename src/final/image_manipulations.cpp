#include "image_manipulations.h"

using namespace std;
using namespace cv;


/******************************************************************************

	shiftMat accepts a CV_32F Mat and shifts it according to the integer 
	parameters that are also passed to it.  It fills in the gaps left by the
	shift with the last line of pixels in the shifted image.  It then returns 
	the image as a Mat.


	shiftMat:
		Mat input:
			The matrix to be shifted.
		int vertical:
			An integer value that determines how far the image needs to be
			shifted vertically.  Negative shifts upward, positive shifts 
			downward.
		int horizontal:
			An integer value that determines how far the image needs to be
			shifted horizontally.  Negative shifts left, positive shifts right.

******************************************************************************/
Mat shiftMat(	Mat input, 
				int vertical, 
				int horizontally ){
	Mat src;
	Mat image_shifted;
	bool opposite_direction;

	input.copyTo(src);  // Do deep copy, apparently this doesn't happen when
						// passing
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

/******************************************************************************

	shiftMatRot accepts a CV_32F Mat and shifts it according to the integer 
	parameters that are also passed to it.  It fills in the gaps left by the
	shift with the pixels that were shifted off of the other side of the image.
	It then returns the image as a Mat.


	shiftMat:
		Mat input:
			The matrix to be shifted.
		int vertical:
			An integer value that determines how far the image needs to be
			shifted vertically.  Negative shifts upward, positive shifts 
			downward.
		int horizontal:
			An integer value that determines how far the image needs to be
			shifted horizontally.  Negative shifts left, positive shifts right.

******************************************************************************/
Mat shiftMatRot(	Mat input, 
					int vertical, 
					int horizontal ){
	Mat src;
	Mat image_shifted; //(size, src.depth(), src.channels());
	bool opposite_direction;

	input.copyTo(src); 	// Do deep copy, apparently this doesn't happen when
						// passing
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

/******************************************************************************

	copyRegion accepts a pointer to a source matrix and a destination matrix as
	parameters along with integer values that specify a region in each image.  
	The function copys the specified region in the src Mat into the specified
	region of the dest Mat.


	copyRegion:
		Mat &src:
			Reference to a source matrix.
		Mat &dest:
			Reference to a destination matrix.
		int x1:
			The x coordinate of the left side of the selected region from src.
		int y1:
			The y coordinate of the top side of the selected region from src.
		int w1:
			The width of the region selected from src.
		int h1:
			The height of the region selected from src.
		int x2:
			The x coordinate of the left side of the selected region from dest.
		int y2:
			The y coordinate of the top side of the selected region from dest.
		int w2:
			The width of the region selected from dest.
		int h2:
			The height of the region selected from dest.

******************************************************************************/
void copyRegion(	Mat &src, 
					Mat &dest, 
					int x1, 
					int y1, 
					int w1, 
					int h1, 
					int x2, 
					int y2, 
					int w2, 
					int h2 ){
	CvRect src_region = cvRect( x1,y1,w1,h1 );
	CvRect dest_region = cvRect( x2,y2,w2,h2 );

	Mat src_roi = Mat(src,src_region);
	Mat dest_roi = Mat(dest,dest_region);

	src_roi.copyTo(dest_roi);
}

/******************************************************************************

	genShiftDownsampleMat shfts the src matrix as directed by up_down and 
	left_right and then shrinks the image by a ratio of 1/downsample_ratio. It
	then returns the resulting Mat.

	genShiftDownsampleMat:
		Mat src:
			This is the source Mat to be shifted and downsampled.
		int up_down:	
			An integer value that determines how far the image needs to be
			shifted vertically.  Negative shifts upward, positive shifts 
			downward.
		int left_right:
			An integer value that determines how far the image needs to be
			shifted horizontally.  Negative shifts left, positive shifts right.
		int downsample_ratio:
			The inverse of the desired scale of the image.

******************************************************************************/
Mat genShiftDownsampleMat(	Mat src, 
							int up_down, 
							int left_right, 
							int downsample_ratio ){	

	Mat gsds_image;
	int del_width = src.cols % downsample_ratio;
	int del_height = src.rows % downsample_ratio;

	gsds_image = Mat(src, 
					 Range(0, src.cols-del_width), 
					 Range(0, src.rows-del_height) );

	gsds_image = shiftMat( gsds_image, up_down, left_right);
	
	resize(	gsds_image, 
			gsds_image, 
			Size(), 
			float(1)/float(downsample_ratio), 
			float(1)/float(downsample_ratio),INTER_LINEAR );

	return gsds_image;
}
