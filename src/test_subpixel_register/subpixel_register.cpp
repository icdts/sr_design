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
float* subpixel_register(const IplImage *hr_image, const IplImage *lr_image, 
                        int ds, int sigma, int &shx, int &shy, int &prob); 

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

  int shift_x = 0; int shift_y = 0;
  int prob = 0;
  float scores[290];

  scores = subpixel_register(image, kron_image, 4, -1, &shift_x, &shift_y, &prob);


  namedWindows ("original", CV_WINDOW_AUTOSIZE);
  namedWindows ("kron'd", CV_WINDOW_AUTOSIZE);
  cvShowImage  ("original", image);
  cvShowImage  ("kron'd", kron_image);
  
  cout << "Best shift:  " << shift_x << ", " << shift_y << ")\n";
  cout << "Probability: " << prob << "\n";
  cout << "Best score:  " << score[289];
  
  waitKey(0);

  cvReleaseImage(&image);
  cvReleaseImage(&kron_image);
  return 0;
}

float* subpixel_register(const IplImage *hr_image, const IplImage *lr_image, 
                          int ds, int sigma, int &shx, int &shy, int &prob){
  if (sigma < 0)
    sigma = 40;

	int sid = 1;
	float sigma = 2 * sigma * sigma; //2*40*40, don't know why
	
  Mat lr(lr_image);
	Mat t;
      
  CvMat *lr_mat = cvCreateMat(lr_image->height, lr_image->width, CV_32FC3 );
  CvMat *t_mat = cvCreateMat(lr_image->height, lr_image->width, CV_32FC3 );
  CvMat *reduced = cvCreateMat(1, lr_image->width, CV_32FC3 );
  int final_score = 0;
  cvConvert (lr_image, lr_mat);


	float score_array[290]; //used to save every score
	for(int i = -8; i<9; i++){
		for(int j = -8; i<9; i++){
			t=genShiftDownsampleImage(hr_image, i, j, ds);

      if (t->width != lr_image->width){
        cout <<" Troubles: the two images are not the right size, sorry)" << endl;
        return 1;
      }

      cvConvert (t, t_mat);
      t_mat = (t_mat - lr_mat);
      cvExp( -cvPow(t_mat, t_mat, 2) / sigma, t_mat);
      cvReduce(t_mat, reduced, CV_REDUCE_AVG);
      temp = temp * t->height;
      cvAvg(reduced, final_score);
      final_score = final_score * t->width;
      //Avg( *width                           
      // Who knows why, but just copy these maths
      // prod(x): treat columns of x as arrays and returns a row vector of the product for each column 
      // exp(x): e raised to the x
      // x.^a: element-by-element power raising (each element of x is
      // raised to the a individually
      //    score(sid)=prod(prod(exp(-(t-lr_image).^2/(2*sigma*sigma))));
      //    sid=sid+1;
      
      score_array[sid] =  final_score;
      sid += 1;
      final_score = 0;

			//if score is highest, set temp_sh[] to be those i and j values
		}
	}
  cvReleaseMat(&reduced);
  cvReleaseMat(&t_mat);
  cvReleaseMat(&lr_mat);

	return score_array;
}
