#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>
#include "../final/image_manipulations.h"
#include "../final/load_images.h"
#include "../test_subpixel_register/subpixel_register.h"

using namespace std;
using namespace cv;

/* This function calls the subpixel_register function and creates the final super-resolution output. 
It is given a source image (mainly for dimensions?) and the vector of input images, and returns a Mat.*/
Mat sr_one_step_wb(Mat src, vector <input_image> input);

int main(int argc, char* argv[]){
	if ( argc < 2) {
    cout << "Usage: functioncall ImagetoUse" << endl;
    return -1;
    }

int rescale_factor = 4;
Mat image;
vector <input_image> input;
image = imread(argv[1]);
Mat output = sr_one_step_wb(image, input);

	return 0;
}

/* This function calls the subpixel_register function and creates the final super-resolution output. 
It is given a source image (mainly for dimensions?) and the vector of input images, and returns a Mat.*/
Mat sr_one_step_wb(Mat src, vector <input_image> input){
	Mat kron_image;
	
	//calls sub_pixel register to assign shifts and probabilities for each input image
	for (int tid = 0; tid < input.size(); tid++){
		input[tid].prob = subpixel_register(src, input[tid], 4, -1);
	}

	Mat image(src.rows, src.cols, CV_32F, Scalar(0));
	Mat sh_image(src.rows, src.cols, CV_32F, Scalar(0));
	for (int tid = 0; tid < input.size(); tid++){
		//Resize image by a scale of 4, which was done with a kron product in matlab
		Mat kron_image = Mat( image.size(), CV_64F );
		resize(image, kron_image, kron_image.size(), 0, 0, CV_INTER_AREA);
		//if the probability for the input image is high, add the shift to the output
		if (input[tid].prob > 0.9){
			sh_image=shiftImage(kron_image, -input[tid].vertical_shift, -input[tid].horizontal_shift);
			image += sh_image;
		}
	}
	return image;
}