#ifdef _MSC_VER
#else
	#include <opencv/cv.h>
	#include <opencv2/core/core.hpp>
	#include <opencv2/highgui/highgui.hpp>
#endif
#include <iostream>
#include "../final/gen_window.h"
#include "../final/image_manipulations.h"

using namespace cv;
using namespace std;

int main(int argc, char  *argv[])
{
	Mat image = imread("/usr/share/pixmaps/faces/coffee.jpg", CV_LOAD_IMAGE_COLOR);
	Mat temp = image;
	Mat anotherImage = gen_window( image.rows,  image.cols, 0.1, 0.1);
	imshow("WORKS1!", anotherImage);

	if(image.depth() == CV_8U) cout<<"found a problem"<<endl<<endl;

    temp.convertTo(image, anotherImage.depth());
    imshow("garbage", image);
	if(image.depth() == CV_8U) cout<<"found a problem"<<endl<<endl;


	image = image * anotherImage;

	imshow("WORKS!", image);

	waitKey(0);

	return 0;
}