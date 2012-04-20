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

Mat sr_one_step_wb(Mat src, vector <input_image> input){
	Mat kron_image;
	
	for (int tid = 0; tid < input.size(); tid++){
		input[tid].prob = subpixel_register(src,&input[tid], 4, -1);
		//fills the probability in the probs vector corresponding to the input
		//image id with its prob from subpixel_register
		//probs.push_back(prob);
		//replaces the row in shs corresponding to the input image id with
        //its shift value from subpixel_register
		//shs[tid].x = shiftx;
		//shs[tid].y = shifty;
		//replaces the index in scores corresponding to the input image id with
        //its scores from subpixel_register
		//scores.push_back(score_array);
	}

	Mat image(src.rows, src.cols, CV_32F, Scalar(0));
	Mat sh_image(src.rows, src.cols, CV_32F, Scalar(0));
	for (int tid = 0; tid < input.size(); tid++){
		CvSize size;
		size.width = input[tid].file.cols * 4;
		size.height = input[tid].file.rows * 4;
		//Resize image by a scale of 4, which was done with a kron product in matlab
		kron_image = cvCreateImage(size, input[tid].depth, input[tid].nChannels);
		cvResize(&input[tid], kron_image, CV_INTER_AREA);
		//if the probability for the input image is high, add the shift to the output
		if (input[tid].prob > 0.9){
			sh_image=shiftImage(kron_image, -input[tid].vertical_shift, -input[tid].horizontal_shift);
			image += sh_image;
		}
	}
}