#include "../final/image_manipulations.h"

using namespace std;
using namespace cv;

//function image=pocs(image,lr_image,ds,sh)
IplImage *pocs(IplImage *img, IplImage *low_res_img, float down_scale, int shift_vertical, int shift_horizontal){
	IplImage *tmp_img;
	IplImage *scale;
	CvSize size;

	// Create image the same size of region
	size.width = low_res_img.width;
	size.height = low_res_img.height;
	scale = cvCreateImage(size, low_res_img->depth, low_res_img->nChannels);

	//% shift image and reduce its size by factor of ds
	//sh_image=gen_shift_downsample_image(image,ds,sh);
	tmp_img = genShiftDownsampleImage(img, shift_vertical, shift_horizontal, down_scale);	

	//%right division: A/B = A*B^-1
	//scale=lr_image./sh_image;
	cvDiv(low_res_image,img_img,scale);
	cvReleaseImage(tmp_img);

	//% a=kron(scale,ones(ds))
	//%    expands scale to the output size
	//% shift_image(a,-sh)
	//%   shift a in the opposite direction of sh_image
	//scale=shift_image(kron(scale,ones(ds)),-sh);
	//TODO: Maybe something other than CV_INTER_AREA?  NN perhaps?
	tmp_img = cvResize(scale,CV_INTER_AREA);
	cvReleaseImage(scale);
	scale = shiftImage(tmp_img,-1*shift_vertical, -1*shift_horizontal);
	cvReleaseImage(tmp_img);

	//% multiple every element by scale
	//image=image.*scale;
	size.width = img->width;
	size.height = img->height;
	tmp_img = cvCreateImage(size, img->depth, img->nChannels);
	cvMul(img,scale,tmp_img);

	return tmp_img;
}


int main(int argc, char* argv[]){
	CvSize size;

	if( argc != 2) {
		cout <<" Usage: display_image ImageToLoadAndDisplay" << endl;
		return -1;
    }

	int rescale_factor = 2;

    IplImage *image;
	IplImage *resize;

	//Read in Image
    image = cvLoadImage(argv[1], CV_LOAD_IMAGE_COLOR);
	
	size.width = image->width*50;
	size.height = image->height*50;
	resize = cvCreateImage(size, image->depth, image->nChannels);

	cvResize(image,resize,CV_INTER_AREA);
	
	//Show all the images
    namedWindow( "original", CV_WINDOW_AUTOSIZE );
    namedWindow( "shifted", CV_WINDOW_AUTOSIZE );
    cvShowImage( "original", image);
    cvShowImage( "shifted",	resize);
    waitKey(0);

	//Release memory
	cvReleaseImage(&image);
	cvReleaseImage(&resize);
    return 0;	
}
