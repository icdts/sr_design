#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>
#include "../final/image_manipulations.h"

using namespace std;
using namespace cv;

struct shifts{
	int y;
	int x;
};

//vector <float[290]> scores;
float shiftx = 0, shifty = 0;
float prob = 0;

float subpixel_register(IplImage* hr_image, IplImage* lr_image, 
                        int ds, int sigma); 
IplImage *sr_one_step_wb( IplImage *src, vector <IplImage> input);

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

IplImage *sr_one_step_wb(IplImage *src, vector <IplImage> input){
	//float tmp_sh;
	vector <shifts> shs;
	vector <float> probs;
	//float score_array[290];
	//float tmp_scores[290];
	IplImage* kron_image;
	
	for (int tid = 0; tid < input.size(); tid++){
		prob = subpixel_register(src,&input[tid], 4, -1);
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
            sh_image=shiftImage(kron_image, -shs[tid].y, -shs[tid].x);
			image += sh_image;
		}
	}
}

float subpixel_register(IplImage* hr_image, IplImage* lr_image, 
                          int ds, int sigma){
  if (sigma < 0)
    sigma = 40;

	int sid = 0;
  int shift_x = 0; int shift_y = 0; //XXX:placholders for struct
  float prob = 0; ///XXX: placeholder for struct

	float sigma2 = 2 * sigma * sigma; //2*40*40, don't know why
      
  Mat lr_mat;
  Mat tmp = Mat(lr_image, true );
  tmp.convertTo(lr_mat, CV_32F);
  Mat t_mat   = Mat(lr_image->height, lr_image->width, CV_32F );
  Mat reduced = Mat(1, lr_image->width, CV_32F );
  IplImage *t;

  int final_score = 0;


	float score_array[290]; //used to save every score
  
  int curr_best_score = 0; //XXX:placeholders
  int curr_best_x = 0; int curr_best_y = 0; //XXX: placeholders

	for(int i = -8; i<9; i++){
		for(int j = -8; j<9; j++){

			t = genShiftDownsampleImage( hr_image, i, j, ds );
      Mat temp = Mat(t);
      temp.convertTo(t_mat, CV_32F);


      if ((hr_image->width)/4 != lr_image->width){
        cout <<" Troubles: the two images are not the right size, sorry)" << endl;
        return 1;
      }
      
      t_mat = t_mat - lr_mat;
      pow(t_mat, 2, t_mat);
      t_mat = -( t_mat / sigma2 );
      exp( t_mat, t_mat);
      reduce(t_mat, reduced, 0, CV_REDUCE_AVG);
      reduced = reduced * lr_image->height;
      final_score = mean(reduced)[0];
      final_score = final_score * lr_image->width;
      cout << "Score for " << i << ", " << j << " is: " << final_score << endl;

      // Who knows why, but just copy these maths
      //    score(sid)=prod(prod(exp(-(t-lr_image).^2/(2*sigma*sigma))));
      
      score_array[sid] =  final_score;
      sid += 1;
      if (final_score > curr_best_score){
        curr_best_score = final_score;
        curr_best_x = i; 
        curr_best_y = j;
      }

      final_score = 0;

			//if score is highest, set temp_sh[] to be those i and j values
		}
	}

  //XXX: placeholder for struct
  score_array[289] = curr_best_score;
  shift_x = curr_best_x;
  shift_y = curr_best_y;
  for (int i=0; i<290; i++){
    prob += score_array[i];
    }
  prob = prob / 289;

  cout << "Best shift:  (" << shift_x << ", " << shift_y << ")\n";
  cout << "Probability: " << prob << "\n";
  cout << "Best score:  " << score_array[289];

	return 1;
}

