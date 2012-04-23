OPENCV_INCS=`pkg-config --cflags --libs opencv`
IM_LOC=../final/

test: test.o register_image.o image_manipulations.o gen_window.o debug.o
	g++ -o test test.o register_image.o image_manipulations.o gen_window.o debug.o $(OPENCV_INCS)

test.o: test.cpp register_image.h
	g++ -c test.cpp $(OPENCV_INCS)

register_image.o: register_image.cpp register_image.h $(IM_LOC)image_manipulations.h
	g++ -c register_image.cpp $(OPENCV_INCS)

image_manipulations.o: $(IM_LOC)image_manipulations.cpp $(IM_LOC)image_manipulations.h
	g++ -c $(IM_LOC)image_manipulations.cpp $(OPENCV_INCS)

gen_window.o: $(IM_LOC)gen_window.cpp $(IM_LOC)gen_window.h
	g++ -c $(IM_LOC)gen_window.cpp $(OPENCV_INCS)

debug.o: $(IM_LOC)debug.cpp $(IM_LOC)debug.cpp
	g++ -c $(IM_LOC)debug.cpp
clean:
	rm -rf test_register_image *.o
