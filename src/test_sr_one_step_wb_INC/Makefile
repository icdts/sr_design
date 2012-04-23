OPENCV_INCS=-I/usr/include/opencv -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_legacy -lopencv_flann
IM_LOC=../final/
OTHER_LOC=../test_subpixel_register/

test: test_sr_one_step_wb.o load_images.o image_manipulations.o subpixel_register.o
	g++ -o test test_sr_one_step_wb.o image_manipulations.o load_images.o subpixel_register.o $(OPENCV_INCS)

test_sr_one_step_wb.o: test_sr_one_step_wb.cpp $(IM_LOC)load_images.h $(IM_LOC)image_manipulations.h $(OTHER_LOC)subpixel_register.h
	g++ -c test_sr_one_step_wb.cpp $(OPENCV_INCS)

load_images.o: $(IM_LOC)load_images.cpp $(IM_LOC)load_images.h
	g++ -c $(IM_LOC)load_images.cpp $(OPENCV_INCS)

image_manipulations.o: $(IM_LOC)image_manipulations.cpp $(IM_LOC)image_manipulations.h
	g++ -c $(IM_LOC)image_manipulations.cpp $(OPENCV_INCS)

subpixel_register.o: $(OTHER_LOC)subpixel_register.cpp $(OTHER_LOC)subpixel_register.h
	g++ -c $(OTHER_LOC)subpixel_register.cpp $(OPENCV_INCS)
clean:
	rm -rf test_register_image *.o
