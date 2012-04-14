#ifdef _MSC_VER
	//FIXME: Windows includes
#else
	#include <opencv/cv.h>
	#include <opencv2/core/core.hpp>
	#include <opencv2/highgui/highgui.hpp>
#endif
#include <iostream>

using namespace std;
using namespace cv;


//function image=pocs(image,lr_image,ds,sh)
IplImage *pocs(IplImage *img, IplImage *low_res_img, float down_scale, int shift_vertical, int shift_horizontal){
	IplImage *new_img;

	//% shift image and reduce its size by factor of ds
	//sh_image=gen_shift_downsample_image(image,ds,sh);
	new_img = //Mitch's function

	//%right division: A/B = A*B^-1
	//scale=lr_image./sh_image;
	
	//% a=kron(scale,ones(ds))
	//%    expands scale to the output size
	//% shift_image(a,-sh)
	//%   shift a in the opposite direction of sh_image
	//scale=shift_image(kron(scale,ones(ds)),-sh);
	//
	//% multiple every element by scale
	//image=image.*scale;
}
