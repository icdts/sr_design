OPENCV_INCS=`pkg-config --cflags --libs opencv`
IM_LOC=../final/

test: test.cpp
	g++ -o test test.cpp $(OPENCV_INCS)
clean:
	rm -rf test_manips *.o
