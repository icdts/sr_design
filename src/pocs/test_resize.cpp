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

IplImage *cropImage(const IplImage *img, const CvRect region);
IplImage *resizeImage(const IplImage *origImg, int newWidth, int newHeight, bool keepAspectRatio);
IplImage *rotateImage(const IplImage *src, float angleDegrees);
IplImage *shiftImage(const IplImage *src, int up_down, int left_right);
IplImage *shiftImageRot(const IplImage *src, int up_down, int left_right);

int main(int argc, char* argv[]){
	if( argc != 2) {
		cout <<" Usage: display_image ImageToLoadAndDisplay" << endl;
		return -1;
    }

	int rescale_factor = 2;

    IplImage *image;
	IplImage *bigger_image;
	IplImage *rotated_image;
	IplImage *shifted_image;
	IplImage *shifted_image2;
	IplImage *shifted_image3;
	IplImage *shifted_image4;

	//Read in Image
    image = cvLoadImage(argv[1], CV_LOAD_IMAGE_COLOR);

	//Transform image
	bigger_image = resizeImage(image,image->width*rescale_factor,image->height*rescale_factor,false);
	rotated_image = rotateImage(image,180);
	shifted_image = shiftImageRot(image,0,25);
	shifted_image2 = shiftImageRot(image,0,-25);
	shifted_image3 = shiftImageRot(image,25,0);
	shifted_image4 = shiftImageRot(image,-25,0);

	//Show all the images
    namedWindow( "original", CV_WINDOW_AUTOSIZE );
    namedWindow( "bigger", CV_WINDOW_AUTOSIZE );
    namedWindow( "rotated", CV_WINDOW_AUTOSIZE );
    namedWindow( "shifted", CV_WINDOW_AUTOSIZE );
    namedWindow( "shifted2", CV_WINDOW_AUTOSIZE );
    namedWindow( "shifted3", CV_WINDOW_AUTOSIZE );
    namedWindow( "shifted4", CV_WINDOW_AUTOSIZE );
    cvShowImage( "original", image );
    cvShowImage( "bigger", bigger_image );
    cvShowImage( "rotated", rotated_image );
    cvShowImage( "shifted",	shifted_image );
    cvShowImage( "shifted2",shifted_image2 );
    cvShowImage( "shifted3",shifted_image3 );
    cvShowImage( "shifted4",shifted_image4 );
    waitKey(0);

	//Release memory
	cvReleaseImage(&image);
	cvReleaseImage(&bigger_image);
	cvReleaseImage(&rotated_image);
	cvReleaseImage(&shifted_image);
	cvReleaseImage(&shifted_image2);
	cvReleaseImage(&shifted_image3);
	cvReleaseImage(&shifted_image4);
    return 0;	
}

