#include "register_image.h"

#define DEBUG

using namespace std;
using namespace cv;

void debug(string str){
#ifdef DEBUG
    cerr << "DEBUG: " << str << endl;
#endif
}

int main(int argc, char const *argv[]){
	debug("Reading Images");
	Mat im = imread(argv[1]);
	Mat im2 = imread(argv[2]);

	debug("Calling register_image");
	Mat result = register_image(im,im2);

	debug("Showing output");
	imshow("im",im);
	imshow("im2",im2);
	imshow("result",result);

	waitKey(0);

	return 0;
}
