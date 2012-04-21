#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace std;

void copyRegion(cv::Mat &src, cv::Mat &dest, int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2){
	CvRect src_region = cvRect( x1,y1,w1,h1 );
	CvRect dest_region = cvRect( x2,y2,w2,h2 );

	cv::Mat src_roi = cv::Mat(src,src_region);
	cv::Mat dest_roi = cv::Mat(dest,dest_region);

	src_roi.copyTo(dest_roi);
}//TODO: Comment better, prolog

cv::Mat shiftMat(cv::Mat src, int vertical, int horizontal){
	cv::Mat image_shifted;
	bool opposite_direction;

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

int main(int argc, char const *argv[]){
	cv::Mat m = cv::imread(argv[1]);

	m = shiftMat(m,25,0); 

	cv::imshow("show",m);
	cv::waitKey(0);
	return 0;
}