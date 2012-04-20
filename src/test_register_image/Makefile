OPENCV_INCS=`pkg-config --cflags --libs opencv`
IM_LOC=../final/

test: test.o register_image.o
	g++ -o test test.o register_image.o $(OPENCV_INCS)
test.o: register_image.h
	g++ -c test.cpp $(OPENCV_INCS)
register_image.o: register_image.h
	g++ -c register_image.cpp $(OPENCV_INCS)
clean:
	rm -rf test_register_image *.o
