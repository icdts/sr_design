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
//<<<<<<< HEAD
int rescale_factor = 4;
input_image image;
input_image curr;
curr.prob = 0;
curr.horizontal_shift = 0;
curr.vertical_shift = 0;
vector <input_image> input;
curr.file = imread("1.jpg");

curr.file = shiftMat(curr.file, -3, 4);
input.push_back(curr);
curr.file = imread("2.jpg");

curr.file = shiftMat(curr.file, -6, 0);
input.push_back(curr);
curr.file = imread("3.jpg");

curr.file = shiftMat(curr.file, 3, 4);
input.push_back(curr);
curr.file = imread("4.jpg");

curr.file = shiftMat(curr.file, 0, -4);
input.push_back(curr);
//curr.file = imread("1.jpg");

//curr.file = shiftMat(curr.file, 5, -2);
//input.push_back(curr);
cout << "third" << endl;
image.file = imread("1.jpg");

Mat kron_image;
resize(image.file, image.file, Size(), 2, 2, CV_INTER_AREA);

Mat output = sr_one_step_wb(image, input);
cout << "working" << endl;
for(int i = 0; i<input.size(); i++){
	cout << "horizontal shift: " << input[i].horizontal_shift;
	cout << ", vertical shift: " << input[i].vertical_shift;
	cout << ", probability: " << input[i].prob << endl;
}
imshow("pop", output);
waitKey(0);
GaussianBlur(output, output, cv::Size(0, 0), 3);
addWeighted(output, 1.5, output, -0.5, 0, output);
imshow("blurred", output);
waitKey(0);

	return 0;
}

/* This function calls the subpixel_register function and creates the final super-resolution output. 
It is given a source image (mainly for dimensions?) and the vector of input images, and returns a Mat.*/
Mat sr_one_step_wb(input_image &src, vector <input_image> &input){
	Mat kron_image;
	cout << "1fourth" << endl;
	//calls sub_pixel register to assign shifts and probabilities for each input image
	for (int tid = 0; tid < input.size(); tid++){
		//curr = input[tid];
		cout<<"I made it "<<tid<<"times!"<<endl;
		input[tid].prob = subpixel_register(&src, &input[tid], 2, -1);
			cout << "horizontal shift: " << input[tid].horizontal_shift;
	cout << ", vertical shift: " << input[tid].vertical_shift;
	cout << ", probability: " << input[tid].prob << endl;
		//cout << src.horizontal_shift <<
	}
	cout << "2fourth" << endl;
	//Mat image = Mat::zeros(src.file.rows, src.file.cols,CV_32FC1);
	Mat image = input[0].file;
	image.reshape(2, src.file.rows);
	Mat sh_image(src.file.rows, src.file.cols, CV_32FC1, Scalar(0));
	cout << "fifth" << endl;
	resize(image, image, Size(), 2, 2, CV_INTER_AREA);
	for (int tid = 0; tid < input.size(); tid++){
		//Resize image by a scale of 4, which was done with a kron product in matlab
		
		cout << "sixth" << endl;
		resize(input[tid].file, kron_image, Size(), 2, 2, CV_INTER_AREA);
		cout << "seventh" << endl;
		//imshow("1", kron_image);
		//waitKey(0);
		//imshow("2", image);
		//waitKey(0);
		//if the probability for the input image is high, add the shift to the output
		if (input[tid].prob > 0.0){
			sh_image=shiftMat(kron_image, -input[tid].vertical_shift, -input[tid].horizontal_shift);
			imshow("3", sh_image);
			waitKey(0);
			cout << "image rows: " << image.rows << ", sh_image rows: " << sh_image.rows << endl;
			cout << "image cols: " << image.cols << ", sh_image cols: " << sh_image.cols << endl;
			image = (sh_image + image)/2;
			//kron_image = (kron_image + kron_image) / 2;
			//imshow("4", kron_image);
			//waitKey(0);
	}
	}
	return image;
}