//sr_gui [-c][LOAD_DIRECTORY][OUTPUT_DIRECTORY]
#include <iostream>
#include <vector>
#include "../final/load_images.h"
#include "../final/register_image.h"
#include "../final/sr_one_step.h"
#include <opencv/cv.h>
#include <opencv2/core/core.hpp>

/*
	Steps for making the matlab program do what we want it to:
	1.	File -> load images
	2.	Register Image
	3.	SR(BW/Color)
*/

using namespace std;
using namespace cv;

int main(int argc, char const *argv[]){
	int loadDirectoryIndex = 0; //Index of argv where the load directory is located
	int saveDirectoryIndex = 0; //Index of argv where the save directory is located

	bool color = false;		//Initialize the color mode to black and white.
	

	//	Handle input
	
	if(argv==1||argv>4){
		cout<<"Invalid arguments.\n\nExecution should be of the format:\n s"+
		"r_gui ([-c])[LOAD_DIRECTORY]([OUTPUT_DIRECTORY])\n"<<endl;
		return -1;
	}

	if(argv==2){
		loadDirectoryIndex = 1;
	} else if(argv == 3){
		if(argv[1] == "-c"){
			color = true;
			loadDirectoryIndex = 2;
		} else{
			loadDirectoryIndex = 1;
			saveDirectoryIndex = 2;
		}
	} else if(argv == 4){
		if(argv[1] == "-c"){
			color = true;
			loadDirectoryIndex = 2;
			saveDirectoryIndex = 3;
		} else{
			cout<<"Invalid arguments.\n\nExecution should be of the format:\n s"+
			"r_gui ([-c])[LOAD_DIRECTORY]([OUTPUT_DIRECTORY])\n"<<endl;
			return -1;
		}
	}
	// #1

	cout<<"Loading Images"<<endl;
	std::vector<input_image> images = load_images(argv[loadDirectoryIndex]);

	// #2
	
	cout<<"Registering Images"<<endl;
	for(int i = 1; i < 1+(int)images.size; i++){
		register_image(images[0], images[i]);
	}


	// #3

	/*% --- Executes on button press in SRBWButton.
	function SRBWButton_Callback(hObject, eventdata, handles)
	% hObject    handle to SRBWButton (see GCBO)
	% eventdata  reserved - to be defined in a future version of MATLAB
	% handles    structure with handles and user data (see GUIDATA)
	[p1,p2]=get_clip_range(handles);
	for id=1:length(handles.images)
	    t2{id}=handles.images{id}(p1(2):p2(2),p1(1):p2(1),1)*255;
	end
	h=waitbar(0,'Running super-resolution for red component...');
	image=sr_bw(t2,h,0,1)/255;
	delete(h);
	figure; imshow(image); title('super resolution image');

	% --- Executes on button press in SRColorButton.
	function SRColorButton_Callback(hObject, eventdata, handles)
	% hObject    handle to SRColorButton (see GCBO)
	% eventdata  reserved - to be defined in a future version of MATLAB
	% handles    structure with handles and user data (see GUIDATA)
	[p1,p2]=get_clip_range(handles);
	h=waitbar(0,'Running super-resolution for all three components...');
	for color=1:3
	    for id=1:length(handles.images)
	        t2{id}=handles.images{id}(p1(2):p2(2),p1(1):p2(1),color)*255;
	    end
	    image{color}=sr_bw(t2,h,(color-1)/3,1/3)/255;
	end
	delete(h);
	tmpimg=zeros([size(image{color}) 3]);
	for color=1:3
	    tmpimg(:,:,color)=image{color};
	end
	figure; imshow(tmpimg); title('super resolution image');
*/
	cout<<"Starting Super-resolution Algorigthm"<<endl;

	Mat final = sr_one_step(images[0].file, images);

	/*if(color){

	} else{
		Mat final = sr_bw(/* Whatever arguments this takes.  I assume 'images' 
							 will be somehow involved.');
	}*/

	if(saveDirectoryIndex){
		imwrite(argv[saveDirectoryIndex]+"output.jpg", final);
	} else{
		imwrite(output.jpg, final);
	}

	imShow("Result", final);
	waitkey(0);

	return 0;
}