#include "sr_one_step.h"

using namespace std;
using namespace cv;

/******************************************************************************

    sr_one_step() takess an image_data and an image_data vector, representing 
    the low-resolution images. It calls subpixel_register() to calculate shifts 
    and probabilities for each file, and uses these shifts with shiftMat() to 
    combine the low-resolution images into one high-resolution output image.
    Output: Mat with 4x the number of rows and columns as the low-res input.

    sr_one_step():
        image_data* src:
            The low-res image struct used as a reference for the shifts in 
            subpixel_register()
        vector <image_data> *input:
            The vector of low-res input images; used in subpixel_register() to 
            determine shifts, and then each image with a positive probability 
            is shifted with shiftMat() and combined together to form the high-
            res output Mat.
    

******************************************************************************/
Mat sr_one_step(image_data *src, vector <image_data> *input){
    debug("sr_one_step called");
    Mat kron_image;
    resize(src->file, src->file, Size(), 4, 4, CV_INTER_AREA);
    //calls subpixel_register to assign shifts and probabilities for each 
    //input image
    for (int tid = 1; tid < input->size(); tid++){
        (*input)[tid].prob = subpixel_register(src, &((*input)[tid]), 4, -1);
    }
    Mat image = (*input)[0].file;
    Mat sh_image(src->file.rows, src->file.cols, CV_32FC1, Scalar(0));
    resize(image, image, Size(), 4, 4, CV_INTER_AREA);
    for (int tid = 0; tid < input->size(); tid++){
        //Resize image by a scale of 4, which was done with a kron product in 
        //matlab
        resize((*input)[tid].file, kron_image, Size(), 4, 4, CV_INTER_AREA);
        //if the probability for the input image is high, add the shift to 
        //the output
        //int counter = 0;
        if ((*input)[tid].prob > 0.0){
            //shift the enlarged input image by its values determined from 
            //subpixel_register
            sh_image=shiftMat(kron_image, -(*input)[tid].vertical_shift, 
            -(*input)[tid].horizontal_shift);
            debug("After shiftMat in sr_one_step");
            image = (sh_image + image)/2;
           // counter ++;
        }
        //if counter is greater than 0, divide the image matrix by counter 
        //to get the average of all added images
        /*if(counter > 0)
            image = image / counter;*/
    }
    //shrink image by a scale of 1/4
    resize(image, image, Size(0,0), 0.25, 0.25, CV_INTER_AREA);
    return image;
}
