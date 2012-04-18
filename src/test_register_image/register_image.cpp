#ifdef +MSC_VER
	//XXX: Need something here?
#else
	#include <opencv/cv.h>
	#include <opencv2/core/core.hpp>
	#include <opencv2/highgui/highgui.hpp>
#enfid
#include <iostream>

using namespace std;
using namespace cv;

Mat* register_image(const Mat* img1, const Mat* img2){
    Mat* oim1, oim2; //original images
    CvSize size1, size2;  //image sizes
    cvScalar avg1,avg2;

    size1.width = img1->width;
    size1.height = img1->height;

    size2.width = img1->width;
    size2.height = img2->height;

    oim1 = cvCreateImage(size, img1->depth, 1);
    oim2 = cvCreateImage(size, img2->depth, 1);
    //create copies
    cvCopy(img1, oim1);
    cvCopy(img2, oim2);

    Mat kernel = generate_window(img1, 0.05, 0.05) //external function

    //assuming image 1 and 2 are the same size
    if (kernel->width != img1->width || kernel->width != img1->width){
    	cout << "ERROR: Image1 width is " << kernel->width
    	     << "expected " << kernel->width << "\n";
    	exit(1);
    }
    //TODO: More sanity checking for img1 and img2

    //source1, source2, destination
    Mul(img1, kernel, img1);  // member by member multiply
    Mul(img2, kernel, img2);  // cv functions, check output

    //Get average value of all images
    avg1 = Avg(img1);
    avg2 = Avg(img2);
  
    /*
        Invert, so that scaling will divide 
        by these values.
    */
    for(int i = 0; i < img1->nChannels; i++){
        avg1[i] = 1/avg1[i];
        avg2[i] = 1/avg2[i];
    }

    img1 = cvConvertScale(img1,img1,1/avg1,0);
    img2 = cvConvertScale(img2,img2,1/avg2,0);
    
    //XXX: We have to do FFTs here and there aren't in OpenCV
    //     check for FFTW online (makefiles will need a change)
    //
    // Don't know the math on this, just copying the matlab code
    IblImage temp, f;
    temp = FFT(img1) .* fft(rotateImage(img2, 180)); //external function
    f = FFT_Shift(abs(inverse_FFT(temp));
    //XXX: FFT_Shift could pose a problem Grant has a link for it, but ???
    //     inverse should be included with FFTW

    find maximum value in f;
    shift_id = location of that value;

    //XXX: Don't understand this part of the matlab yet, this isn't done
    // make a point based on the middle of the image
    sh(1)=(mod(shid-1,size(im1,1))+1)-size(im1,1)/2;
    sh(2)=ceil((shid-1)/size(im1,1))-size(im1,2)/2;
    //shift the image based on the (new) midpoint?
    //this is still somewhat unclear
    
    return shift_image(oim2,sh); //external function
}
