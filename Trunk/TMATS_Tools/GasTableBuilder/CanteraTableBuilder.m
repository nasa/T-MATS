function varargout = CanteraTableBuilder(varargin)
%CANTERATABLEBUILDER M-file for CanteraTableBuilder.fig
%      CANTERATABLEBUILDER, by itself, creates a new CANTERATABLEBUILDER or raises the existing
%      singleton*.
%
%      H = CANTERATABLEBUILDER returns the handle to a new CANTERATABLEBUILDER or the handle to
%      the existing singleton*.
%
%      CANTERATABLEBUILDER('Property','Value',...) creates a new CANTERATABLEBUILDER using the
%      given property value pairs. Unrecognized properties are passed via
%      varargin to CanteraTableBuilder_OpeningFcn.  This calling syntax produces a
%      warning when there is an existing singleton*.
%
%      CANTERATABLEBUILDER('CALLBACK') and CANTERATABLEBUILDER('CALLBACK',hObject,...) call the
%      local function named CALLBACK in CANTERATABLEBUILDER.M with the given input
%      arguments.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help CanteraTableBuilder

% Last Modified by GUIDE v2.5 22-May-2014 13:34:36

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @CanteraTableBuilder_OpeningFcn, ...
                   'gui_OutputFcn',  @CanteraTableBuilder_OutputFcn, ...
                   'gui_LayoutFcn',  [], ...
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


% --- Executes just before CanteraTableBuilder is made visible.
function CanteraTableBuilder_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   unrecognized PropertyName/PropertyValue pairs from the
%            command line (see VARARGIN)

