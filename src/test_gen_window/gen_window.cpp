#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

Mat* gen_window(int s1, int s2, float d1, float d2);
IplImage* cropImage(const IplImage *img, const CvRect region);
IplImage* resizeImage(const IplImage *origImg, int newWidth, int newHeight, bool keepAspectRatio);

int main(int argc, char* argv[]){
/*if( argc != 2) {
     cout <<" Usage: display_image ImageToLoadAndDisplay" << endl;
     return -1;
    }
	*/
	
int rescale_factor = 2;

    IplImage *image;


    image = cvLoadImage("cars.JPG", CV_LOAD_IMAGE_COLOR); // Read the file
	Mat w = Mat::Mat(23, 23, CV_32F);
    namedWindow( "Display window", CV_WINDOW_AUTOSIZE );// Create a window for display.

w = *gen_window(image->height, image->width, 0.1, 0.1);

    /*cvShowImage( "Display window", w ); // Show our image inside it.
    waitKey(0); // Wait for a keystroke in the window

cvReleaseImage(&image);
cvReleaseImage(&w);*/
    return 0;
}

/* Results in a s1 by s2 matrix where all borders are 0 and the middle
is 1 with s1*d1 numbers fading from 1 to 0 at each x edge and s2*d2 at
each y edge */
Mat* gen_window(int s1, int s2, float d1, float d2)
{
	Mat *w;
	//Create an s1 by 1 matrix (column matrix) filled with ones
	Mat x = Mat::ones(s1, 1, CV_32F);
	//CvMat x = 
	int steps = s1 * d1;
	float slope = 0;
	float increment = 1/(steps - 1);

	/*Create a linear vector (row matrix) with s1*d1 evenly spaced numbers
	between 0 and 1; a, b, c, etc.
	results in array a,b,c,1,1,1,1,1,1,1,c,b,a */
	for(int i=0; i<steps; i++){
		x.at<float>(i, 0) = slope;
		x.at<float>(s1 - 1 - i, 0) = slope;
		slope = slope + increment;
	}
	
	Mat y = Mat::ones(1, s2, CV_32F);
	steps = s2 * d2;
	slope = 0;
	increment = 1/(steps - 1);

	/*Create a linear vector (row matrix) with s2*d1 evenly spaced numbers
	between 0 and 1; a, b, c, etc.
	results in array a,b,c,1,1,1,1,1,1,1,c,b,a */
	for(int i=0; i<steps; i++){
		y.at<float>(0, i) = slope;
		y.at<float>(0, s2 - 1 - i) = slope;
		slope = slope + increment;
	}

	Mat z = Mat::Mat(s1, s2, CV_32F);
	z = x * y;
	w = &z;
	return w;
}
