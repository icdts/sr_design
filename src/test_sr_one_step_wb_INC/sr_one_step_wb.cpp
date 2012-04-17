#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

struct shifts{
	float x;
	float y;
}

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

  image = cvLoadImage(argv[1], CV_LOAD_IMAGE_COLOR);
  
  
  //kron_image = shiftImage(kron_image, -7, 6); 
	IplImage *output = sr_one_step_wb(image, input);

	return 0;
}

IplImage *sr_one_step_wb(const IplImage *src, vector <IplImage> input){
	float tmp_sh;
	vector <shifts> shs;
	vector <float> probs;
	float score_array[290];
	float tmp_scores[290];
	IplImage* kron_image;
	for (int tid = 0; tid < input.size(); tid++){
		prob = subpixel_register(src,input[tid], 4, -1);
		probs.push_back(prob);
		shs[tid].x = shiftx;
		shs[tid].y = shifty;
		scores.push_back(score_array);
	}

	Mat image(src->width, src->height, CV_32F, Scalar(0));
	Mat sh_image(src->width, src->height, CV_32F, Scalar(0));
	for (int tid = 0; tid < input.size(); tid++){
		CvSize size;
		size.width = input[tid]->width * 4;
		size.height = input[tid]->height * 4;
		kron_image = cvCreateImage(size, input[tid]->depth, input[tid]->nChannels);
		cvResize(input[tid], kron_image, CV_INTER_AREA);
		if (probs[tid] > 0.9){
            sh_image=shift_image(kron_image, -shs[tid].x, -shs[tid].y);
			image += sh_image;
		}
}
