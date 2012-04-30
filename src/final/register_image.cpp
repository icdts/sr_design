#include "register_image.h"

/*
function [im2_reg,sh,f]=register_image(im1,im2)
oim2=im2;
w=gen_window(size(im1),0.05,0.05);
im1=w.*im1; % mitigate boundary effect
im2=w.*im2; % by using w as a kernel
%dot-star is entry-by-entry multiplication

%matrix right division (mrdivide)
%must have same number of columns

%mean(im(:)) returns a single value for mean of all elements
im1=im1/mean(im1(:)); %pretend normalizing
im2=im2/mean(im2(:));

%2D FFT on image 1 entry-by-entry multipled with FFT on image 2 rotated 180
%degrees
tmp=fft2(im1).*fft2(flipud(fliplr(im2)));
f=fftshift(abs(ifft2(tmp))); %fftshift moves fft so zero frequency 
                             %component is in middle, ifft2 inverse fft
%find max value in the new image (value: dummy, index: shid)
[dummy,shid]=max(f(:));

%make a point based on the middle of the image
sh(1)=(mod(shid-1,size(im1,1))+1)-size(im1,1)/2;
sh(2)=ceil((shid-1)/size(im1,1))-size(im1,2)/2;
%shift the image based on the (new) midpoint?
%this is still somewhat unclear
im2_reg=shift_image(oim2,sh);
*/

using namespace std;
using namespace cv;

Point phase_correlation( Mat m_ref, Mat m_tpl ){
    debug("Phase correlation called");
    IplImage * poc;
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

    cvReleaseImage(&poc);

    return maxloc;
}

void register_image(input_image * input1, input_image * input2){
    debug("Register image called");
    debug(input1->name);
    debug(input2->name);
    Mat im1;
    Mat im2;
    Point shift;
    input_image ret;
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
    imwrite("registered" + input2->name + ".jpg",im1);
}
