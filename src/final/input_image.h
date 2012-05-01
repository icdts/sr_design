#ifndef INPUT_IMAGE
#define INPUT_IMAGE


//TODO: Comment this struct.  What are these variables for?
struct input_image {
    std::string name;
    cv::Mat file;
    int vertical_shift;
    int horizontal_shift;
    float prob;
    float score;
};

#endif