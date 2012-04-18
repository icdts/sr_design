#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>
#include "../test_subpixel_register/subpixel_register.h"

using namespace std;
using namespace cv;

struct shifts{
	float y;
	float x;
};

vector <float[290]> scores;
float shiftx = 0, shifty = 0;
float prob = 0;

IplImage *sr_one_step_wb(const IplImage *src, vector <IplImage> input);

int main(int argc, char* argv[]){
	if ( argc < 2) {
    cout << "Usage: functioncall ImagetoUse" << endl;
    return -1;
    }

  int rescale_factor = 4;

  IplImage* image;
  IplImage* kron_image;
  IplImage* return_image;
  vector <IplImage> input;
  image = cvLoadImage(argv[1], CV_LOAD_IMAGE_COLOR);
  
  
  //kron_image = shiftImage(kron_image, -7, 6); 
	IplImage *output = sr_one_step_wb(image, input);

	return 0;
}

IplImage *sr_one_step_wb(const IplImage *src, vector <IplImage> input){
	float tmp_sh;
	vector <shifts> shs;
	vector <float> probs;
	//float score_array[290];
	//float tmp_scores[290];
	IplImage* kron_image;
	
	for (int tid = 0; tid < input.size(); tid++){
		prob = subpixel_register(src,input[tid], 4, -1);
		//fills the probability in the probs vector corresponding to the input
		//image id with its prob from subpixel_register
		probs.push_back(prob);
		//replaces the row in shs corresponding to the input image id with
        //its shift value from subpixel_register
		shs[tid].x = shiftx;
		shs[tid].y = shifty;
		//replaces the index in scores corresponding to the input image id with
        //its scores from subpixel_register
		//scores.push_back(score_array);
	}

	Mat image(src->height, src->width, CV_32F, Scalar(0));
	Mat sh_image(src->height, src->width, CV_32F, Scalar(0));
	for (int tid = 0; tid < input.size(); tid++){
		CvSize size;
		size.width = input[tid].width * 4;
		size.height = input[tid].height * 4;
		//Resize image by a scale of 4, which was done with a kron product in matlab
		kron_image = cvCreateImage(size, input[tid].depth, input[tid].nChannels);
		cvResize(&input[tid], kron_image, CV_INTER_AREA);
		//if the probability for the input image is high, add the shift to the output
		if (probs[tid] > 0.9){
            sh_image=shift_image(kron_image, -shs[tid].y, -shs[tid].x);
			image += sh_image;
		}
	}
}
