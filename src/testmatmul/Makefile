IM_LOC=../final/
OPENCV_INCS=-I/usr/include/opencv  -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_legacy -lopencv_flann

testmatmul: testmatmul.o gen_window.o
	g++ -o testmatmul testmatmul.o gen_window.o $(OPENCV_INCS)
testmatmul.o: $(IM_LOC)gen_window.h testmatmul.cpp
	g++ -c testmatmul.cpp $(OPENCV_INCS)
gen_window.o: $(IM_LOC)gen_window.h $(IM_LOC)gen_window.cpp
	g++ -c $(IM_LOC)gen_window.cpp $(OPENCV_INCS)
clean:
	rm -rf testmatmul *.o