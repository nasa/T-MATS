function MSK_PlotTMap_TMATS()

%get variables
NcV = evalin('base',get_param(gcb, 'Y_T_Map_NcVec_M'));
PRV = evalin('base',get_param(gcb, 'X_T_Map_PRVec_M'));
WcA = evalin('base',get_param(gcb, 'T_T_Map_WcArray_M'));
EffA = evalin('base',get_param(gcb,'T_T_Map_EffArray_M'));

sWc = evalin('base',get_param(gcb, 's_T_Wc_M'));
sPR = evalin('base',get_param(gcb, 's_T_PR_M'));
sEff = evalin('base',get_param(gcb,'s_T_Eff_M'));

Scalego = get_param(gcb,'Scale_M');

switch Scalego
    case 'none'
        TMATS.PlotTMap(NcV,PRV, WcA,EffA)
    case 'user defined'
        TMATS.PlotTMap(NcV,PRV, WcA,EffA,'scalar',sWc,sPR,sEff)
    case 'iDesign'
        FileName = evalin('base',get_param(gcb,'FVar_M'));
        P = load(FileName);
        sWci = P(2);
        sPRi = P(3);
        sEffi =P(4);
        
        TMATS.PlotTMap(NcV,PRV, WcA,EffA,'scalar',sWci,sPRi,sEffi)
end