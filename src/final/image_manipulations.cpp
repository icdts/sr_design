#include "image_manipulations.h"

using namespace std;
using namespace cv;

// Returns a new image that is a cropped version (rectangular cut-out)
// of the original image.
IplImage* cropImage(IplImage *img, CvRect region){
	IplImage *image_cropped;
	CvSize size;

	if (img->width <= 0 || img->height <= 0 || region.width <= 0 || region.height <= 0) {
		cerr << "ERROR in cropImage(): invalid dimensions." << endl;
		exit(1);
	}

	// Set the desired region of interest.
	cvSetImageROI((IplImage*)img, region);

	// Create image the same size of region
	size.width = region.width;
	size.height = region.height;
	image_cropped = cvCreateImage(size, img->depth, img->nChannels);

	// Copy region into new image
	cvCopy(img, image_cropped);

	return image_cropped;
}


// Creates a new image copy that is of a desired size. The aspect ratio will
// be kept constant if 'keepAspectRatio' is true, by cropping undesired parts
// so that only pixels of the original image are shown, instead of adding
// extra blank space.
// Remember to free the new image later.
IplImage* resizeImage(IplImage *orig_img, int new_width, int new_height, bool keep_aspect_ratio){
	IplImage *out_img;
	int orig_width;
	int orig_height;

	if (orig_img) {
		orig_width = orig_img->width;
		orig_height = orig_img->height;
	}

	if (new_width <= 0 || new_height <= 0 || orig_img == 0 || orig_width <= 0 || orig_height <= 0) {
		cerr << "ERROR: Bad desired image size of " << new_width
			<< "x" << new_height << " in resizeImage().\n";
		exit(1);
	}

	if (keep_aspect_ratio) {
		CvRect r;
		float orig_aspect;
		float new_aspect;
		IplImage *cropped_img;

		// Resize the image without changing its aspect ratio,
		// by cropping off the edges and enlarging the middle section.
		// input aspect ratio
		orig_aspect = (orig_width / (float)orig_height);
		// output aspect ratio
		new_aspect = (new_width / (float)new_height);

		// crop width to be orig_height * new_aspect
		if (orig_aspect > new_aspect) {
			int tw;
			
			tw = (orig_height * new_width) / new_height;
			r = cvRect((orig_width - tw)/2, 0, tw, orig_height);
		}
		else {	// crop height to be orig_width / new_aspect
			int th;

			th = (orig_width * new_height) / new_width;
			r = cvRect(0, (orig_height - th)/2, orig_width, th);
		}
		cropped_img = cropImage(orig_img, r);

		// Call this function again, with the new aspect ratio image.
		// Will do a scaled image resize with the correct aspect ratio.
		out_img = resizeImage(cropped_img, new_width, new_height, false);
		cvReleaseImage( &cropped_img );

	} else {

		// Scale the image to the new dimensions,
		// even if the aspect ratio will be changed.
		out_img = cvCreateImage(cvSize(new_width, new_height), orig_img->depth, orig_img->nChannels);
		if (new_width > orig_img->width && new_height > orig_img->height) {
			// Make the image larger
			cvResetImageROI((IplImage*)orig_img);
			// CV_INTER_LINEAR: good at enlarging.
			// CV_INTER_CUBIC: good at enlarging.			
			cvResize(orig_img, out_img, CV_INTER_LINEAR);
		}
		else {
			// Make the image smaller
			cvResetImageROI((IplImage*)orig_img);
			// CV_INTER_AREA: good at shrinking (decimation) only.
			cvResize(orig_img, out_img, CV_INTER_AREA);
		}

	}
	return out_img;
}

// Rotate the image clockwise (or counter-clockwise if negative).
// Remember to free the returned image.
IplImage *rotateImage(IplImage *src, float angle_degrees){
	float m[6];
	CvSize size_rotated;
	CvMat M;
	int w;
	int h;
	float angle_radians;
	IplImage *image_rotated;

	// Create a map_matrix, where the left 2x2 matrix
	// is the transform and the right 2x1 is the dimensions.
	M = cvMat(2, 3, CV_32F, m);
	w = src->width;
	h= src->height;

	angle_radians = angle_degrees * ((float)CV_PI / 180.0f);
	m[0] = (float)( cos(angle_radians) );
	m[1] = (float)( sin(angle_radians) );
	m[3] = -m[1];
	m[4] = m[0];
	m[2] = w*0.5f;  
	m[5] = h*0.5f;  

	// Make a spare image for the result
	size_rotated.width = cvRound(w);
	size_rotated.height = cvRound(h);

	// Rotate
	image_rotated = cvCreateImage( size_rotated, src->depth, src->nChannels );

	// Transform the image
	cvGetQuadrangleSubPix( src, image_rotated, &M);

	return image_rotated;
}


