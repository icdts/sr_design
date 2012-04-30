#include <iostream>
#include <vector>
#include "load_images.h"
#include "register_image.h"
#include "sr_one_step.h"
#include <opencv/cv.h>
#include <opencv2/core/core.hpp>

using namespace std;
using namespace cv;

/******************************************************************************
	
	sr_gui [LOAD_DIRECTORY] ([OUTPUT_DIRECTORY])

	Steps for making the matlab program do what we want it to:
	
	1.	Load images
	2.	Register images
	3.	Super Resolution
	4. 	Convert Mat from float to uchar
	5. 	Save result
	6. 	Output result to screen

******************************************************************************/

int main(int argc, char const *argv[]){
	/* Index of argv where the load directory is located */
	int loadDirectoryIndex = 0; 
	
	/* Index of argv where the save directory is located */
	int saveDirectoryIndex = 0; 

	/* Initialize the color mode to black and white. */
	bool color = false;	
	

	/* Handle input */
	if(argc==1||argc>3){
		cout<< "Invalid arguments.\n\nExecution should be of the format:\n sr_"
			<< "gui [LOAD_DIRECTORY] ([OUTPUT_DIRECTORY])\n"
			<< endl;
		return -1;
	}
	if(argc==2){
		loadDirectoryIndex = 1;
	} else if(argc == 3){
		loadDirectoryIndex = 1;
		saveDirectoryIndex = 2;
	}


	/* 1. Load images */
	cout<<"Loading Images"<<endl;
	std::vector<input_image> images;
	load_images(argv[loadDirectoryIndex],&images);

	/* 2. Register images */
	cout<<"Registering Images"<<endl;
	for(int i = 1; i < images.size(); i++){
		register_image(&images[0], &images[i]);
	}

	/* 3. Super resolution */
	cout<<"Starting Super-resolution Algorigthm"<<endl;
	Mat final = sr_one_step(&images[0], &images);
	
	/* 4. Convert Mat from float to uchar */
	final.convertTo(final,CV_8U);

	/* 5. Save result */
	string name = "output.jpg";
	if(saveDirectoryIndex){
		name = argv[saveDirectoryIndex] + name;
		imwrite(name, final);
	} else{
		imwrite(name, final);
	}

	/* 6. Output result to screen */
	imshow("Result", final);
	waitKey(0);

	return 0;
}
