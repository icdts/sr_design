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
  Mat kron_image;
  resize(image, kron_image, Size(), 4, 4, CV_INTER_AREA);
  kron_image = shiftMat(kron_image, -7, 6); 

  input_image original;
  original.name = "Original";
  image.copyTo(original.file);
  original.vertical_shift = 0;
  original.horizontal_shift = 0;
  original.score = 0;
  original.prob = 0;

  input_image krond;
  krond.name = "Original";
  kron_image.copyTo(krond.file);
  krond.vertical_shift = 0;
  krond.horizontal_shift = 0;
  krond.score = 0;
  krond.prob = 0;

  float prob = subpixel_register(krond, original, 4, -1);

  namedWindow ("original", CV_WINDOW_AUTOSIZE);
  namedWindow ("kron'd", CV_WINDOW_AUTOSIZE);
  imshow ("original", original.file);
  imshow ("kron'd", krond.file);
  
  waitKey(0);

  return 0;
}

float subpixel_register(input_image hr_input, input_image lr_input, int ds, int sigma)
{
  if (sigma < 0)
    sigma = 40;

	int sid = 0;

	float sigma2 = 2 * sigma * sigma; //2*40*40, don't know why
      
  Mat lr_image, hr_image;
  lr_input.file.copyTo(lr_image);//create a copy of the matrix contained in the struct
  hr_input.file.copyTo(hr_image);//create a copy...

  lr_image.convertTo( lr_image, CV_64F );//convert to 64F so exp and reduce work correctly
  Mat t_mat;
  Mat reduced = Mat( 1, lr_image.width(), CV_64F );//a 1 by width array to store column averages

  int final_score = 0;
	float score_array[289]; //used to save every score

	for(int i = -8; i<9; i++){
		for(int j = -8; j<9; j++){

      if ( ( hr_image.width()) / 4  !=   lr_image.width() ){
        cout <<" Troubles: the two images are not the right size, sorry)" << endl;
        cout <<"hr: " << hr_image.width() << " lr: " << lr_image.width() << endl;
        return 1;
      }
			
      Mat t = genShiftDownsampleMat( hr_image, i, j, ds );
      t.convertTo(t_mat, CV_64F);

      
      t_mat = t_mat - lr_image;

      pow(t_mat, 2, t_mat);
      t_mat = -( t_mat / sigma2 );
      exp( t_mat, t_mat);
      reduce(t_mat, reduced, 0, CV_REDUCE_AVG);
      reduced = reduced * lr_image.height();
      final_score = mean(reduced)[0];
      final_score = final_score * lr_image.width();
      cout << "Score for " << i << ", " << j << " is: " << final_score << endl;


      // Who knows why, but just copy these maths
      //    score(sid)=prod(prod(exp(-(t-lr_image).^2/(2*sigma*sigma))));
      score_array[sid] =  final_score;
      sid += 1;
      if (final_score > hr_input.score){
        hr_input.score = final_score;
        hr_input.horizontal_shift = i; 
        hr_input.vertical_shift = j;
      }

      final_score = 0;
			//if score is highest, set temp_sh[] to be those i and j values
		}
	}

  for (int i=0; i<289; i++){
    hr_input.prob += score_array[i];
  }
  hr_input.prob = hr_input.prob / 289;
  cout << "Best score: " << hr_input.score << endl;
  cout << "Shift: (" << hr_input.horizontal_shift << ", " << hr_input.vertical_shift << ")\n";
  cout << "Prob: " << hr_input.prob << endl;

	return hr_input.prob;
}