//TODO: Comment better, prolog
IplImage *shiftImage(IplImage *src, int vertical, int horizontal){
	IplImage *image_shifted;
	IplImage *after_vertical;
	CvSize size;
	bool opposite_direction;

	size.width = cvRound(src->width);
	size.height = cvRound(src->height);
	image_shifted = cvCreateImage(size, src->depth, src->nChannels);

	opposite_direction = (vertical < 0);
	if(opposite_direction){
		vertical *= -1;
	}
	for(int i = 1; i <= vertical && i < src->height; i++){
		if(opposite_direction){
			copyRegion(
				src,image_shifted,
				0, i, src->width, src->height - i, 
				0, 0, src->width, src->height - i
			);
		}else{
			copyRegion(
				src,image_shifted,
				0, 0, src->width, src->height - i,
				0, i, src->width, src->height - i 
			);
		}
	}

	//Save changes in prep for second direction
	after_vertical = cvCreateImage(size, src->depth, src->nChannels);
	cvResetImageROI(image_shifted);
	cvCopy(image_shifted,after_vertical);	

	opposite_direction = (horizontal < 0);
	if(opposite_direction){
		horizontal *= -1;
	}
	for(int i = 1; i <= horizontal && i < src->width; i++){
		if(opposite_direction){
			copyRegion(
				after_vertical,image_shifted,
				i, 0, src->width - i, src->height,
				0, 0, src->width - i, src->height
			);
		}else{
			copyRegion(
				after_vertical,image_shifted,
				0, 0, src->width - i, src->height,
				i, 0, src->width - i, src->height
			);
		}
	}

	cvResetImageROI((IplImage*)src);
	cvResetImageROI(image_shifted);
	return image_shifted;
}

//TODO: Comment better, prolog
IplImage *shiftImageRot(IplImage *src, int vertical, int horizontal){
	IplImage *image_shifted;
	IplImage *orig_image;
	CvSize size;
	bool opposite_direction;

	// Create a new image to copy to
	size.width = cvRound(src->width);
	size.height = cvRound(src->height);
	image_shifted = cvCreateImage(size, src->depth, src->nChannels);
	orig_image = cvCreateImage(size, src->depth, src->nChannels);
	cvCopy(src,orig_image);

	if(vertical != 0 ){
		opposite_direction = (vertical < 0);
		if(opposite_direction){
			vertical *= -1;
		}

		//Copy 1st portion
		if(opposite_direction){
			copyRegion(
				src,image_shifted,
				0, vertical, src->width, src->height-vertical,
				0, 0, src->width, src->height - vertical
			);
		}else{
			copyRegion(
				src,image_shifted,
				0, 0, src->width, src->height - vertical,
				0, vertical, src->width, src->height - vertical 
			);
		}

		//Copy 2nd portion
		if(opposite_direction){
			copyRegion(
				src,image_shifted,
				0, 0, src->width, vertical,
				0, src->height - vertical, src->width, vertical
			);
		}else{
			copyRegion(
				src,image_shifted,
				0, src->height - vertical, src->width, vertical,
				0, 0, src->width, vertical
			);
		}

		//Copy changes to orig_image, so next steps don't overwrite
		cvResetImageROI(image_shifted);
		cvResetImageROI(orig_image);
		cvCopy(image_shifted,orig_image);	
	}

	if( horizontal != 0 ){
		opposite_direction = (horizontal < 0);
		if(opposite_direction){
			horizontal *= -1;
		}
		
		//Copy 1st portion
		if(opposite_direction){
			copyRegion(
				orig_image,image_shifted,
				horizontal, 0, src->width - horizontal, src->height,
				0, 0, src->width - horizontal, src->height
			);
		}else{
			copyRegion(
				orig_image,image_shifted,
				0, 0, src->width - horizontal, src->height,
				horizontal, 0, src->width - horizontal, src->height 
			);
		}

		//Copy 2nd portion
		if(opposite_direction){
			copyRegion(
				orig_image,image_shifted,
				0, 0, horizontal, src->height,
				src->width - horizontal, 0, horizontal, src->height 
			);
		}else{
			copyRegion(
				orig_image,image_shifted,
				src->width - horizontal, 0, horizontal, src->height,
				0, 0, horizontal, src->height
			);
		}
	}

	//Reset region of interest
	cvResetImageROI((IplImage*)src);
	cvResetImageROI(image_shifted);
	return image_shifted;
}

void copyRegion(IplImage *src, IplImage *dest, int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2){
	CvRect src_region = cvRect( x1,y1,w1,h1 );
	CvRect dest_region = cvRect( x2,y2,w2,h2 );

	cvSetImageROI((IplImage*)src,src_region);
	cvSetImageROI(dest,dest_region);

	cvCopy(src,dest);
}

/*
	This shfts the image as directed by up_down and left_right and then shrinks 
	the image by a ratio of 1/downsample_ratio
*/
IplImage *genShiftDownsampleImage(const IplImage *src, int up_down, int left_right, int downsample_ratio){	
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
	size2.width = cvRound( size.width - (size.width % downsample_ratio) );
	size2.height = cvRound( size.height - (size.height % downsample_ratio) );
	  
	//If either dimension isn't divisble by the downsample value, redefine the region 
	if( size.width != size2.width || size.height != size2.height ){
		cvSetImageROI( origImage, cvRect( 0, 0, size2.width, size2.height ) );
	}

	// Creates image with size equal to either the size of the original image or the region of
	// interest specified above.
	gsdsImage = cvCreateImage( cvGetSize( origImage ), origImage->depth, origImage->nChannels );
	cvCopy( origImage, gsdsImage );
	cvReleaseImage( &origImage );
	gsdsImage = shiftImage( gsdsImage, up_down, left_right );
	gsdsImage = resizeImage( gsdsImage, gsdsImage->width/downsample_ratio, gsdsImage->height/downsample_ratio, false);

	return gsdsImage;
}
