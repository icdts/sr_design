#include "subpixel_register.h"

using namespace std;
using namespace cv; 

#define height() rows
#define width() cols

/*******************************************************************************

    subpixel_register accepts two image_datas, a downsample int and an int 
    sigma value.  When values are passed in hr_image should be the original 
    image kronned with ds 1 (i.e.: should be the image with each pixel expanded 
    to fill (ds) times the space).  lr_image is the original image being worked on.
    This way the 'gen_shift_downsample'd image has the same size as the 
    original.

    subpixel_register:
        image_data *hr_input:
            A pointer to an image that has been scaled to (ds)x the size of 
            lr_image
        image_data *lr_input:
            A pointer to the original image that hr_input is being compared to.
        int ds:
            The factor by which the pixels are scaled.
        int sigma:
            Some arbitrary value.  I think it's 40.

*******************************************************************************/
float subpixel_register( image_data *hr_input, 
                         image_data *lr_input, 
                         int ds, 
                         int sigma ){
    debug("SubpixelRegister called");
    
    if (sigma < 0)
        sigma = 40;

    /* Iterator for the score array */
    int sid = 0; 

    /* 2*40*40, don't know why */
    float sigma2 = 2 * sigma * sigma; 
            
    Mat lr_image, hr_image;

    /* Create a copy of the matrix contained in the struct */
    lr_input->file.copyTo(lr_image);

    /* Create a copy... */
    hr_input->file.copyTo(hr_image);

    /* Convert to 64F so exp and reduce work correctly */
    lr_image.convertTo( lr_image, CV_64F );
    Mat t_mat;

    /* A (1 x width) array to store column averages */
    Mat reduced = Mat( 1, lr_image.width(), CV_64F );

    int final_score = 0;

    /* Used to save every score */
    float score_array[289]; 

    debug("Shift around");

    /* These loops shift the image in every possible combination from (-8,-8) to
    (8,8) and determines a score based on some code from the matlab 
    implementation, the score is usually a number in the thousands */
    for(int i = -8; i<9; i++){
        for(int j = -8; j<9; j++){
            if ( ( hr_image.width()) / 4  !=   lr_image.width() ){
                cout <<" Troubles: the two images are not the right size)\n";
                cout <<"hr: "<< hr_image.width()<<" lr: "<< lr_image.width()
                     <<endl;
                return 1;
            }
            
            Mat t = genShiftDownsampleMat( hr_image, i, j, ds );
        debug("Converting to floats");

            /* Convert to a 64F matrix and store in t_mat */
            t.convertTo(t_mat, CV_64F);

        debug("Math start");
            t_mat = t_mat - lr_image;                 
            pow(t_mat, 2, t_mat);
            /* sigma2 = 2*sigma*sigma */                     
            t_mat = -( t_mat / sigma2 ); 
        debug("Exponentiating");
            exp( t_mat, t_mat);
            reduce(t_mat, reduced, 0, CV_REDUCE_AVG);
            reduced = reduced * lr_image.height();
        debug("Getting score");
            final_score = mean(reduced)[0];
            final_score = final_score * lr_image.width();

            /* Store the resulting score in the array */
            score_array[sid] =  final_score; 
            sid += 1;

            /* if the new score is better put a new best in the passed image
            since it was passed by reference this changes the image_data struct
            in the above function as well */
            if (final_score > lr_input->score){
                lr_input->score = final_score;
                lr_input->horizontal_shift = i; 
                lr_input->vertical_shift = j;
            }

            /* Reset score */
            final_score = 0; 
        }
    }

    debug("determine probability by dividing best score by sum of scores");
    
    /* Determine probability by dividing best score by sum of scores */
    for (int i=0; i<289; i++){
        lr_input->prob += score_array[i];
    }

    lr_input->prob = lr_input->score / lr_input->prob;

    return lr_input->prob;
}
