function varargout = DViewerGUI_TMATS(varargin)
% GUIDETEMPLATE0 MATLAB code for guidetemplate0.fig
%      GUIDETEMPLATE0, by itself, creates a new GUIDETEMPLATE0 or raises the existing
%      singleton*.
%
%      H = GUIDETEMPLATE0 returns the handle to a new GUIDETEMPLATE0 or the handle to
%      the existing singleton*.
%
%      GUIDETEMPLATE0('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in GUIDETEMPLATE0.M with the given input arguments.
%
%      GUIDETEMPLATE0('Property','Value',...) creates a new GUIDETEMPLATE0 or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before guidetemplate0_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to guidetemplate0_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Copyright 2002-2014 The MathWorks, Inc.

% Edit the above text to modify the response to help guidetemplate0

% Last Modified by GUIDE v2.5 18-Mar-2016 14:07:18

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
    'gui_Singleton',  gui_Singleton, ...
    'gui_OpeningFcn', @guidetemplate0_OpeningFcn, ...
    'gui_OutputFcn',  @guidetemplate0_OutputFcn, ...
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


% --- Executes just before guidetemplate0 is made visible.
function guidetemplate0_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to guidetemplate0 (see VARARGIN)

% Choose default command line output for guidetemplate0
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes guidetemplate0 wait for user response (see UIRESUME)
% uiwait(handles.DViewerGUI);


% --- Outputs from this function are returned to the command line.
function varargout = guidetemplate0_OutputFcn(hObject, eventdata, handles)
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on button press in btn_Run.
function btn_Run_Callback(hObject, eventdata, handles)
% hObject    handle to btn_Run (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

%disable User input and change pointer to hourglass
set(handles.DViewerGUI,'Pointer','watch');
set(handles.btn_set,'Enable','off'); %disable the Set button
set(handles.btn_Plot,'Enable','off'); %disable the Plot button
set(handles.btn_Run,'Enable','off'); %disable the Run button
refresh(handles.DViewerGUI);
pause(0.1);


FileNm = get(handles.edit_Mod,'String');
try
    disp([FileNm,' run has started'])
    evalin('base',['sim(','''',FileNm,'''',')']);
    disp([FileNm,' has been run successfully'])
catch ME
    disp([FileNm,' run has been aborted'])
    disp(ME.message)
end

% Re-enable inputs and return pointer
set(handles.DViewerGUI,'Pointer','arrow');
set(handles.btn_set,'Enable','on'); %enable the Set button
set(handles.btn_Plot,'Enable','on'); %enable the Plot button
set(handles.btn_Run,'Enable','on'); %enable the Run button
refresh(handles.DViewerGUI);



% --- Executes on button press in btn_Plot.
function btn_Plot_Callback(hObject, eventdata, handles)
% hObject    handle to btn_Plot (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

%disable User input and change pointer to hourglass
set(handles.DViewerGUI,'Pointer','watch');
set(handles.btn_set,'Enable','off'); %disable the Set button
set(handles.btn_Plot,'Enable','off'); %disable the Plot button
set(handles.btn_Run,'Enable','off'); %disable the Run button
refresh(handles.DViewerGUI);
pause(0.1);

FileNm = get(handles.edit_Mod,'String');
SS_EN = get(handles.chk_SS,'Value');
Dyn_EN = get(handles.chk_Trans,'Value');
Stn = get(handles.edit_Station,'String');
StnVec = evalin('base',Stn);
if Dyn_EN && SS_EN
    TMATS.SSplot(FileNm,StnVec,'TurbR','CompR');
    TMATS.TDplot(FileNm);
elseif SS_EN
    TMATS.SSplot(FileNm,StnVec);
elseif Dyn_EN
    TMATS.TDplot(FileNm);
else
    disp('No plots selected')
end

% Re-enable inputs and return pointer
set(handles.DViewerGUI,'Pointer','arrow');
set(handles.btn_set,'Enable','on'); %enable the Set button
set(handles.btn_Plot,'Enable','on'); %enable the Plot button
set(handles.btn_Run,'Enable','on'); %enable the Run button
refresh(handles.DViewerGUI);



function edit_Mod_Callback(hObject, eventdata, handles)
% hObject    handle to edit_Mod (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit_Mod as text
%        str2double(get(hObject,'String')) returns contents of edit_Mod as a double


% --- Executes during object creation, after setting all properties.
function edit_Mod_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit_Mod (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in chk_SS.
function chk_SS_Callback(hObject, eventdata, handles)
% hObject    handle to chk_SS (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of chk_SS


% --- Executes on button press in chk_Trans.
function chk_Trans_Callback(hObject, eventdata, handles)
% hObject    handle to chk_Trans (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of chk_Trans



function edit_Station_Callback(hObject, eventdata, handles)
% hObject    handle to edit_Station (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit_Station as text
%        str2double(get(hObject,'String')) returns contents of edit_Station as a double


% --- Executes during object creation, after setting all properties.
function edit_Station_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit_Station (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on selection change in pop_IDes.
function pop_IDes_Callback(hObject, eventdata, handles)
% hObject    handle to pop_IDes (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns pop_IDes contents as cell array
%        contents{get(hObject,'Value')} returns selected item from pop_IDes


% --- Executes during object creation, after setting all properties.
function pop_IDes_CreateFcn(hObject, eventdata, handles)
% hObject    handle to pop_IDes (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in btn_set.
function btn_set_Callback(hObject, eventdata, handles)
% hObject    handle to btn_set (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

%disable User input and change pointer to hourglass
set(handles.DViewerGUI,'Pointer','watch');
set(handles.btn_set,'Enable','off'); %disable the Set button
set(handles.btn_Plot,'Enable','off'); %disable the Plot button
set(handles.btn_Run,'Enable','off'); %disable the Run button
refresh(handles.DViewerGUI);
pause(0.1);

PopVal = get(handles.pop_IDes,'Value');
FileNm = get(handles.edit_Mod,'String');
IDesVal = PopVal - 1;
if exist(FileNm, 'file') == 4
    % find all models being referenced in the model
    [ref, blocks] = find_mdlrefs(FileNm);
    for i = 1:length(ref)
        load_system(ref(i));
        SetIDesFun_TMATS(ref(i), IDesVal);
        if strcmp(get_param(ref(i),'Dirty'),'on')
            save_system(ref(i));
        end 
    end
    bdclose('all');
else
    disp([FileNm, ' does not exist'])
end

% Re-enable inputs and return pointer
set(handles.DViewerGUI,'Pointer','arrow');
set(handles.btn_set,'Enable','on'); %enable the Set button
set(handles.btn_Plot,'Enable','on'); %enable the Plot button
set(handles.btn_Run,'Enable','on'); %enable the Run button
refresh(handles.DViewerGUI);
