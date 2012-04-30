/*
 * phase_correlation: 
 * load 2 images and calculate phase correlation
 *
 * Author:
 * Nashruddin Amin (http://www.nasrudin.com)
 */

#include <stdio.h>
#include "cv.h"
#include "highgui.h"
#include "fftw3.h"

CvPoint phase_correlation( IplImage *ref, IplImage *tpl );

int main( int argc, char** argv )
{
	IplImage *tpl = 0;
	IplImage *ref = 0;
	
	/* load reference image */
	ref = cvLoadImage( argv[1], CV_LOAD_IMAGE_GRAYSCALE );
	
	/* load template image */
	tpl = cvLoadImage( argv[2], CV_LOAD_IMAGE_GRAYSCALE );
	
	/* get phase correlation of input images */
	CvPoint maxloc = phase_correlation( ref, tpl );
	
	/* print it */
	fprintf( stdout, "Maxval at (%d, %d) \n", maxloc.x, maxloc.y );
	
	cvReleaseImage( &tpl );
	cvReleaseImage( &ref );
	
	return 0;
}

/*
 * get phase correlation from two images and save result to the third image
 */
CvPoint phase_correlation( IplImage *ref, IplImage *tpl ){
	IplImage *poc;

	int 	i, j, k;
	double	tmp;
	
	/* get image properties */
	int width  	 = ref->width;
	int height   = ref->height;
	int step     = ref->widthStep;
	int fft_size = width * height;

	/* create a new image, to store phase correlation result */
	poc = cvCreateImage( cvSize( tpl->width, tpl->height ), IPL_DEPTH_64F, 1 );

	/* setup pointers to images */
	uchar 	*ref_data = ( uchar* ) ref->imageData;
	uchar 	*tpl_data = ( uchar* ) tpl->imageData;
	double 	*poc_data = ( double* )poc->imageData;
	
	/* allocate FFTW input and output arrays */
	fftw_complex *img1 = ( fftw_complex* )fftw_malloc( sizeof( fftw_complex ) * width * height );
	fftw_complex *img2 = ( fftw_complex* )fftw_malloc( sizeof( fftw_complex ) * width * height );
	fftw_complex *res  = ( fftw_complex* )fftw_malloc( sizeof( fftw_complex ) * width * height );	
	
	/* setup FFTW plans */
	fftw_plan fft_img1 = fftw_plan_dft_1d( width * height, img1, img1, FFTW_FORWARD,  FFTW_ESTIMATE );
	fftw_plan fft_img2 = fftw_plan_dft_1d( width * height, img2, img2, FFTW_FORWARD,  FFTW_ESTIMATE );
	fftw_plan ifft_res = fftw_plan_dft_1d( width * height, res,  res,  FFTW_BACKWARD, FFTW_ESTIMATE );
	
	/* load images' data to FFTW input */
	for( i = 0, k = 0 ; i < height ; i++ ) {
		for( j = 0 ; j < width ; j++, k++ ) {
			img1[k][0] = ( double )ref_data[i * step + j];
			img1[k][1] = 0.0;
			
			img2[k][0] = ( double )tpl_data[i * step + j];
			img2[k][1] = 0.0;
		}
	}
	
	/* obtain the FFT of img1 */
	fftw_execute( fft_img1 );
	
	/* obtain the FFT of img2 */
	fftw_execute( fft_img2 );
	
	/* obtain the cross power spectrum */
	for( i = 0; i < fft_size ; i++ ) {
		res[i][0] = ( img2[i][0] * img1[i][0] ) - ( img2[i][1] * ( -img1[i][1] ) );
		res[i][1] = ( img2[i][0] * ( -img1[i][1] ) ) + ( img2[i][1] * img1[i][0] );

		tmp = sqrt( pow( res[i][0], 2.0 ) + pow( res[i][1], 2.0 ) );

		res[i][0] /= tmp;
		res[i][1] /= tmp;
	}

	/* obtain the phase correlation array */
	fftw_execute(ifft_res);

	/* normalize and copy to result image */
	for( i = 0 ; i < fft_size ; i++ ) {
        poc_data[i] = res[i][0] / ( double )fft_size;
	}

		/* find the maximum value and its location */
    CvPoint minloc, maxloc;
	double  minval, maxval;
	cvMinMaxLoc( poc, &minval, &maxval, &minloc, &maxloc, 0 );

	/* deallocate FFTW arrays and plans */
	fftw_destroy_plan( fft_img1 );
	fftw_destroy_plan( fft_img2 );
	fftw_destroy_plan( ifft_res );
	fftw_free( img1 );
	fftw_free( img2 );
	fftw_free( res );	

	return maxloc;
}
