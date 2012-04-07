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

//These two functions are dependencies borrowed from resize_test.cpp
IplImage *shiftImage(const IplImage *src, int up_down, int left_right);
IplImage* resizeImage(const IplImage *origImg, int newWidth, int newHeight, bool keepAspectRatio);
IplImage *cropImage(const IplImage *img, const CvRect region);

//This is the matlab function that this file is attempting to recreate.
IplImage *genShiftDownsampleImage(const IplImage *src, int up_down, int left_right, int ds);


int main(int argc, char* argv[]){
	if( argc != 2) {
		cout <<" Usage: display_image ImageToLoadAndDisplay" << endl;
		return -1;
    }

	int rescale_factor = 2;

    IplImage *image;
	IplImage *gsds_image;

	//Read in Image
    image = cvLoadImage(argv[1], CV_LOAD_IMAGE_COLOR);

	//Transform image
	gsds_image = genShiftDownsampleImage( image, 30, 30, 3 );

	//Show all the images
    namedWindow( "gsds_image", CV_WINDOW_AUTOSIZE );
    cvShowImage( "gsds_image",	gsds_image );
    waitKey(0);

	//Release memory
	cvReleaseImage(&image);
	cvReleaseImage(&gsds_image);
    return 0;	

	return 0;
}

IplImage *genShiftDownsampleImage(const IplImage *src, int up_down, int left_right, int ds){	
	IplImage *origImage;
	IplImage *gsdsImage;
	CvSize size;
	CvSize size2;

	size.width = cvRound( src->width );
	size.height = cvRound( src->height );
	origImage = cvCreateImage( size, src->depth, src->nChannels );
	cvCopy( src, origImage );

	// The image is cropped to dimensions that are divisible by the downsample value
	// so that they can be more cleanly resized.
	size2.width = cvRound( size.width - (size.width % ds) );
	size2.height = cvRound( size.height - (size.height % ds) );
	  
	//If either dimension isn't divisble by the downsample value, redefine the region 
	if( size.width != size2.width || size.height != size2.height ){
		cvSetImageROI( origImage, cvRect( 0, 0, size2.width, size2.height ) );
	}

	// Creates image with size equal to either the size of the original image or the region of
	// interest specified above.
	gsdsImage = cvCreateImage( cvGetSize( origImage ), origImage->depth, origImage->nChannels );
	cvCopy( origImage, gsdsImage );
	cvReleaseImage( &origImage );
//% shift the image the way I'm told
//	% throw out information in those two directions
	gsdsImage = shiftImage( gsdsImage, up_down, left_right );
	gsdsImage = resizeImage( gsdsImage, gsdsImage->width/ds, gsdsImage->height/ds, false );
//% create output of some fraction of the size of image
//imouts=floor(size(image)/ds);
//imout=zeros(imouts);

//% Add each region into imout
//for id=1:ds
//    for jd=1:ds
//        imout=imout+image(id:ds:size(image,1),jd:ds:size(image,2));
//    end
//end

//% And divide to get the average of each region
//imout=imout/ds/ds;
	return gsdsImage;
}

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
	for(int i = 0; i < vertical && i < src->height; i++){
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
	for(int i = 0; i < horizontal && i < src->width; i++){
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