#include "../final/image_manipulations.h"
#include "../final/load_images.h"
#include "subpixel_register.h"
#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <stdio.h>


using namespace std;
using namespace cv;	

#define height() rows
#define width() cols

// When values are passed in hr_image should be the original image kroned with ds 1
//    i.e. should be the image with each pixel expanded to fill 4 times the space
//
// lr_image is the origianl image beign worked on
// This way the 'gen_shift_downsample'd image has the same size as the original
//
// Passing in addresses lets you return multiple values, bad practice probably

/*int main(int argc, char* argv[]){
  if ( argc < 2) {
    cout << "Usage: functioncall ImagetoUse" << endl;
    return -1;
    }

  int rescale_factor = 4;

  Mat image = imread( argv[1] );
  Mat kron_image;
  resize(image, kron_image, Size(), 4, 4, CV_INTER_AREA);
  kron_image = shiftMat(kron_image, 8, 8); 

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

  float prob = subpixel_register(&krond, &original, 4, -1);

  namedWindow ("original", CV_WINDOW_AUTOSIZE);
  namedWindow ("kron'd", CV_WINDOW_AUTOSIZE);
  imshow ("original", original.file);
  imshow ("kron'd", krond.file);
 
  krond.prob = krond.prob / 289;
  cout << "Best score: " << krond.score << endl;
  cout << "Shift: (" << krond.horizontal_shift << ", " << krond.vertical_shift << ")\n";
  cout << "Prob: " << krond.prob << endl; 
  
  waitKey(0);

  return 0;
}*/

float subpixel_register(input_image *hr_input, input_image *lr_input, int ds, int sigma){
  debug("SubpixelRegister called");
  
  if (sigma < 0)
    sigma = 40;

	int sid = 0; //iterator for the score array
	float sigma2 = 2 * sigma * sigma; //2*40*40, don't know why
      
  Mat lr_image, hr_image;
  lr_input->file.copyTo(lr_image);//create a copy of the matrix contained in the struct
  hr_input->file.copyTo(hr_image);//create a copy...

  lr_image.convertTo( lr_image, CV_64F );//convert to 64F so exp and reduce work correctly
  Mat t_mat;
  Mat reduced = Mat( 1, lr_image.width(), CV_64F );//a (1 x width) array to store column averages

  int final_score = 0;
	float score_array[289]; //used to save every score

  debug("Shift around");
	for(int i = -8; i<9; i++){
		for(int j = -8; j<9; j++){
      // these loops shift the image in every possible combination
      // from (-8,-8) to (8,8) and determines a score based on some
      // code from the matlab implementation, the score is usually a
      // number in the thousands

      if ( ( hr_image.width()) / 4  !=   lr_image.width() ){
        cout <<" Troubles: the two images are not the right size, sorry)" << endl;
        cout <<"hr: " << hr_image.width() << " lr: " << lr_image.width() << endl;
        return 1;
      }
			
      Mat t = genShiftDownsampleMat( hr_image, i, j, ds );
	  debug("Converting to floats");
      //shift the kronned image and downsample it so it's the same size as lr
      t.convertTo(t_mat, CV_64F); //convert to a 64F matrix and store in t_mat

      // I do not know this math, it came from the MATLAB code: 
      //  score(sid)=prod(
      //                  prod(
      //                        exp(
      //                            -(t-lr_image).^2 
      //                            /(2*sigma*sigma)
      //                            )
      //                        )
      //                )
	  debug("Math start");
      t_mat = t_mat - lr_image;                 
      pow(t_mat, 2, t_mat);                     
      t_mat = -( t_mat / sigma2 ); // sigma2 = 2*sigma*sigma 
	  debug("Exponentiating");
      exp( t_mat, t_mat);
      reduce(t_mat, reduced, 0, CV_REDUCE_AVG);
      reduced = reduced * lr_image.height();
	  debug("Getting score");
      final_score = mean(reduced)[0];
      final_score = final_score * lr_image.width();
      //cout << "Score for " << i << ", " << j << " is: " << final_score << endl;

      score_array[sid] =  final_score; //store the resulting score in the array
      sid += 1;
      if (final_score > lr_input->score){
        // if the new score is better put a new best in the passed image
        // since it was passed by reference this changes the input_image
        // struct in the above function as well
        lr_input->score = final_score;
        lr_input->horizontal_shift = i; 
        lr_input->vertical_shift = j;
      }

      final_score = 0; //reset score
      }
	}

  debug("determine probability by dividing best score by sum of scores");
  //determine probability by dividing best score by sum of scores
  for (int i=0; i<289; i++){
    lr_input->prob += score_array[i];
	//cout << "score at this blasted place is  " << score_array[i] << endl;
  }
  lr_input->prob = lr_input->score / lr_input->prob;

	return lr_input->prob;
}