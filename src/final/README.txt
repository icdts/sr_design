================================================================================
Super Resolve
Written by Casey Henson, Grant Schwartz, Mitch Morton, and Robert Nolan
================================================================================

This program generates a super-resolved image of several lower resolution
images.

The following is the pattern for passing arguments when calling the program:

    sr_gui [LOAD_DIRECTORY] ([OUTPUT_DIRECTORY])

Where LOAD_DIRECTORY is a directory that only the identically-sized *.jpg files 
to be resolved and OUTPUT_DIRECTORY is an optional argument to specify a 
directory to save the super-resolved image. If no OUTPUT_DIRECTORY is specified,
the resulting image will instead be saved in the same directory as the program. 

Files and functions:
================================================================================
Main file: super_resolve.cpp:
================================================================================
This file calls other functions that accomplish the following tasks:
	1.  Load images
    2.  Register images
    3.  Super Resolution
    4.  Convert Mat from float to uchar
    5.  Save result
    6.  Output result to screen

================================================================================
subpixel_register.cpp:
================================================================================

subpixel_register:
	subpixel_register accepts two image_datas, a downsample int and an int 
    sigma value.  When values are passed in hr_image should be the original 
    image kronned with ds 1 (i.e.: should be the image with each pixel expanded 
    to fill (ds) times the space).  lr_image is the original image being worked 
    on. This way the 'gen_shift_downsample'd image has the same size as the 
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

================================================================================
sr_one_step.cpp
================================================================================

sr_one_step:
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

================================================================================
register_image.cpp
================================================================================

phase_correlation:
	phase_correlation accepts two Mat variables as parameters.  This program
    takes two images of equal size and uses the FFTW library to assess the two
    images' similarity to one another.

    phase_correlation:
        Mat m_ref:
            Reference image.  This is the first image passed to the function.
        Mat m_tpl:
            Template image.  This is the second image passed to the function, to
            which the first image is compared.

    Full disclosure:
        We obtained this code from nashruddin.com after many failed attempts to
        write this with OpenCV's built-in and DFT function.

        http://nashruddin.com/phase-correlation-function-in-opencv.html

register_image:
    register_image accepts two image_data variables as parameters.  It then
    uses calls to phase_correlation to determine how the second image should be
    shifted in order for the two images to "line up".  It then performs the
    recommended shift.

    register_image:
        image_data * input1:
            The base image to which input2 is compared and shifted.
        image_data * input2:
            The image that is shifted according to its correlation with the
            first image.

================================================================================
load_images.cpp
================================================================================

load_images:
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

================================================================================
image_manipulations.cpp
================================================================================

shiftMat:
    shiftMat accepts a CV_32F Mat and shifts it according to the integer 
    parameters that are also passed to it.  It fills in the gaps left by the
    shift with the last line of pixels in the shifted image.  It then returns 
    the image as a Mat.


    shiftMat:
        Mat input:
            The matrix to be shifted.
        int vertical:
            An integer value that determines how far the image needs to be
            shifted vertically.  Negative shifts upward, positive shifts 
            downward.
        int horizontal:
            An integer value that determines how far the image needs to be
            shifted horizontally.  Negative shifts left, positive shifts right.

shift_mat_rot:
	shiftMatRot accepts a CV_32F Mat and shifts it according to the integer 
    parameters that are also passed to it.  It fills in the gaps left by the
    shift with the pixels that were shifted off of the other side of the image.
    It then returns the image as a Mat.


    shiftMatRot:
        Mat input:
            The matrix to be shifted.
        int vertical:
            An integer value that determines how far the image needs to be
            shifted vertically.  Negative shifts upward, positive shifts 
            downward.
        int horizontal:
            An integer value that determines how far the image needs to be
            shifted horizontally.  Negative shifts left, positive shifts right.

copyRegion:
	copyRegion accepts a pointer to a source matrix and a destination matrix as
    parameters along with integer values that specify a region in each image.  
    The function copys the specified region in the src Mat into the specified
    region of the dest Mat.


    copyRegion:
        Mat &src:
            Reference to a source matrix.
        Mat &dest:
            Reference to a destination matrix.
        int x1:
            The x coordinate of the left side of the selected region from src.
        int y1:
            The y coordinate of the top side of the selected region from src.
        int w1:
            The width of the region selected from src.
        int h1:
            The height of the region selected from src.
        int x2:
            The x coordinate of the left side of the selected region from dest.
        int y2:
            The y coordinate of the top side of the selected region from dest.
        int w2:
            The width of the region selected from dest.
        int h2:
            The height of the region selected from dest.

genShiftDownsample:	
	genShiftDownsampleMat shfts the src matrix as directed by up_down and 
    left_right and then shrinks the image by a ratio of 1/downsample_ratio. It
    then returns the resulting Mat.

    genShiftDownsampleMat:
        Mat src:
            This is the source Mat to be shifted and downsampled.
        int up_down:    
            An integer value that determines how far the image needs to be
            shifted vertically.  Negative shifts upward, positive shifts 
            downward.
        int left_right:
            An integer value that determines how far the image needs to be
            shifted horizontally.  Negative shifts left, positive shifts right.
        int downsample_ratio:
            The inverse of the desired scale of the image.

================================================================================
gen_window.cpp
================================================================================

gen_window:
    gen_window generates a matrix of size [height x width] with [channels] 
    channels.  The generated matrix fades linearally from 0 (black) to 1 
    (white) across hpercent percent of the top and bottom borders and wpercent
    percent of the left and right borders.  This is creates a kernel that we
    can use to detect edges without running into problems with the edges of the
    matrix itself.


    gen_window:
        int height:
            The height of the desired output Mat.
        int width:
            The width of the desired output Mat.
        float hpercent:
            The percent of the top and bottom images to fade to black on those 
            edges of the image.
        float wpercent:
            The percent of the left and right images to fade to black on those 
            edges of the image.

================================================================================
debug.cpp
================================================================================

debug:
    debug simply outputs to the console whatever string is passed to it.
    
    debug:
        string str:
            This is a string variable to be output to the console.


printMatrix:
	    printMatrix accepts a single Mat variable as a parameter.  It then breaks
    the matrix up into its individual panels and outputs its numeric values in a
    readable format.

    printMatrix:
        Mat m:
            m is a input matrix of type CV_32F
