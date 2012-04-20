#include "../final/image_manipulations.h" 

using namespace std;
using namespace cv;

int main(int argc, char* argv[]){
	if( argc != 2) {
		cout <<" Usage: display_image ImageToLoadAndDisplay" << endl;
		return -1;
    }

	int rescale_factor = 2;

    cv::Mat image;
	cv::Mat bigger_image;
	cv::Mat rotated_image;
	cv::Mat shifted_image;
	cv::Mat shifted_image2;
	cv::Mat shifted_image3;
	cv::Mat shifted_image4;
	cv::Mat shifted_image5;
	cv::Mat shifted_image6;
	cv::Mat shifted_image7;
	cv::Mat shifted_image8;

	//Read in Image
    image = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);

	//Transform image
	cv::resize(image,bigger_image,Size(0,0),5,5,INTER_LINEAR);
	cv::flip(image,rotated_image,-1);
	shifted_image = shiftMatRot(image,0,25);
	shifted_image2 = shiftMatRot(image,0,-25);
	shifted_image3 = shiftMatRot(image,25,0);
	shifted_image4 = shiftMatRot(image,-25,0);
	shifted_image5 = shiftMat(image,25,25);
	shifted_image6 = shiftMat(image,-25,-25);
	shifted_image7 = shiftMat(image,25,0);
	shifted_image8 = shiftMat(image,-25,0);
	//Show all the images
    cv::imshow( "original", image );
    cv::imshow( "bigger", bigger_image );
    cv::imshow( "rotated", rotated_image );
    cv::imshow( "1",	shifted_image );
    cv::imshow( "2",shifted_image2 );
    cv::imshow( "3",shifted_image3 );
    cv::imshow( "4",shifted_image4 );
    cv::imshow( "5",	shifted_image5 );
    cv::imshow( "6",shifted_image6 );
    cv::imshow( "7",shifted_image7 );
    cv::imshow( "8",shifted_image8 );

    waitKey(0);

    return 0;	
}
