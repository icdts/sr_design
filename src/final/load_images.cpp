#include "load_images.h"

using namespace std;
using namespace cv;


/******************************************************************************

	load_images accepts the string of a directory as a parameter.  It then 
	loads all of the images of that directory into a Mat element in  an 
	input_images object. The input_image is also loaded with the name of the 
	image and is then added to a vector with all of the other images from the
	directory.  This vector is then returned.

	load_images:
		string dir:
			The directory within which all of the input images are contained.

******************************************************************************/
vector<input_image> load_images( string dir ){
	vector<input_image> files;
	Mat tmp;
	DIR *dp;

	struct dirent *dirp;
	if((dp = opendir(dir.c_str())) == NULL) {
		cout << "Error(" << errno << ") opening " << dir << endl;
		exit(errno);
	}

	while ((dirp = readdir(dp)) != NULL) {
		if( string(dirp->d_name) != "." && string(dirp->d_name) != ".."){
			input_image f;
			f.name = dir + string(dirp->d_name);

			tmp = imread(f.name.c_str(), CV_LOAD_IMAGE_COLOR);
			tmp.convertTo(f.file,CV_32F);

			f.name = string(dirp->d_name);

			files.push_back(f);
		}
	}

	closedir(dp);

	return files;
}