#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <stdio.h>


using namespace std;
using namespace cv;	

float subpixel_register(const IplImage *hr_image, const IplImage *lr_image, int ds, int sigma); 

int main(int argc, char* argv[]){
/*if( argc != 2) {
     cout <<" Usage: display_image ImageToLoadAndDisplay" << endl;
     return -1;
    }
	*/
	return 0;
}

float subpixel_register(const IplImage *hr_image, const IplImage *lr_image, int ds, int sigma){
	float *prob_ptr;
	float *score_ptr;
	int temp_sh[2]; 

	int sid = 1;
	float sig = 2 * sigma * sigma;
	Mat lr(lr_image);
	Mat t;
	
	float score_array[289];
	for(int i = -8; i<9; i++){
		for(int j = -8; i<9; i++){
			t=genShiftDownsampleImage(hr_image, i, j, ds); //needs #include
			Mat temp = lr - t;
			Mat temp2;
			pow(2.718 * (temp), 2, temp2);
			score_array[sid]=prod(prod(temp2 /(sig))); //xxx
			//if score is highest, set temp_sh[] to be those i and j values
		}
	}
	return temp_sh[];