#ifdef _MSC_VER
#else
	#include <opencv/cv.h>
	#include <opencv2/core/core.hpp>
	#include <opencv2/highgui/highgui.hpp>
#endif
#include <iostream>
#include "../final/gen_window.h"
#include "../final/image_manipulations.h"
#include <vector>

using namespace cv;
using namespace std;

int main(int argc, char  *argv[])
{
	Mat image = imread("/usr/share/pixmaps/faces/coffee.jpg", CV_LOAD_IMAGE_COLOR);
	Mat temp = image;
	Mat anotherImage = gen_window( image.rows,  image.cols, 0.1, 0.1, 4);
	imshow("WORKS1!", anotherImage);

	//imshow("before", image);

    /*int channels = image.channels();
	vector<Mat> planes;
	split(image, planes);*/
    //temp.convertTo(planes[1], anotherImage.depth());

    //imshow("after", planes[1]);


    cout<<anotherImage.channels()<<endl;
	if(image.depth() == CV_8U) cout<<"found a problem"<<endl<<endl;

	if(image.depth() == CV_8U) cout<<"found a problem"<<endl<<endl;

	//for(int i = 0; i<channels;i++)
	//Mat hello = planes[1] * anotherImage;

	//imshow("WORKS!", hello);

	waitKey(0);

	return 0;
}