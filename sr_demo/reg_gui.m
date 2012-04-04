function varargout = reg_gui(varargin)
% REG_GUI M-file for reg_gui.fig
%      REG_GUI, by itself, creates a new REG_GUI or raises the existing
%      singleton*.
%
%      H = REG_GUI returns the handle to a new REG_GUI or the handle to
%      the existing singleton*.
%
%      REG_GUI('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in REG_GUI.M with the given input arguments.
%
%      REG_GUI('Property','Value',...) creates a new REG_GUI or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before reg_gui_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to reg_gui_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help reg_gui

% Last Modified by GUIDE v2.5 30-Apr-2009 12:09:27

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @reg_gui_OpeningFcn, ...
                   'gui_OutputFcn',  @reg_gui_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before reg_gui is made visible.
function reg_gui_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to reg_gui (see VARARGIN)

% Choose default command line output for reg_gui
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes reg_gui wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = reg_gui_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on slider movement.
function h_slider_Callback(hObject, eventdata, handles)
% hObject    handle to h_slider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'Value') returns position of slider
%        get(hObject,'Min') and get(hObject,'Max') to determine range of slider
update_img(handles);

% --- Executes during object creation, after setting all properties.
function h_slider_CreateFcn(hObject, eventdata, handles)
% hObject    handle to h_slider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: slider controls usually have a light gray background.
if isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor',[.9 .9 .9]);
end

%----
function update_img(handles)
load
w=gen_window(size(im1));
im1=im1.*w;
im2=im2.*w;
sh2=round(get(handles.h_slider,'value'));
sh1=round(get(handles.v_slider,'value'));

im2=shift_image_rot(im2,[sh1,sh2]);
axes(handles.axes2);
imshow(im2);
title(sum(sum(im1.*im1))+sum(sum(im2.*im2))-2*sum(sum((im1.*im2))));
axes(handles.axes3);
imshow(im1-im2);
title(sum(sum(abs(im1-im2).^2)));
axes(handles.axes1);
xlabel(sh2);
ylabel(sh1);

% --- Executes on slider movement.
function v_slider_Callback(hObject, eventdata, handles)
% hObject    handle to v_slider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'Value') returns position of slider
%        get(hObject,'Min') and get(hObject,'Max') to determine range of slider
update_img(handles)

% --- Executes during object creation, after setting all properties.
function v_slider_CreateFcn(hObject, eventdata, handles)
% hObject    handle to v_slider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: slider controls usually have a light gray background.
if isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor',[.9 .9 .9]);
end


% --- Executes on button press in pushbutton1.
function pushbutton1_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
load
axes(handles.axes1);
imshow(im1);
axes(handles.axes2);
imshow(im2);
axes(handles.axes3);
imshow(im1-im2);

