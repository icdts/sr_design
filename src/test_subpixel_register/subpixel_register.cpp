#include "../final/image_manipulations.h"
#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <stdio.h>


using namespace std;
using namespace cv;	

//When values are passed in hr_image should be the original image kroned with ds 1
//    i.e. should be the image with each pixel expanded to fill 4 times the space
//
//lr_image is the origianl image beign worked on
//This way the 'gen_shift_downsample'd image has the same size as the original
//
//Passing in addresses lets you return multiple values, bad practice probably
float subpixel_register(IplImage* hr_image, IplImage* lr_image, 
                        int ds, int sigma); 

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
  
  CvSize size;
  size.width = image->width * 4;
  size.height = image->height * 4;
  kron_image = cvCreateImage(size, image->depth, image->nChannels);
  cvResize(image, kron_image, CV_INTER_AREA);
  kron_image = shiftImage(kron_image, -7, 6); 

  int shift_x = 0; int shift_y = 0;
  int prob = 0;
  float dummy = 0; //XXX: placeholder for struct

  dummy = subpixel_register(kron_image, image, 4, -1);


  namedWindow ("original", CV_WINDOW_AUTOSIZE);
  namedWindow ("kron'd", CV_WINDOW_AUTOSIZE);
  cvShowImage ("original", image);
  cvShowImage ("kron'd", kron_image);
  
  waitKey(0);

  cvReleaseImage(&image);
  cvReleaseImage(&kron_image);
  return 0;
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
