#include "gen_window.h"
#include <vector>

using namespace std;
using namespace cv;

Mat gen_window(int height, int width, float hpercent, float wpercent, int channels)
{
	vector<Mat> planes;
	Mat retVal;

	//Create an height by 1 matrix (column matrix) filled with ones
	Mat x = Mat::ones(width, 1, CV_32F);
	//CvMat x = 
	int steps = width * wpercent;
	float slope = 0;
	float increment = 1.0/(steps - 1);

	/*Create a linear vector (row matrix) with width*wpercent evenly spaced numbers
	between 0 and 1; a, b, c, etc.
	results in array a,b,c,1,1,1,1,1,1,1,c,b,a */
	for(int i=0; i<steps; i++){
		x.at<float>(i, 0) = slope;
		x.at<float>(width - 1 - i, 0) = slope;
		slope = slope + increment;
	}

	Mat y = Mat::ones(1, height, CV_32F);
	steps = height * hpercent;
	slope = 0;
	increment = 1.0/(steps - 1);

	/*Create a linear vector (row matrix) with height*hpercent evenly spaced numbers
	between 0 and 1; a, b, c, etc.
	results in array a,b,c,1,1,1,1,1,1,1,c,b,a */
	for(int i=0; i<steps; i++){
		y.at<float>(0, i) = slope;
		y.at<float>(0, height - 1 - i) = slope;
		slope = slope + increment;
	}

	Mat z = Mat(width, height, CV_32F);
	z = x * y;

	for(int i = 0; i < channels; i++)
		planes.push_back(z);

	merge(planes, retVal);

	return retVal;
}