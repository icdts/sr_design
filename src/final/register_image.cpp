#include "register_image.h"

using namespace std;
using namespace cv;

/******************************************************************************

    phase_correlation accepts two Mat variables as parameters.  This program
    takes two images of equal size and uses the FFTW library to assess the two
    images' similarity to one another.

    phase_correlation:
        Mat m_ref:
            Reference image.  This is the first image passed to the function.
        Mat m_tpl:
            Template image.  This is the second image passed to the function, to
            which the first image is compared.

    Full disclosure:
        We obtained this code from nashruddin.com after many failed attempts to
        write this with OpenCV's built-in and DFT function.

        http://nashruddin.com/phase-correlation-function-in-opencv.html

******************************************************************************/
Point phase_correlation( Mat m_ref, Mat m_tpl ){
    debug("Phase correlation called");
    IplImage * poc;
    
    /*  */
    IplImage ref = m_ref;
    IplImage tpl = m_tpl;

    int     i, j, k;
    double  tmp;
    
    /* get image properties */
    int width    = ref.width;
    int height   = ref.height;
    int step     = ref.widthStep;
    int fft_size = width * height;

    /* create a new image, to store phase correlation result */
    poc = cvCreateImage( cvSize( tpl.width, tpl.height ), IPL_DEPTH_64F, 1 );

    /* setup pointers to images */
    uchar   *ref_data = ( uchar* ) ref.imageData;
    uchar   *tpl_data = ( uchar* ) tpl.imageData;
    double  *poc_data = ( double* )poc->imageData;
    
    /* allocate FFTW input and output arrays */
    fftw_complex *img1 = ( fftw_complex* )fftw_malloc( 
            sizeof( fftw_complex ) * width * height );
    
    fftw_complex *img2 = ( fftw_complex* )fftw_malloc( 
            sizeof( fftw_complex ) * width * height );
    
    fftw_complex *res  = ( fftw_complex* )fftw_malloc( 
            sizeof( fftw_complex ) * width * height );   
    
    /* setup FFTW plans */
    fftw_plan fft_img1 = fftw_plan_dft_1d( width * height, 
                                           img1, 
                                           img1, 
                                           FFTW_FORWARD,  
                                           FFTW_ESTIMATE );

    fftw_plan fft_img2 = fftw_plan_dft_1d( width * height, 
                                           img2, 
                                           img2, 
                                           FFTW_FORWARD,  
                                           FFTW_ESTIMATE );
    
    fftw_plan ifft_res = fftw_plan_dft_1d( width * height, 
                                           res,  
                                           res,  
                                           FFTW_BACKWARD, 
                                           FFTW_ESTIMATE );
    
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
        res[i][0] = ( img2[i][0] * img1[i][0] ) - 
                    ( img2[i][1] * ( -img1[i][1] ) );
        
        res[i][1] = ( img2[i][0] * ( -img1[i][1] ) ) + 
                    ( img2[i][1] * img1[i][0] );

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


    /* deallocate IplImage variables */
    cvReleaseImage(&poc);

    return maxloc;
}

/******************************************************************************
    
    register_image accepts two image_data variables as parameters.  It then
    uses calls to phase_correlation to determine how the second image should be
    shifted in order for the two images to "line up".  It then performs the
    recommended shift.

    register_image:
        image_data * input1:
            The base image to which input2 is compared and shifted.
        image_data * input2:
            The image that is shifted according to its correlation with the
            first image.

******************************************************************************/
void register_image(image_data * input1, image_data * input2){
    debug("Register image called");
    debug(input1->name);
    debug(input2->name);
    Mat im1;
    Mat im2;
    Point shift;
    image_data ret;
    stringstream ss;
    string d_out;

    //Deep copies to keep from changing original
    debug("Deep Copying");
    input1->file.copyTo(im1);
    input2->file.copyTo(im2);

    debug("Converting");
    im1.convertTo(im1,CV_8U);
    im2.convertTo(im2,CV_8U);

    debug("Phase correlation");
    shift = phase_correlation(im1,im2);

#ifdef DEBUG
    ss << shift.x;
    ss >> d_out;
    debug("SHIFT IS:");
    debug("\t" + d_out);
    ss << shift.y;
    ss >> d_out;
    debug("\t" + d_out);
#endif
    
    shiftMat(input2->file,shift.x,shift.y);
    input2->horizontal_shift = shift.x;
    input2->vertical_shift = shift.y;

    input2->file.convertTo(im1,CV_8U);
    //imwrite("registered" + input2->name + ".jpg",im1);
}
