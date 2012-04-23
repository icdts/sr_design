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

Mat fftshift(Mat& input){
    Mat output;
    int half_x;
    int half_y;
    int new_i;
    int new_j;

    output = input.clone();
    half_y = input.cols / 2;
    half_x = input.rows / 2;

    for(int i=0; i<input.rows; i++){
        for(int j=0; j<input.cols; j++){
            new_i = (i + half_x) % input.rows;
            new_j = (j + half_y) % input.cols;
            output.at<float>(new_i,new_j) = input.at<float>(i,j);
        }
    }
    return output;
}

Mat fft_math(Mat &input1, Mat &input2){
    Mat im1;
    Mat im2;

    //Deep copy to protect original contents
    input1.copyTo(im1);
    input2.copyTo(im2);

    debug("Flipping im2 left-right and up-down");
    flip(im2,im2,-1);

    debug("FFT calls");
    dft(im1,im1,DFT_COMPLEX_OUTPUT);
    dft(im2,im2,DFT_COMPLEX_OUTPUT);

    debug("Multipy on element-by-element basis");
    im1 = im1.mul(im2);

    /*
        %fftshift moves fft so zero frequency 
        %component is in middle, ifft2 inverse fft    debug("Reverse FFT");
        f=fftshift(abs(ifft2(tmp))); 
    */
    debug("Reverse fft");
    idft(im1,im1,DFT_REAL_OUTPUT);

    debug("Abs of result");    
    im1 = abs(im1);

    debug("Shift zero frequencies to the middle");
    im1 = fftshift(im1);

    return im1;
}

Mat register_image(Mat input1, Mat input2){
    Mat im1;
    Mat im2;
    Point max;
    int x;
    int y;
    int shid;
    Mat fft_return;

    //Deep copies to keep from changing original
    debug("Deep Copying");
    input1.copyTo(im1);
    input2.copyTo(im2);

    if(im1.channels() == 1 && im2.channels() == 1){
        Mat window;
        Mat im1_mean;
        Mat im2_mean;

        debug("Mitigating Boundary Effect");
        window = gen_window(im1.rows,im1.cols,0.05,0.05,im1.channels());

        debug("Got window back");
        im1 = window.mul(im1);
        im2 = window.mul(im2);

        debug("Fake Normalizing");
        //Normalize result
        im1.copyTo(im1_mean);
        im2.copyTo(im2_mean);
        im1_mean.setTo(mean(im1));
        im2_mean.setTo(mean(im2));
        divide(im1,im1_mean,im1,1);
        divide(im2,im2_mean,im2,1);

        debug("Do fft transforms");
        im2_mean = fft_math(im1,im2);

        debug("Returning shifted img");
        fft_return = fft_math(im1,im2);
    }else if(im1.channels() == 3 && im2.channels() == 3){
        //TODO:  Test to see if one channel images can be done this way?
            //Not sure what all the windowing and dividing accomplishes.
        vector<Mat> channels_im1;
        vector<Mat> channels_im2;

        split(im1,channels_im1);
        split(im2,channels_im2);
        fft_return = Mat::zeros(im1.rows,im1.cols,CV_32FC1);

        for(int i = 0; i < channels_im1.size(); i++){
            fft_return = fft_return + fft_math(channels_im1[i],channels_im2[i]);
        }
    }else{
        cerr << "Passed wrong image type to register_image:" 
            << input1.channels() 
            << ", " 
            << input2.channels() << endl;
        exit(1);
    }
    debug("Finding max loc");
    //Find maximum element
    minMaxLoc(
        fft_return, 
        NULL, NULL, NULL, //Don't want the max/min value or min location
        &max, //Want the maximum location
        Mat::ones(fft_return.rows, fft_return.cols, CV_8UC1) //No masking eles
    );

    //Shift based on that maximum
    cerr << "MAX: " << max.x << "," << max.y << endl;
    x = max.y - (im1.rows/2);
    y = max.x - (im1.cols/2);
    cerr << "SHIFT: " << x << ", " << y << endl;

    return shiftMat(input2,x,y);   
}
