OPENCV_INCS=`pkg-config --cflags --libs opencv`
IM_LOC=../final/

test_pocs: test_pocs.o image_manipulations.o
	g++ -o test_pocs test_pocs.o image_manipulations.o $(OPENCV_INCS)
test_pocs.o: $(IM_LOC)image_manipulations.h test_pocs.cpp
	g++ -c test_pocs.cpp $(OPENCV_INCS)
image_manipulations.o: $(IM_LOC)image_manipulations.h $(IM_LOC)image_manipulations.cpp
	g++ -c $(IM_LOC)image_manipulations.cpp $(OPENCV_INCS)
clean:
	rm -rf test_pocs *.o
