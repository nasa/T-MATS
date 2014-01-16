function GF_Convert_TMATS(callbackInfo)
%		T-MATS -- GF_Convert_TMATS.m
% *************************************************************************
% written by Jeffryes Chapman 
% NASA Glenn Research Center, Cleveland, OH
% April 19th, 2013
%
% This function converts GoTo tags to From tags and Inports to Outports.
% *************************************************************************

temp.BT = get_param(gcb, 'BlockType');
switch lower(temp.BT)
    case('from')
        temp.Pos = get_param(gcb,'Position');
        temp.Name = gcb;
        temp.Tag = get_param(gcb,'GotoTag');
        temp.FC = get_param(gcb,'ForegroundColor');
        temp.SN = get_param(gcb,'ShowName');
        delete_block(gcb);
        add_block('simulink/Signal Routing/Goto',temp.Name,'MakeNameUnique','off','ShowName','off','GotoTag', temp.Tag, 'Position',temp.Pos, 'ForegroundColor',temp.FC,'ShowName',temp.SN)
    case('goto')
        temp.Pos = get_param(gcb,'Position');
        temp.Name = gcb;
        temp.Tag = get_param(gcb,'GotoTag');
        temp.FC = get_param(gcb,'ForegroundColor');
        temp.SN = get_param(gcb,'ShowName');
        delete_block(gcb);
        add_block('simulink/Signal Routing/From',temp.Name,'MakeNameUnique','off','ShowName','off','GotoTag', temp.Tag, 'Position',temp.Pos, 'ForegroundColor',temp.FC,'ShowName',temp.SN)
    case('inport')
        temp.Pos = get_param(gcb,'Position');
        temp.Name = gcb;
        temp.FC = get_param(gcb,'ForegroundColor');
        delete_block(gcb);
        add_block('simulink/Sinks/Out1',temp.Name,'MakeNameUnique','off','ShowName','on','Position',temp.Pos, 'ForegroundColor',temp.FC)
    case('outport')
        temp.Pos = get_param(gcb,'Position');
        temp.Name = gcb;
        temp.FC = get_param(gcb,'ForegroundColor');
        delete_block(gcb);
        add_block('simulink/Sources/In1',temp.Name,'MakeNameUnique','off','ShowName','on','Position',temp.Pos, 'ForegroundColor',temp.FC)
    otherwise
end
