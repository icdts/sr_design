OPENCV_INCS=`pkg-config --cflags --libs opencv`
IM_LOC=../final/

test_manips: test_manips.o image_manipulations.o
	g++ -o test_manips test_manips.o image_manipulations.o $(OPENCV_INCS)
test_manips.o: $(IM_LOC)image_manipulations.h test_manips.cpp
	g++ -c test_manips.cpp $(OPENCV_INCS)
image_manipulations.o: $(IM_LOC)image_manipulations.h $(IM_LOC)image_manipulations.cpp
	g++ -c $(IM_LOC)image_manipulations.cpp $(OPENCV_INCS)
clean:
	rm -rf test_manips *.o
