#include "load_images.h"

using namespace std;
using namespace cv;


/******************************************************************************

    load_images accepts the string of a directory and a pointer to a vector of 
    image_datas as parameters.  It then loads all of the images of that 
    directory into a Mat element in  an image_datas object. The image_data is 
    also loaded with the name of the image and is then added to a vector with 
    all of the other images from the directory.  This vector is then returned.

    load_images:
        string dir:
            The directory within which all of the input images are contained.
        vector<image_data> * files:
            A pointer to a vector of image_datas. The variable files is passed
            by reference whenever the function is called, so each time the 
            funtion executes, the loaded images are stored in the vector.

******************************************************************************/
void load_images( string dir, vector<image_data> * files ){
    debug("Load Images called");
    Mat tmp;
    DIR *dp;

    struct dirent *dirp;
    if((dp = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        exit(errno);
    }

    while ((dirp = readdir(dp)) != NULL) {
        debug(dirp->d_name);
        if( string(dirp->d_name) != "." && string(dirp->d_name) != ".."){
            image_data f;
            f.name = dir + string(dirp->d_name);

            tmp = imread(f.name.c_str(), CV_LOAD_IMAGE_COLOR);
            tmp.convertTo(f.file,CV_32F);

            f.name = string(dirp->d_name);

            files->push_back(f);
        }
    }

    closedir(dp);
}