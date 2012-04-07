README
--------------------------------------------------------------------------------
Directories:

src
	README.txt
	test_gen_shift_downsample
		gen_shift_downsample_image.cpp
		Makefile
		test.sh
	test_gen_window
		gen_window.cpp
		Makefile
		test.sh
	test_register_image_INC
		register_image_INC.cpp
	test_resize
		test_resize.cpp
		Makefile
		test.sh
	test_subpixel_register
		subpixel_register.cpp
	c_equivalencies_todo.txt


--------------------------------------------------------------------------------
Conversion Progress:

For a list of Matlab files we haven't converted yet, see 
c_equivalencies_todo.txt

Below are descriptions of each of the cpp files in this submission along with
the names of the matlab files their functions implement.  These are currently
broken in to individual files in order to make it easier for us to write and 
test them concurrently and individually.  Our plan is to eventually consolidate
all of these functions and simply call them from our as-of-yet unwritten main 
file

test_resize.cpp:
	
	Contains function(s):
		cropImage
		resizeImage
		rotateImage
		shiftImage
		shiftImageRot
	Implements the Matlab file(s):
		shift_image.m
		shift_image_rot.m
		bilinear_interp.m
		bilinear_interp_bw.m


gen_shift_downsample_image.cpp:
	
	Contains function(s):
		genShiftDownsampleImage
		resizeImage
		shiftImage
		cropImage
	Implements the Matlab file(s):
		gen_shift_downsample_image.m


---The next three files are still in progress and do not run:---

gen_window.cpp:
	
	Contains function(s):
		gen_window
		cropImage
		resizeImage
	Implements the Matlab file(s):
		gen_window.m
	Notes:
		We're currently debugging a seg fault on this file.


register_image_INC.cpp:

	Contains function(s):
		register_Image
	Implements Matlab file(s):
		register_image.m
	Notes:
		The main stumbling block with this program is getting Fast Fourier 
		Transforms to work.

subpixel_register.cpp

	Contains function(s):
		subpixel_register
	Implements Matlab file(s):
		subpixel_register.m
	Notes:
		This file is still being written.

--------------------------------------------------------------------------------
Compilation:

	With the exception of register_image_INC.cpp (which doesn't have its own
	main function) and subpixel_register, we've supplied makefiles and a simple 
	test script for each of these files.  OpenCV 2.3.1 libraries are required to 
	compile.

--------------------------------------------------------------------------------