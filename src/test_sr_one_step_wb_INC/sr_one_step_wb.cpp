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

float subpixelregister(input_image *hr_image, input_image *lr_image, int ds, int sigma){
	hr_image->horizontal_shift = 1;
	hr_image->vertical_shift = 2;
	return 0.95;
}

/* This function calls the subpixel_register function and creates the final super-resolution output. 
It is given a source image (mainly for dimensions?) and the vector of input images, and returns a Mat.*/
Mat sr_one_step_wb(input_image &src, vector <input_image> &input);

int main(int argc, char* argv[]){
	/*if ( argc < 2) {
    cout << "Usage: functioncall ImagetoUse" << endl;
    return -1;
    }*/
/*	cout << "first" << endl;
int rescale_factor = 4;
input_image image;
input_image curr;
cout << "second" << endl;
vector <input_image> input;
curr.file = imread("1.jpg");
//imshow("pop", curr.file);
resize(curr.file, curr.file, Size(), 4, 4, CV_INTER_AREA);
input.push_back(curr);
curr.file = imread("1.jpg");
resize(curr.file, curr.file, Size(), 4, 4, CV_INTER_AREA);
input.push_back(curr);
curr.file = imread("1.jpg");
resize(curr.file, curr.file, Size(), 4, 4, CV_INTER_AREA);
input.push_back(curr);
curr.file = imread("1.jpg");
resize(curr.file, curr.file, Size(), 4, 4, CV_INTER_AREA);
input.push_back(curr);
curr.file = imread("1.jpg");
resize(curr.file, curr.file, Size(), 4, 4, CV_INTER_AREA);
input.push_back(curr);
cout << "third" << endl;
image.file = imread("1.jpg");
*/
Mat kron_image;
//resize(image.file, image.file, Size(), 4, 4, CV_INTER_AREA);

Mat output = sr_one_step_wb(image, input);
cout << "working" << endl;
for(int i = 0; i<input.size(); i++){
	cout << "horizontal shift: " << input[i].horizontal_shift << endl;
}

system("pause");
	return 0;
}

/* This function calls the subpixel_register function and creates the final super-resolution output. 
It is given a source image (mainly for dimensions?) and the vector of input images, and returns a Mat.*/
Mat sr_one_step_wb(input_image &src, vector <input_image> &input){
	Mat kron_image;
	cout << "1fourth" << endl;
	//input_image curr;
	
	//int input_size = sizeof(input_image);
	//calls sub_pixel register to assign shifts and probabilities for each input image
	for (int tid = 0; tid < input.size(); tid++){
		//curr = input[tid];
		cout<<"I made it "<<tid<<"times!"<<endl;
		input[tid].prob = subpixel_register(&input[tid], &src, 4, -1);
		//cout << src.horizontal_shift <<
	}
	cout << "2fourth" << endl;
	Mat image(src.file.rows, src.file.cols, CV_32F, Scalar(0));
	Mat sh_image(src.file.rows, src.file.cols, CV_32F, Scalar(0));
	cout << "fifth" << endl;
	for (int tid = 0; tid < input.size(); tid++){
		//Resize image by a scale of 4, which was done with a kron product in matlab
		Mat kron_image;
		cout << "sixth" << endl;
		resize(input[tid].file, kron_image, Size(), 4, 4, CV_INTER_AREA);
		cout << "seventh" << endl;
		//if the probability for the input image is high, add the shift to the output
		if (input[tid].prob > 0.9){
			//sh_image=shiftMat(kron_image, -input[tid].vertical_shift, -input[tid].horizontal_shift);
			image += sh_image;
		}
	}
	return image;
}