% Choose default command line output for CanteraTableBuilder
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes CanteraTableBuilder wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = CanteraTableBuilder_OutputFcn(hObject, eventdata, handles)
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes when CTB_GUI is resized.
function CTB_GUI_ResizeFcn(hObject, eventdata, handles)
% hObject    handle to CTB_GUI (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on button press in Gen_All.
function Gen_All_Callback(hObject, eventdata, handles)
% hObject    handle to Gen_All (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
DisableButtons(handles);
Streams = StreamSetup(handles);
Fun_All(handles,Streams);
EnableButtons(handles);


% --- Executes on button press in Gen_H2T.
function Gen_H2T_Callback(hObject, eventdata, handles)
% hObject    handle to Gen_H2T (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
DisableButtons(handles);
Streams = StreamSetup(handles);
Fun_H2T(handles,Streams);
EnableButtons(handles);


% --- Executes on button press in Gen_T2H.
function Gen_T2H_Callback(hObject, eventdata, handles)
% hObject    handle to Gen_T2H (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
DisableButtons(handles);
Streams = StreamSetup(handles);
Fun_T2H(handles,Streams);
EnableButtons(handles);

% --- Executes on button press in Gen_SP2T.
function Gen_SP2T_Callback(hObject, eventdata, handles)
% hObject    handle to Gen_SP2T (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
DisableButtons(handles);
Streams = StreamSetup(handles);
Fun_SP2T(handles,Streams);
EnableButtons(handles);

% --- Executes on button press in Gen_PT2S.
function Gen_PT2S_Callback(hObject, eventdata, handles)
% hObject    handle to Gen_PT2S (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
DisableButtons(handles);
Streams = StreamSetup(handles);
Fun_PT2S(handles,Streams);
EnableButtons(handles);


function Import_Phase_File_Callback(hObject, eventdata, handles)
% hObject    handle to Import_Phase_File (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of Import_Phase_File as text
%        str2double(get(hObject,'String')) returns contents of Import_Phase_File as a double


% --- Executes during object creation, after setting all properties.
function Import_Phase_File_CreateFcn(hObject, eventdata, handles)
% hObject    handle to Import_Phase_File (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function Name1_Callback(hObject, eventdata, handles)
% hObject    handle to Name1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of Name1 as text
%        str2double(get(hObject,'String')) returns contents of Name1 as a double


% --- Executes during object creation, after setting all properties.
function Name1_CreateFcn(hObject, eventdata, handles)
% hObject    handle to Name1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function Name2_Callback(hObject, eventdata, handles)
% hObject    handle to Name2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of Name2 as text
%        str2double(get(hObject,'String')) returns contents of Name2 as a double


% --- Executes during object creation, after setting all properties.
function Name2_CreateFcn(hObject, eventdata, handles)
% hObject    handle to Name2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function Species1_Callback(hObject, eventdata, handles)
% hObject    handle to Species1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of Species1 as text
%        str2double(get(hObject,'String')) returns contents of Species1 as a double


% --- Executes during object creation, after setting all properties.
function Species1_CreateFcn(hObject, eventdata, handles)
% hObject    handle to Species1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function Species2_Callback(hObject, eventdata, handles)
% hObject    handle to Species2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of Species2 as text
%        str2double(get(hObject,'String')) returns contents of Species2 as a double


% --- Executes during object creation, after setting all properties.
function Species2_CreateFcn(hObject, eventdata, handles)
% hObject    handle to Species2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function TempR_Callback(hObject, eventdata, handles)
% hObject    handle to TempR (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of TempR as text
%        str2double(get(hObject,'String')) returns contents of TempR as a double


% --- Executes during object creation, after setting all properties.
function TempR_CreateFcn(hObject, eventdata, handles)
% hObject    handle to TempR (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function PresR_Callback(hObject, eventdata, handles)
% hObject    handle to PresR (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of PresR as text
%        str2double(get(hObject,'String')) returns contents of PresR as a double


% --- Executes during object creation, after setting all properties.
function PresR_CreateFcn(hObject, eventdata, handles)
% hObject    handle to PresR (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function FracR_Callback(hObject, eventdata, handles)
% hObject    handle to FracR (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of FracR as text
%        str2double(get(hObject,'String')) returns contents of FracR as a double


% --- Executes during object creation, after setting all properties.
function FracR_CreateFcn(hObject, eventdata, handles)
% hObject    handle to FracR (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


function DisableButtons(handles)
 % disable User input and change pointer to hourglass
set(handles.CTB_GUI,'Pointer','watch');
set(handles.Gen_PT2S,'Enable','off'); %disable the Execute button
set(handles.Gen_SP2T,'Enable','off'); %disable the Execute button
set(handles.Gen_H2T,'Enable','off'); %disable the Execute button
set(handles.Gen_T2H,'Enable','off'); %disable the Execute button
set(handles.Gen_All,'Enable','off'); %disable the Execute button
refresh(handles.CTB_GUI);
pause(1);

function EnableButtons(handles)
% re-enable User input and change pointer to arrow
set(handles.CTB_GUI,'Pointer','arrow');
set(handles.Gen_PT2S,'Enable','on'); %enable the Execute button
set(handles.Gen_SP2T,'Enable','on'); %enable the Execute button
set(handles.Gen_H2T,'Enable','on'); %enable the Execute button
set(handles.Gen_T2H,'Enable','on'); %enable the Execute button
set(handles.Gen_All,'Enable','on'); %enable the Execute button

function in = StreamSetup(handles)
in.NameFile = get(handles.Import_Phase_File,'String');
in.Name1 = strsplit(get(handles.Name1,'String'));
in.Name2 = strsplit(get(handles.Name2,'String'));
in.Species1 = str2num(get(handles.Species1,'String'));
in.Species2 = str2num(get(handles.Species2,'String'));
in.Temp = str2num(get(handles.TempR,'String'));
in.Pres = str2num(get(handles.PresR,'String'));
in.Frac = str2num(get(handles.FracR,'String'));

function Fun_SP2T(handles,in)
in.type = {'SP2T'};
TableGen(in.type, in.NameFile, in.Name1, in.Name2, in.Species1, in.Species2, in.Temp, in.Pres, in.Frac);


function Fun_PT2S(handles,in)
in.type = {'PT2S'};
TableGen(in.type, in.NameFile, in.Name1, in.Name2, in.Species1, in.Species2, in.Temp, in.Pres, in.Frac);

function Fun_T2H(handles,in)
in.type = {'T2H'};
TableGen(in.type, in.NameFile, in.Name1, in.Name2, in.Species1, in.Species2, in.Temp, in.Pres, in.Frac);

function Fun_H2T(handles,in)
in.type = {'H2T'};
TableGen(in.type, in.NameFile, in.Name1, in.Name2, in.Species1, in.Species2, in.Temp, in.Pres, in.Frac);

function Fun_All(handles,in)
in.type = {'SP2T' 'PT2S' 'T2H' 'H2T'} ;
TableGen(in.type, in.NameFile, in.Name1, in.Name2, in.Species1, in.Species2, in.Temp, in.Pres, in.Frac);