// Returns a new image that is a cropped version (rectangular cut-out)
// of the original image.
IplImage* cropImage(const IplImage *img, const CvRect region){
	IplImage *imageCropped;
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

// Rotate the image clockwise (or counter-clockwise if negative).
// Remember to free the returned image.
IplImage *rotateImage(const IplImage *src, float angleDegrees){
	// Create a map_matrix, where the left 2x2 matrix
	// is the transform and the right 2x1 is the dimensions.
	float m[6];
	CvMat M = cvMat(2, 3, CV_32F, m);
	int w = src->width;
	int h = src->height;
	float angleRadians = angleDegrees * ((float)CV_PI / 180.0f);
	m[0] = (float)( cos(angleRadians) );
	m[1] = (float)( sin(angleRadians) );
	m[3] = -m[1];
	m[4] = m[0];
	m[2] = w*0.5f;  
	m[5] = h*0.5f;  

	// Make a spare image for the result
	CvSize sizeRotated;
	sizeRotated.width = cvRound(w);
	sizeRotated.height = cvRound(h);

	// Rotate
	IplImage *imageRotated = cvCreateImage( sizeRotated,
		src->depth, src->nChannels );

	// Transform the image
	cvGetQuadrangleSubPix( src, imageRotated, &M);

	return imageRotated;
}


IplImage *shiftImage(const IplImage *src, int vertical, int horizontal){
	IplImage *imageShifted;
	IplImage *origImage;
	CvSize size;
	CvSize size2;
	CvRect origRegion;
	CvRect shiftedRegion;
	bool oppositeDirection;

	// Create a new image to copy to
	size.width = cvRound(src->width);
	size.height = cvRound(src->height);
	imageShifted = cvCreateImage(size, src->depth, src->nChannels);
	origImage = cvCreateImage(size, src->depth, src->nChannels);
	cvCopy(src,origImage);

	oppositeDirection = (vertical < 0);
	if(oppositeDirection){
		vertical *= -1;
	}
	for(int i = 1; i <= vertical && i < src->height; i++){
		if(oppositeDirection){
			origRegion = cvRect( 0, i, src->width, src->height);
			shiftedRegion = cvRect( 0, 0, origRegion.width, origRegion.height - i);
		}else{
			origRegion = cvRect( 0, 0, src->width, src->height - i);
			shiftedRegion = cvRect( 0, i, origRegion.width, origRegion.height);
		}

		cvSetImageROI(origImage,origRegion);
		cvSetImageROI(imageShifted,shiftedRegion);

		cvCopy(origImage, imageShifted);
	}

	cvResetImageROI(imageShifted);
	cvResetImageROI(origImage);
	cvCopy(imageShifted,origImage);	

	oppositeDirection = (horizontal < 0);
	if(oppositeDirection){
		horizontal *= -1;
	}
	for(int i = 1; i <= horizontal && i < src->width; i++){
		if(oppositeDirection){
			origRegion = cvRect( i, 0, origRegion.width, origRegion.height);
			shiftedRegion = cvRect( 0, 0, src->width - i, src->height );
		}else{
			origRegion = cvRect( 0, 0, src->width - i, src->height);
			shiftedRegion = cvRect( i, 0, origRegion.width, origRegion.height);
		}
		cvSetImageROI(origImage,origRegion);
		cvSetImageROI(imageShifted,shiftedRegion);

		cvCopy(origImage, imageShifted);
	}

	cvResetImageROI((IplImage*)src);
	cvResetImageROI(imageShifted);
	return imageShifted;
}

IplImage *shiftImageRot(const IplImage *src, int vertical, int horizontal){
	IplImage *imageShifted;
	IplImage *origImage;
	CvSize size;
	CvSize size2;
	CvRect origRegion;
	CvRect shiftedRegion;
	bool oppositeDirection;

	// Create a new image to copy to
	size.width = cvRound(src->width);
	size.height = cvRound(src->height);
	imageShifted = cvCreateImage(size, src->depth, src->nChannels);
	origImage = cvCreateImage(size, src->depth, src->nChannels);
	cvCopy(src,origImage);

	if(vertical != 0 ){
		oppositeDirection = (vertical < 0);
		if(oppositeDirection){
			vertical *= -1;
		}

		//Copy 1st portion
		if(oppositeDirection){
			origRegion = cvRect( 0, vertical, src->width, src->height-vertical );
			shiftedRegion = cvRect( 0, 0, src->width, src->height - vertical);
		}else{
			origRegion = cvRect( 0, 0, src->width, src->height - vertical);
			shiftedRegion = cvRect( 0, vertical, src->width, src->height - vertical );
		}

		cvSetImageROI(origImage,origRegion);
		cvSetImageROI(imageShifted,shiftedRegion);

		cvCopy(origImage, imageShifted);

		//Copy 2nd portion
		if(oppositeDirection){
			origRegion = cvRect( 0, 0, src->width, vertical);
			shiftedRegion = cvRect( 0, src->height - vertical, src->width, vertical );
		}else{
			origRegion = cvRect( 0, src->height - vertical, src->width, vertical );
			shiftedRegion = cvRect( 0, 0, src->width, vertical);
		}

		cvSetImageROI(origImage,origRegion);
		cvSetImageROI(imageShifted,shiftedRegion);

		cvCopy(origImage, imageShifted);

		//Copy changes to origImage, so next steps don't overwrite
		cvResetImageROI(imageShifted);
		cvResetImageROI(origImage);
		cvCopy(imageShifted,origImage);	
	}

	if( horizontal != 0 ){
		oppositeDirection = (horizontal < 0);
		if(oppositeDirection){
			horizontal *= -1;
		}

		//Copy 1st portion
		if(oppositeDirection){
			origRegion = cvRect( horizontal, 0, src->width - horizontal, src->height );
			shiftedRegion = cvRect( 0, 0, src->width - horizontal, src->height);
		}else{
			origRegion = cvRect( 0, 0, src->width - horizontal, src->height);
			shiftedRegion = cvRect( horizontal, 0, src->width - horizontal, src->height );
		}

		cvSetImageROI(origImage,origRegion);
		cvSetImageROI(imageShifted,shiftedRegion);

		cvCopy(origImage, imageShifted);

		//Copy 2nd portion
		if(oppositeDirection){
			origRegion = cvRect( 0, 0, horizontal, src->height);
			shiftedRegion = cvRect( src->width - horizontal, 0, horizontal, src->height );
		}else{
			origRegion = cvRect( src->width - horizontal, 0, horizontal, src->height );
			shiftedRegion = cvRect( 0, 0, horizontal, src->height);
		}

		cvSetImageROI(origImage,origRegion);
		cvSetImageROI(imageShifted,shiftedRegion);

		cvCopy(origImage, imageShifted);
	}

	//Reset region of interest
	cvResetImageROI((IplImage*)src);
	cvResetImageROI(imageShifted);
	return imageShifted;
}
