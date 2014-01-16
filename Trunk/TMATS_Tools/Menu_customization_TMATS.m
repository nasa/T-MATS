function Menu_customization_TMATS()
%		T-MATS -- Menu_customization_TMATS.m
% *************************************************************************
% written by Jeffryes Chapman 
% NASA Glenn Research Center, Cleveland, OH
% April 19th, 2013
%
% This script creates the Menu objects in the Simulink window
% for the T-MATS tools. To have this script called automatically it can be
% called from the MATLAB setup.m script. Note: load_simulink should be used
% before this function is called to initialize Simlulink and allow it to be
% changed.
% *************************************************************************
  

%% Register custom menu function.

  cm = sl_customization_manager;
  cm.addCustomMenuFcn('Simulink:DiagramMenu', @getMyMenuItems);
end

%% Define the custom menu function.
function schemaFcns = getMyMenuItems(callbackInfo) 
  schemaFcns = {@getItem1}; 
end

%% Define the schema function for first menu item.
function schema = getItem1(callbackInfo)
  % Make a submenu label 'T-MATS Tools' with     
  % the other menu items.      
  schema = sl_container_schema;
  schema.label = 'T-MATS Tools';     
  schema.childrenFcns = {@getItem2, @getItem3, @getItem4, @getItem5};
end 

function schema = getItem2(callbackInfo)
% Create a menu item to call iDesign_En_TMATS
  schema = sl_action_schema;
  schema.label = 'GF_Convert';
  schema.userdata = 'GF_Convert';
  schema.accelerator = 'Alt+K';
  schema.callback = @GF_Convert_TMATS; 
end

function schema = getItem3(callbackInfo)
  % Create a menu item to call iDesign_En_TMATS
  schema = sl_action_schema;
  schema.label = 'iDesign_ON';
  schema.userdata = 'iDesign_ON';	
  schema.callback = @iDesign_En_TMATS; 
end

function schema = getItem4(callbackInfo)
  % Create a menu item to call iDesign_Off_TMATS
  schema = sl_action_schema;
  schema.label = 'iDesign_Off';
  schema.userdata = 'iDesign_Off';	
  schema.callback = @iDesign_Off_TMATS; 
end

function schema = getItem5(callbackInfo)
  % Create a menu item to call iDesign_Off_TMATS
  schema = sl_action_schema;
  schema.label = 'Block Link Setup';
  schema.userdata = 'Block Link Setup';	
  schema.accelerator = 'Alt+L';
  schema.callback = @Link_Setup_TMATS; 
end
