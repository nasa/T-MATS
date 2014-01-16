function iDesign_En_TMATS(callbackInfo)
%		T-MATS -- iDesign_En_TMATS.m
% *************************************************************************
% written by Jeffryes Chapman 
% NASA Glenn Research Center, Cleveland, OH
% April 19th, 2013
%
% This script sets all iDesign bit for blocks withing a Model
% *************************************************************************

% Set iDesign_M for all blocks in a model.
temp.p = get_param(gcs,'Blocks');
temp.i = length(temp.p);
for i = 1:temp.i
    temp.block = strcat(gcs, '/',temp.p(i));
temp.see = get_param(temp.block{1},'Name');
try
set_param(temp.block{1} , 'iDesign_M', 1);
catch
end
end