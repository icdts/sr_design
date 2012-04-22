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

void debug(string str){
#ifdef DEBUG
    cerr << "DEBUG: " << str << endl;
#endif
}

Mat fftshift(Mat& input){
    Mat output = input.clone();
    int half_x = input.rows / 2;
    int half_y = input.cols / 2;
    int new_i, new_j;
    for(int i=0; i<input.rows; i++){
        for(int j=0; j<input.cols; j++){
            new_i = (i + half_x) % input.rows;
            new_j = (j + half_y) % input.cols;
            output.at<uchar>(new_i,new_j) = input.at<uchar>(i,j);
        }
    }
    return output;
}

Mat register_image(Mat input1, Mat input2){
    Mat im1;
    Mat im2;
    Mat window;
    Mat flipped_im2;
    Mat tmp1;
    Mat tmp2;
    Point max;

    Scalar scalar;
    int x;
    int y;

    int shid;

    debug("Deep Copying");
    //Deep copies to keep from changing original
    input1.copyTo(im1);
    input2.copyTo(im2);

    debug("Mitigating Boundary Effect");
    //Mitigate boundary effect
    window = gen_window(im1.rows,im1.cols,0.05,0.05,im1.channels());
    im1 = window.mul(im1);
    im2 = window.mul(im2);

    debug("Fake Normalizing");
    //Normalize result
    scalar = mean(im1);
    im1.copyTo(tmp1);
    im2.copyTo(tmp2);
    tmp1.setTo(mean(im1));
    tmp2.setTo(mean(im2));
    divide(im1,tmp1,im1,1);
    divide(im2,tmp2,im2,1);

    debug("Flipping im2");
    //Flip left-right and up-down
    flip(im2,flipped_im2,-1);

    debug("FFT calls");
    //First fft2
    dft(im1,tmp1,DFT_COMPLEX_OUTPUT);

    //Second fft2
    dft(flipped_im2,tmp2,DFT_COMPLEX_OUTPUT);

    //Multiply on element-by-element basis
    tmp1 = tmp1.mul(tmp2);

    debug("Reverse FFT");
    //Reverse fft
    idft(tmp1,tmp2,DFT_COMPLEX_OUTPUT);

    debug("Abs of result");
    //Shift zero frequencies to the middle
    tmp2 = abs(tmp2);
    debug("FFTSHIFT");
    tmp2 = fftshift(tmp2);

    debug("Finding max loc");
    //Find maximum element
    minMaxLoc(tmp2,NULL,NULL,NULL,&max);

    debug("Crazy Maths");
    //replicating Matlab, no good explination
    shid = (max.x * tmp2.rows) + max.y;

    //%make a point based on the middle of the image
    //sh(1)=(mod(shid-1,size(im1,1))+1)-size(im1,1)/2;
    //sh(2)=ceil((shid-1)/size(im1,1))-size(im1,2)/2;
    x = (((shid-1)%im1.rows) + 1) - (im1.rows/2);
    y = ceil((shid-1)/im1.rows) - (im1.rows/2);

    debug("Returning shifted img");
    return shiftMat(input2,x,y);   
}
