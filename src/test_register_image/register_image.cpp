#ifdef +MSC_VER
	//XXX: Need something here?
#else
	#include <opencv/cv.h>
	#include <opencv2/core/core.hpp>
	#include <opencv2/highgui/highgui.hpp>
#enfid
#include <iostream>

using namespace std;

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
cv::Mat register_image(cv::Mat input1, cv::Mat input2){
    cv::Mat im1;
    cv::Mat im2;
    cv::Mat window;
    cv::Mat flipped_im2;
    cv::Mat tmp1;
    cv::Mat tmp2;
    CvPoint max;

    int x;
    int y;

    int shid;

    //Deep copies to keep from changing original
    input1.copyTo(im1);
    input2.copyTo(im2);

    //Mitigate boundary effect
    window = gen_window(im1.rows,im1.cols,0.05,0.05);
    im1 = window.multiply(im1);
    im2 = window.multiply(im2);

    //Normalize result
    im1 = im1.divide(cv::mean(im1));
    im2 = im2.divide(cv::mean(im2));

    //Flip left-right and up-down
    cv::flip(im2,flipped_im2,-1);

    //First fft2
    dft(im1,tmp1,DFT_COMPLEX_OUTPUT);

    //Second fft2
    dft(flipped_im2,tmp2,DFT_COMPLEX_OUTPUT);

    //Multiply on element-by-element basis
    tmp1 = tmp1.multiply(tmp2);

    //Reverse fft
    idft(tmp1,tmp2,DFT_COMPLEX_OUTPUT);

    //Shift zero frequencies to the middle
    tmp2 = fftshift(abs(tmp2));

    //Find maximum element
    minMaxLoc(tmp2,NULL,NULL,NULL,&max);

    //replicating Matlab, no good explination
    shid = (max.x * tmp2.rows) + max.y;

    //%make a point based on the middle of the image
    //sh(1)=(mod(shid-1,size(im1,1))+1)-size(im1,1)/2;
    //sh(2)=ceil((shid-1)/size(im1,1))-size(im1,2)/2;
    x = (((shid-1)%im1.rows) + 1) - (im1.rows/2);
    y = Math.ceil((shid-1)/im1.rows) - (im1.rows/2);

    return shiftImage(input2,x,y);   
}
