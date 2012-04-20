#include "../final/image_manipulations.h"
#include "../final/load_images.h"
#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <stdio.h>


using namespace std;
using namespace cv;	
#define height() rows
#define width() cols

//When values are passed in hr_image should be the original image kroned with ds 1
//    i.e. should be the image with each pixel expanded to fill 4 times the space
//
//lr_image is the origianl image beign worked on
//This way the 'gen_shift_downsample'd image has the same size as the original
//
//Passing in addresses lets you return multiple values, bad practice probably
float subpixel_register(input_image hr_image, input_image lr_image, int ds, int sigma); 

int main(int argc, char* argv[]){
  if ( argc < 2) {
    cout << "Usage: functioncall ImagetoUse" << endl;
    return -1;
    }

  int rescale_factor = 4;

  Mat image = imread( argv[1] );
  Mat kron_image = Mat( image.size(), CV_64F );
  resize(image, kron_image, kron_image.size(), 0, 0, CV_INTER_AREA);
  //kron_image = shiftImage(kron_image, -7, 6); 

  input_image original;
  original.name = "Original";
  original.file = image;
  original.vertical_shift = 0;
  original.horizontal_shift = 0;
  original.score = 0;
  original.prob = 0;

  input_image krond;
  krond.name = "Original";
  krond.file - kron_image;
  krond.vertical_shift = 0;
  krond.horizontal_shift = 0;
  krond.score = 0;
  krond.prob = 0;

  float prob = subpixel_register(krond, original, 4, -1);


  namedWindow ("original", CV_WINDOW_AUTOSIZE);
  namedWindow ("kron'd", CV_WINDOW_AUTOSIZE);
  imshow ("original", image);
  imshow ("kron'd", kron_image);
  
  waitKey(0);

  image.release();
  kron_image.release();
  return 0;
}

float subpixel_register(input_image hr_image, input_image lr_image, int ds, int sigma)
{
  if (sigma < 0)
    sigma = 40;

	int sid = 0;

	float sigma2 = 2 * sigma * sigma; //2*40*40, don't know why
      
  Mat lr_mat;
  Mat tmp = Mat( lr_image.file );
  tmp.convertTo( lr_mat, CV_64F );
  Mat t_mat   = Mat( lr_image.file.size(), CV_64F );
  Mat reduced = Mat( 1, lr_image.file.width(), CV_64F );
  Mat t;

  int final_score = 0;
	float score_array[289]; //used to save every score
  
  int curr_best_score = 0; //XXX:placeholders
  int curr_best_x = 0; int curr_best_y = 0; //XXX: placeholders

	for(int i = -8; i<9; i++){
		for(int j = -8; j<9; j++){

			t = genShiftDownsampleMat( hr_image.file , i, j, ds );
      Mat temp = Mat(t);
      temp.convertTo(t_mat, CV_64F);

      if ( ( hr_image.file.width() ) / 4  !=   lr_image.file.width() ){
        cout <<" Troubles: the two images are not the right size, sorry)" << endl;
        return 1;
      }
      
      t_mat = t_mat - lr_mat;
      pow(t_mat, 2, t_mat);
      t_mat = -( t_mat / sigma2 );
      exp( t_mat, t_mat);
      reduce(t_mat, reduced, 0, CV_REDUCE_AVG);
      reduced = reduced * lr_image.file.height();
      final_score = mean(reduced)[0];
      final_score = final_score * lr_image.file.width();
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
  hr_image.score = curr_best_score;
  hr_image.horizontal_shift = curr_best_x;
  hr_image.vertical_shift = curr_best_y;
  for (int i=0; i<289; i++){
    hr_image.prob += score_array[i];
    }
  hr_image.prob = hr_image.prob / 289;

	return hr_image.prob;
}
