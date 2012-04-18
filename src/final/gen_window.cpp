#include "gen_window.h"

Mat gen_window(int s1, int s2, float d1, float d2)
{
	//Create an s1 by 1 matrix (column matrix) filled with ones
	Mat x = Mat::ones(s2, 1, CV_32F);
	//CvMat x = 
	int steps = s2 * d2;
	float slope = 0;
	float increment = 1.0/(steps - 1);

	/*Create a linear vector (row matrix) with s2*d2 evenly spaced numbers
	between 0 and 1; a, b, c, etc.
	results in array a,b,c,1,1,1,1,1,1,1,c,b,a */
	for(int i=0; i<steps; i++){
		x.at<float>(i, 0) = slope;
		x.at<float>(s2 - 1 - i, 0) = slope;
		slope = slope + increment;
	}

	Mat y = Mat::ones(1, s1, CV_32F);
	steps = s1 * d1;
	slope = 0;
	increment = 1.0/(steps - 1);

	/*Create a linear vector (row matrix) with s1*d1 evenly spaced numbers
	between 0 and 1; a, b, c, etc.
	results in array a,b,c,1,1,1,1,1,1,1,c,b,a */
	for(int i=0; i<steps; i++){
		y.at<float>(0, i) = slope;
		y.at<float>(0, s1 - 1 - i) = slope;
		slope = slope + increment;
	}

	Mat z = Mat(s2, s1, CV_32F);
	z = x * y;
	return z;
}