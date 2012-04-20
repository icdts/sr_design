#OPENCV_INCS=-I/usr/include/opencv  -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_legacy -lopencv_flann
OPENCV_INCS=`pkg-config --cflags --libs opencv`
IM_LOC=../final/

test_gen_shift: gen_shift_downsample_image.o image_manipulations.o
	g++ -o test_gen_shift gen_shift_downsample_image.o image_manipulations.o $(OPENCV_INCS)
gen_shift_downsample_image.o: $(IM_LOC)image_manipulations.h gen_shift_downsample_image.cpp
	g++ -c gen_shift_downsample_image.cpp $(OPENCV_INCS)
image_manipulations.o: $(IM_LOC)image_manipulations.h $(IM_LOC)image_manipulations.cpp
	g++ -c $(IM_LOC)image_manipulations.cpp $(OPENCV_INCS)
clean:
	rm -rf test_gen_shift *.o
