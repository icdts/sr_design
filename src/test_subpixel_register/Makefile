#OPENCV_INCS=-I/usr/include/opencv  -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_legacy -lopencv_flann
OPENCV_INCS=`pkg-config --cflags --libs opencv`
IM_LOC=../final/

test_subpixel: subpixel_register.o image_manipulations.o
	g++ -o test_subpixel subpixel_register.o image_manipulations.o $(OPENCV_INCS)
subpixel_register.o: $(IM_LOC)image_manipulations.h subpixel_register.cpp
	g++ -c subpixel_register.cpp $(OPENCV_INCS)
image_manipulations.o: $(IM_LOC)image_manipulations.h $(IM_LOC)image_manipulations.cpp
	g++ -c $(IM_LOC)image_manipulations.cpp $(OPENCV_INCS)
clean:
	rm -rf test_subpixel *.o
