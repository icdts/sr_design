#ifdef _MSC_VER
	//FIXME: Windows includes
#else
	#include <opencv/cv.h>
	#include <opencv2/core/core.hpp>
	#include <opencv2/highgui/highgui.hpp>
#endif
#include <iostream>

using namespace std;
using namespace cv;

Mat gen_window(int height, int width, float d1, float d2, int depth, int nChannels);
IplImage* cropImage(const IplImage *img, const CvRect region);
IplImage* resizeImage(const IplImage *origImg, int newWidth, int newHeight, bool keepAspectRatio);

int main(int argc, char* argv[]){
	if( argc != 2) {
		cout <<" Usage: display_image ImageToLoadAndDisplay" << endl;
		return -1;
	}
	
	int rescale_factor = 2;

	IplImage *image;

	image = cvLoadImage(argv[1], CV_LOAD_IMAGE_COLOR); // Read the file

	namedWindow( "Display window", CV_WINDOW_AUTOSIZE );// Create a window for display.

	IplImage newImage = gen_window(image->height, image->width, 0.1, 0.1, image->depth, image->nChannels);

	//IplImage image2 = w;


	cvShowImage( "Display window", &newImage ); // Show our image inside it.


	cvReleaseImage(&image);
	return 0;
}
/*
Results in a height by width matrix where all borders are 0 and the middle
is 1 with height*d1 numbers fading from 1 to 0 at each x edge and width*d2 at
each y edge*/ 
Mat gen_window(int height, int width, float d1, float d2, int depth, int nChannels)
{
	CvSize size;
	size.height = height;
	size.width = width;
	CvScalar s;

	IplImage* retval = cvCreateImage(size, depth, nChannels);

	//Create an height by 1 matrix (column matrix) filled with ones
	size.width = 1;
	IplImage* x = cvCreateImage(size, depth, nChannels);
	cvSet(x, CV_RGB(255, 255, 255));
	int steps = height * d1;
	float increment = 1.0/(steps - 1);
	CvPoint pt;
	/*Create a linear vector (row matrix) with height*d1 evenly spaced numbers
	between 0 and 1; a, b, c, etc.
	results in array a,b,c,1,1,1,1,1,1,1,c,b,a */

	int ct = 0; //This counter is used as the pixel index.
	for(float slope = 0; slope<1.0; slope += increment){
		s = cvGet2D(x, ct, 0); // get the (i,j) pixel value
		s.val[0] = 255.0 * slope;
		s.val[1] = 255.0 * slope;
		s.val[2] = 255.0 * slope;
		printf("B=%f, G=%f, R=%f\n",s.val[0],s.val[1],s.val[2]);
		cvSet2D(x, ct, 0, s); // set the (i,j) pixel value
		
		s = cvGet2D(x, height - 1 - ct, 0); // get the (i,j) pixel value
		s.val[0] = 255.0 * slope;
		s.val[1] = 255.0 * slope;
		s.val[2] = 255.0 * slope;
		printf("B=%f, G=%f, R=%f\n",s.val[0],s.val[1],s.val[2]);
		cvSet2D(x, height - 1 - ct++, 0, s); 
	}
	
	size.width = width;
	size.height = 1;
	IplImage* y = cvCreateImage(size, depth, nChannels);
	cvSet(y, CV_RGB(255, 255, 255));
	steps = width * d2;
	increment = 1.0/(steps - 1);

	ct = 0;
	/*Create a linear vector (row matrix) with width*d1 evenly spaced numbers
	between 0 and 1; a, b, c, etc.
	results in array a,b,c,1,1,1,1,1,1,1,c,b,a */
	for(float slope = 0; slope<1.0; slope += increment){
		s = cvGet2D(y, 0, ct); // get the (i,j) pixel value
		s.val[0] = 255.0 * slope;
		s.val[1] = 255.0 * slope;
		s.val[2] = 255.0 * slope;
		cvSet2D(y, 0, ct, s); // set the (i,j) pixel value

		s = cvGet2D(y, 0, width - 1 - ct); // get the (i,j) pixel value
		s.val[0] = 255.0 * slope;
		s.val[1] = 255.0 * slope;
		s.val[2] = 255.0 * slope;
		cvSet2D(y, 0, width - 1 - ct++, s); 
	}

	Mat Mx;
	Mat tmp = Mat(x, true); //true lets it copy the image contents
	tmp.convertTo(Mx, CV_32F); //default seems to be CV_8U

	Mat My;
	Mat tmp1 = Mat(y, true); //true lets it copy the image contents
	tmp1.convertTo(My, CV_32F); //default seems to be CV_8U

	Mat Mretval;
	Mat tmp2 = Mat(retval, true); //true lets it copy the image contents
	tmp2.convertTo(Mretval, CV_32F); //default seems to be CV_8U

	Mretval = Mx * My;

	cvReleaseImage(&retval);
	cvReleaseImage(&x);
	cvReleaseImage(&y);
	
	return Mretval;
}

