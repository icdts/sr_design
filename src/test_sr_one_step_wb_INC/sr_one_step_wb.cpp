#include "sr_one_step.h"

using namespace std;
using namespace cv;

/* This function calls the subpixel_register function and creates the final super-resolution output. 
It is given a source image (mainly for dimensions?) and the vector of input images, and returns a Mat.*/


/* This function calls the subpixel_register function and creates the final super-resolution output. 
It is given a source image (mainly for dimensions?) and the vector of input images, and returns a Mat.*/
Mat sr_one_step(image_data &src, vector <image_data> &input){
	Mat kron_image;
	//calls sub_pixel register to assign shifts and probabilities for each input image
	for (int tid = 0; tid < input.size(); tid++){
		cout<<"I made it "<<tid<<"times!"<<endl;
		input[tid].prob = subpixel_register(&src, &input[tid], 4, -1);
		//cout << "horizontal shift: " << input[tid].horizontal_shift;
		//cout << ", vertical shift: " << input[tid].vertical_shift;
		//cout << ", probability: " << input[tid].prob << endl;
	}
	Mat image = input[0].file;
	Mat sh_image(src.file.rows, src.file.cols, CV_32FC1, Scalar(0));
	resize(image, image, Size(), 4, 4, CV_INTER_AREA);
	for (int tid = 0; tid < input.size(); tid++){
		//Resize image by a scale of 4, which was done with a kron product in matlab
		resize(input[tid].file, kron_image, Size(), 4, 4, CV_INTER_AREA);
		//if the probability for the input image is high, add the shift to the output
		if (input[tid].prob > 0.0){
			sh_image=shiftMat(kron_image, -input[tid].vertical_shift, -input[tid].horizontal_shift);
			image = (sh_image + image)/2;
		}
	}
	return image;
}
