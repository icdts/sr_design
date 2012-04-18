OPENCV_INCS=`pkg-config --cflags --libs opencv`
LI_LOC=../final/

test_load_images: load_images.o test_load_images.o	
	g++ -o test_load_images test_load_images.o load_images.o $(OPENCV_INCS)
test_load_images.o: $(LI_LOC)load_images.h test_load_images.cpp
	g++ -c test_load_images.cpp $(OPENCV_INCS)
load_images.o: $(LI_LOC)load_images.h $(LI_LOC)load_images.cpp
	g++ -c $(LI_LOC)load_images.cpp $(OPENCV_INCS)
clean:
	rm -rf test_load_images *.o