// Returns a new image that is a cropped version (rectangular cut-out)
// of the original image.
IplImage* cropImage(const IplImage *img, const CvRect region){
	IplImage* imageCropped;
	CvSize size;

	if (img->width <= 0 || img->height <= 0 || region.width <= 0 || region.height <= 0) {
		cerr << "ERROR in cropImage(): invalid dimensions." << endl;
		exit(1);
	}

	//FIXME: Make work with more depths
	if (img->depth != IPL_DEPTH_8U) {
		cerr << "ERROR in cropImage(): image depth is not 8." << endl;
		exit(1);
	}

	// Set the desired region of interest.
	cvSetImageROI((IplImage*)img, region);

	// Create image the same size of region
	size.width = region.width;
	size.height = region.height;
	imageCropped = cvCreateImage(size, IPL_DEPTH_8U, img->nChannels);

	// Copy region into new image
	cvCopy(img, imageCropped);

	return imageCropped;
}


// Creates a new image copy that is of a desired size. The aspect ratio will
// be kept constant if 'keepAspectRatio' is true, by cropping undesired parts
// so that only pixels of the original image are shown, instead of adding
// extra blank space.
// Remember to free the new image later.
IplImage* resizeImage(const IplImage *origImg, int newWidth, int newHeight, bool keepAspectRatio){
	IplImage *outImg = 0;
	int origWidth;
	int origHeight;
	if (origImg) {
		origWidth = origImg->width;
		origHeight = origImg->height;
	}

	if (newWidth <= 0 || newHeight <= 0 || origImg == 0 || origWidth <= 0 || origHeight <= 0) {
		cerr << "ERROR: Bad desired image size of " << newWidth
			<< "x" << newHeight << " in resizeImage().\n";
		exit(1);
	}

	if (keepAspectRatio) {
		// Resize the image without changing its aspect ratio,
		// by cropping off the edges and enlarging the middle section.
		CvRect r;
		// input aspect ratio
		float origAspect = (origWidth / (float)origHeight);
		// output aspect ratio
		float newAspect = (newWidth / (float)newHeight);
		// crop width to be origHeight * newAspect
		if (origAspect > newAspect) {
			int tw = (origHeight * newWidth) / newHeight;
			r = cvRect((origWidth - tw)/2, 0, tw, origHeight);
		}
		else {	// crop height to be origWidth / newAspect
			int th = (origWidth * newHeight) / newWidth;
			r = cvRect(0, (origHeight - th)/2, origWidth, th);
		}
		IplImage *croppedImg = cropImage(origImg, r);

		// Call this function again, with the new aspect ratio image.
		// Will do a scaled image resize with the correct aspect ratio.
		outImg = resizeImage(croppedImg, newWidth, newHeight, false);
		cvReleaseImage( &croppedImg );

	}
	else {

		// Scale the image to the new dimensions,
		// even if the aspect ratio will be changed.
		outImg = cvCreateImage(cvSize(newWidth, newHeight),
			origImg->depth, origImg->nChannels);
		if (newWidth > origImg->width && newHeight > origImg->height) {
			// Make the image larger
			cvResetImageROI((IplImage*)origImg);
			// CV_INTER_LINEAR: good at enlarging.
			// CV_INTER_CUBIC: good at enlarging.			
			cvResize(origImg, outImg, CV_INTER_LINEAR);
		}
		else {
			// Make the image smaller
			cvResetImageROI((IplImage*)origImg);
			// CV_INTER_AREA: good at shrinking (decimation) only.
			cvResize(origImg, outImg, CV_INTER_AREA);
		}

	}
	return outImg;
}
