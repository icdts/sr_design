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
  kron_image = shiftMat(kron_image, 40, 40);
	
	//Transform image
	Mat gsds_image = genShiftDownsampleMat( kron_image, 30, 30, 4 );

	//Show all the images
  namedWindow( "gsds_image", CV_WINDOW_AUTOSIZE );
  imshow( "gsds_image",	gsds_image );
  waitKey(0);

	return 0;
}
