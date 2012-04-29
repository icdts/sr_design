#ifdef _MSC_VER
	//FIXME: Windows includes
#else
	#include <opencv/cv.h>
	#include <opencv2/core/core.hpp>
	#include <opencv2/highgui/highgui.hpp>
#endif
#include <iostream>
#include "../final/image_manipulations.h"
#include "../final/load_images.h"

using namespace std;
using namespace cv;

int main(int argc, char* argv[]){
	if( argc != 2) {
		cout <<" Usage: display_image ImageToLoadAndDisplay" << endl;
		return -1;
  }

  Mat image = imread( argv[1] );
  Mat kron_image;
  resize (image, kron_image, Size(), 4, 4, CV_INTER_AREA);
  //kron_image = shiftMat(kron_image, 0, 0);
	
	//Transform image
	Mat gsds_image = genShiftDownsampleMat( kron_image, 0, 0, 4 );
  Mat gsds_image1 = genShiftDownsampleMat( kron_image, 100, 0, 4 );
  Mat gsds_image2 = genShiftDownsampleMat( kron_image, 0, 100, 4 );
  Mat gsds_image3 = genShiftDownsampleMat( kron_image, 0, 0, 4 );
  Mat gsds_image4 = genShiftDownsampleMat( kron_image, 100, 100, 4 );
	//Show all the images
 namedWindow( "gsds_image", CV_WINDOW_AUTOSIZE );
  imshow( "gsds_image",	gsds_image );
 namedWindow( "gsds_image1", CV_WINDOW_AUTOSIZE );
  imshow( "gsds_image1",	gsds_image1 );
 namedWindow( "gsds_image2", CV_WINDOW_AUTOSIZE );
  imshow( "gsds_image2",	gsds_image2 );
 namedWindow( "gsds_image3", CV_WINDOW_AUTOSIZE );
  imshow( "gsds_image3",	gsds_image3 );
 namedWindow( "gsds_image4", CV_WINDOW_AUTOSIZE );
  imshow( "gsds_image4",	gsds_image4 );
  waitKey(0);

	return 0;
}
