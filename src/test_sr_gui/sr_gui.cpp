//sr_gui [-c][LOAD_DIRECTORY][OUTPUT_DIRECTORY]
#include "../finals"


int main(int argc, char const *argv[])
{

	/*
		Steps for making the matlab program do what we want it to:
		1.	File -> load images
		2.	Register Image
		3.	Select Area
		4.	Zoom
		5.	SR(BW/Color)
	*/

	// #1


	/*function LoadMenuItem_Callback(hObject, eventdata, handles)
	% hObject    handle to LoadMenuItem (see GCBO)
	% eventdata  reserved - to be defined in a future version of MATLAB
	% handles    structure with handles and user data (see GUIDATA)
	folder = uigetdir;
	files=dir(sprintf('%s/*.jpg',folder));
	tmp=dir(sprintf('%s/*.JPG',folder));
	files(length(files)+1:length(files)+length(tmp))=tmp;

	handles.images=[];
	h=waitbar(0,'Loading images...');
	for id=1:length(files)
	    handles.images{id}=double(imread(sprintf('%s/%s',folder,files(id).name)))/255;
	    waitbar(id/length(files),h);
	end
	delete(h);
	 
	axes(handles.ImageAxes);
	hold off;
	imshow(handles.images{1});
	hold on;
	set(handles.FrameSlider,'min',1);
	set(handles.FrameSlider,'max',length(handles.images));
	set(handles.FrameSlider,'value',1);
	set(handles.frame_title,'string','Frame 1');
	title('Frame 1');
	guidata(hObject,handles);*/


	// #2


	/*function RegisterImageButton_Callback(hObject, eventdata, handles)
	% hObject    handle to RegisterImageButton (see GCBO)
	% eventdata  reserved - to be defined in a future version of MATLAB
	% handles    structure with handles and user data (see GUIDATA)

	h=waitbar(0,'Registering images...');
	for id=2:length(handles.images)
	    clear timage;

	    timage=register_color_image(double(handles.images{1}), double(handles.images{id}));
	    handles.images{id}=timage;
	    waitbar(id/length(handles.images),h);
	end
	delete(h);
	guidata(hObject,handles);
	FrameSlider_Callback(hObject, [], handles)
	*/


	// #3

	/*% --- Executes on mouse motion over figure - except title and menu.
	function figure1_WindowButtonMotionFcn(hObject, eventdata, handles)
	% hObject    handle to figure1 (see GCBO)
	% eventdata  reserved - to be defined in a future version of MATLAB
	% handles    structure with handles and user data (see GUIDATA)
	pos=get(handles.ImageAxes,'currentpoint');
	pos=pos(1,1:2);
	if inrange(pos,handles.ImageAxes)
	    if isfield(handles,'mouse_pressed')
	        if handles.mouse_pressed
	           if isfield(handles,'last_box')
	                if ishandle(handles.last_box)
	                    delete(handles.last_box);
	                end
	           end
	           axes(handles.ImageAxes);
	           handles.last_box=drawbox(handles.start_pos,pos); 
	           guidata(hObject,handles);
	        end
	    end
	    
	end
	*/


	// #4

	/*% --- Executes on selection change in BilinearProjectionPopup.
	function BilinearProjectionPopup_Callback(hObject, eventdata, handles)
	% hObject    handle to BilinearProjectionPopup (see GCBO)
	% eventdata  reserved - to be defined in a future version of MATLAB
	% handles    structure with handles and user data (see GUIDATA)

	% Hints: contents = get(hObject,'String') returns BilinearProjectionPopup contents as cell array
	%        contents{get(hObject,'Value')} returns selected item from BilinearProjectionPopup
	p1(1)=min([handles.start_pos(1),handles.end_pos(1)]);
	p1(2)=min([handles.start_pos(2),handles.end_pos(2)]);
	p2(1)=max([handles.start_pos(1),handles.end_pos(1)]);
	p2(2)=max([handles.start_pos(2),handles.end_pos(2)]);
	p1=round(p1); p2=round(p2);
	imobj=findobj(handles.ImageAxes,'type','image');
	cdata=get(imobj,'cdata');
	fid=round(get(handles.FrameSlider,'value'));
	cdata=handles.images{fid};
	imclip=cdata(p1(2):p2(2),p1(1):p2(1),:);

	contents=get(hObject,'String');
	figure; 
	switch contents{get(hObject,'value')}
	    case 'zoom 1x'
	        imshow(imclip);
	        title('zoom 1x');
	    case 'zoom 2x'
	        imclip=bilinear_interp(double(imclip));
	        imshow(imclip);         
	        title('zoom 2x');
	    case 'zoom 4x'
	        imclip=bilinear_interp(double(imclip));
	        imclip=bilinear_interp(imclip);
	        imshow(imclip);
	        title('zoom 4x');
	end*/


	// #5

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
	return 0;
}