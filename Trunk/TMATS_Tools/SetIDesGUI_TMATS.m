function varargout = SetIDesGUI_TMATS(varargin)
% SETIDESGUI_TMATS MATLAB code for SetIDesGUI_TMATS.fig
%      SETIDESGUI_TMATS, by itself, creates a new SETIDESGUI_TMATS or raises the existing
%      singleton*.
%
%      H = SETIDESGUI_TMATS returns the handle to a new SETIDESGUI_TMATS or the handle to
%      the existing singleton*.
%
%      SETIDESGUI_TMATS('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in SETIDESGUI_TMATS.M with the given input arguments.
%
%      SETIDESGUI_TMATS('Property','Value',...) creates a new SETIDESGUI_TMATS or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before SetIDesGUI_TMATS_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to SetIDesGUI_TMATS_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help SetIDesGUI_TMATS

% Last Modified by GUIDE v2.5 15-Mar-2016 14:57:52

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @SetIDesGUI_TMATS_OpeningFcn, ...
                   'gui_OutputFcn',  @SetIDesGUI_TMATS_OutputFcn, ...
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


% --- Executes just before SetIDesGUI_TMATS is made visible.
function SetIDesGUI_TMATS_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to SetIDesGUI_TMATS (see VARARGIN)

% Choose default command line output for SetIDesGUI_TMATS
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes SetIDesGUI_TMATS wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = SetIDesGUI_TMATS_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on button press in btn_set.
function btn_set_Callback(hObject, eventdata, handles)
% hObject    handle to btn_set (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
PopVal = get(handles.popupmenu1,'Value');
IDesVal = PopVal - 1;
SetIDesFun_TMATS(gcs, IDesVal);

% --- Executes on button press in btn_cancel.
function btn_cancel_Callback(hObject, eventdata, handles)
% hObject    handle to btn_cancel (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
close(SetIDesGUI_TMATS)



% --- Executes on selection change in popupmenu1.
function popupmenu1_Callback(hObject, eventdata, handles)
% hObject    handle to popupmenu1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns popupmenu1 contents as cell array
%        contents{get(hObject,'Value')} returns selected item from popupmenu1


% --- Executes during object creation, after setting all properties.
function popupmenu1_CreateFcn(hObject, eventdata, handles)
% hObject    handle to popupmenu1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
