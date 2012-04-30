OPENCV_INCS=`pkg-config --cflags --libs opencv`
FFTW_INCS=`pkg-config --cflags --libs fftw3`
IM_LOC=../final/

all: sr_gui
sr_gui: sr_gui.cpp register_image.o image_manipulations.o gen_window.o
	g++ -o sr_gui sr_gui.cpp register_image.o image_manipulations.o gen_window.o $(OPENCV_INCS) $(FFTW_INCS)

register_image.o: $(IM_LOC)register_image.cpp $(IM_LOC)register_image.h $(IM_LOC)image_manipulations.h
	g++ -c $(IM_LOC)register_image.cpp $(OPENCV_INCS) $(FFTW_INCS)

image_manipulations.o: $(IM_LOC)image_manipulations.cpp $(IM_LOC)image_manipulations.h
	g++ -c $(IM_LOC)image_manipulations.cpp $(OPENCV_INCS)

gen_window.o: $(IM_LOC)gen_window.cpp $(IM_LOC)gen_window.h
	g++ -c $(IM_LOC)gen_window.cpp $(OPENCV_INCS)

clean:
	rm -f *.o sr_gui