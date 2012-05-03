OPENCV_INCS=`pkg-config --cflags --libs opencv`
FFTW_INCS=`pkg-config --cflags --libs fftw3`

all: super_resolve
super_resolve: super_resolve.cpp register_image.o image_manipulations.o gen_window.o load_images.o sr_one_step.o debug.o subpixel_register.o register_image.h load_images.h subpixel_register.h
	g++ -o super_resolve super_resolve.cpp register_image.o image_manipulations.o gen_window.o load_images.o sr_one_step.o debug.o subpixel_register.o $(OPENCV_INCS) $(FFTW_INCS)

sr_one_step.o: sr_one_step.h sr_one_step.cpp
	g++ -c sr_one_step.cpp $(OPENCV_INCS)

subpixel_register.o: subpixel_register.h subpixel_register.cpp image_manipulations.h subpixel_register.h debug.h
	g++ -c subpixel_register.cpp $(OPENCV_INCS)

load_images.o: load_images.h load_images.cpp image_data.h debug.h
	g++ -c load_images.cpp $(OPENCV_INCS)

register_image.o: register_image.cpp register_image.h image_manipulations.h image_data.h debug.h
	g++ -c register_image.cpp $(OPENCV_INCS) $(FFTW_INCS)

gen_window.o: gen_window.cpp gen_window.h debug.h
	g++ -c gen_window.cpp $(OPENCV_INCS)

image_manipulations.o: image_manipulations.cpp image_manipulations.h debug.h
	g++ -c image_manipulations.cpp $(OPENCV_INCS)

debug.o: debug.cpp debug.h
	g++ -c debug.cpp $(OPENCV_INCS)

clean:
	rm -f *.o super_resolve
