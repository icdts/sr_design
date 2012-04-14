#include "../final/image_manipulations.h" 

using namespace std;
using namespace cv;

int main(int argc, char* argv[]){
	if( argc != 2) {
		cout <<" Usage: display_image ImageToLoadAndDisplay" << endl;
		return -1;
    }

	int rescale_factor = 2;

    IplImage *image;
	IplImage *bigger_image;
	IplImage *rotated_image;
	IplImage *shifted_image;
	IplImage *shifted_image2;
	IplImage *shifted_image3;
	IplImage *shifted_image4;

	//Read in Image
    image = cvLoadImage(argv[1], CV_LOAD_IMAGE_COLOR);

	//Transform image
	bigger_image = resizeImage(image,image->width*rescale_factor,image->height*rescale_factor,false);
	rotated_image = rotateImage(image,180);
	shifted_image = shiftImageRot(image,0,25);
	shifted_image2 = shiftImageRot(image,0,-25);
	shifted_image3 = shiftImageRot(image,25,0);
	shifted_image4 = shiftImageRot(image,-25,0);

	//Show all the images
    namedWindow( "original", CV_WINDOW_AUTOSIZE );
    namedWindow( "bigger", CV_WINDOW_AUTOSIZE );
    namedWindow( "rotated", CV_WINDOW_AUTOSIZE );
    namedWindow( "shifted", CV_WINDOW_AUTOSIZE );
    namedWindow( "shifted2", CV_WINDOW_AUTOSIZE );
    namedWindow( "shifted3", CV_WINDOW_AUTOSIZE );
    namedWindow( "shifted4", CV_WINDOW_AUTOSIZE );
    cvShowImage( "original", image );
    cvShowImage( "bigger", bigger_image );
    cvShowImage( "rotated", rotated_image );
    cvShowImage( "shifted",	shifted_image );
    cvShowImage( "shifted2",shifted_image2 );
    cvShowImage( "shifted3",shifted_image3 );
    cvShowImage( "shifted4",shifted_image4 );
    waitKey(0);

	//Release memory
	cvReleaseImage(&image);
	cvReleaseImage(&bigger_image);
	cvReleaseImage(&rotated_image);
	cvReleaseImage(&shifted_image);
	cvReleaseImage(&shifted_image2);
	cvReleaseImage(&shifted_image3);
	cvReleaseImage(&shifted_image4);
    return 0;	
}
