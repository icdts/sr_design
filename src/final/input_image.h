#ifndef INPUT_IMAGE
#define INPUT_IMAGE

struct input_image {
    std::string name;
    cv::Mat file;
    int vertical_shift;
    int horizontal_shift;
    float prob;
    float score;
};

#endif