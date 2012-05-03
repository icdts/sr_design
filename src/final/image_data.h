#ifndef IMAGE_DATA
#define IMAGE_DATA


/* This structure is for storing images along with some relevant data */
struct image_data {
	/* Name of the file read in */
    std::string name;	

    /* Store the image data */
    cv::Mat file;

    /* Store the shifts made to the image */
    int vertical_shift;
    int horizontal_shift;

    /* 
    	Store the probability of this image given some prior.
    	(used in sr_one_step and subpixel register)
    */
    float prob;

    /* Stores the score for various shifts in subpixel register */
    float score;
};

#endif