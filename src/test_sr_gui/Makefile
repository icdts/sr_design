OPENCV_INCS=`pkg-config --cflags --libs opencv`
FFTW_INCS=`pkg-config --cflags --libs fftw3`
IM_LOC=../final/

all: sr_gui
sr_gui: sr_gui.cpp register_image.o image_manipulations.o gen_window.o load_images.o sr_one_step.o debug.o subpixel_register.o $(IM_LOC)register_image.h $(IM_LOC)load_images.h $(IM_LOC)subpixel_register.h
	g++ -o sr_gui sr_gui.cpp register_image.o image_manipulations.o gen_window.o load_images.o sr_one_step.o debug.o subpixel_register.o $(OPENCV_INCS) $(FFTW_INCS)

sr_one_step.o: $(IM_LOC)sr_one_step.h $(IM_LOC)sr_one_step.cpp
	g++ -c $(IM_LOC)sr_one_step.cpp $(OPENCV_INCS)

subpixel_register.o: $(IM_LOC)subpixel_register.h $(IM_LOC)subpixel_register.cpp $(IM_LOC)image_manipulations.h $(IM_LOC)subpixel_register.h $(IM_LOC)debug.h
	g++ -c $(IM_LOC)subpixel_register.cpp $(OPENCV_INCS)

load_images.o: $(IM_LOC)load_images.h $(IM_LOC)load_images.cpp $(IM_LOC)image_data.h $(IM_LOC)debug.h
	g++ -c $(IM_LOC)load_images.cpp $(OPENCV_INCS)

register_image.o: $(IM_LOC)register_image.cpp $(IM_LOC)register_image.h $(IM_LOC)image_manipulations.h $(IM_LOC)image_data.h $(IM_LOC)debug.h
	g++ -c $(IM_LOC)register_image.cpp $(OPENCV_INCS) $(FFTW_INCS)

gen_window.o: $(IM_LOC)gen_window.cpp $(IM_LOC)gen_window.h $(IM_LOC)debug.h
	g++ -c $(IM_LOC)gen_window.cpp $(OPENCV_INCS)

image_manipulations.o: $(IM_LOC)image_manipulations.cpp $(IM_LOC)image_manipulations.h $(IM_LOC)debug.h
	g++ -c $(IM_LOC)image_manipulations.cpp $(OPENCV_INCS)

debug.o: $(IM_LOC)debug.cpp $(IM_LOC)debug.h
	g++ -c $(IM_LOC)debug.cpp $(OPENCV_INCS)

clean:
	rm -f *.o sr_gui