function MSK_PlotCMap_TMATS()

%get variables
NcV = evalin('base',get_param(gcb, 'Y_C_Map_NcVec_M'));
WcA = evalin('base',get_param(gcb, 'T_C_Map_WcArray_M'));
PRA = evalin('base',get_param(gcb, 'T_C_Map_PRArray_M'));
EffA = evalin('base',get_param(gcb,'T_C_Map_EffArray_M'));

sWc = evalin('base',get_param(gcb, 's_C_Wc_M'));
sPR = evalin('base',get_param(gcb, 's_C_PR_M'));
sEff = evalin('base',get_param(gcb,'s_C_Eff_M'));

SMEn = get_param(gcb,'DefSMEn_M');
WcSM = evalin('base',get_param(gcb,'X_C_Map_WcSurgeVec_M'));
PRSM = evalin('base',get_param(gcb,'T_C_Map_PRSurgeVec_M'));
if strcmp(SMEn, 'on')
    SRline = evalin('base',get_param(gcb, 'SRline_M')); 
    Rline = evalin('base',get_param(gcb, 'X_C_RlineVec_M'));
    LenNc = length(NcV);
    LenRline = length(Rline);
    ColWc = size(WcA,2);
    ColPR = size(PRA,2);
    RowWc = size(WcA,1);
    RowPR = size(PRA,1);
    if (LenNc == RowPR && LenNc == RowWc && LenRline == ColWc && LenRline == ColPR)
    SMPR = ones(length(LenNc));
    SMWc = ones(length(LenNc));
    for i = 1:LenNc
        SMPR(i) = interp2(Rline , NcV,  PRA, SRline, NcV(i));
        SMWc(i) = interp2(Rline , NcV,  WcA, SRline, NcV(i));
    end
    WcSM = SMWc;
    PRSM = SMPR;
    end
end


Scalego = get_param(gcb,'Scale_M');
SMgo = get_param(gcb,'SL_M');
plotgo = [Scalego SMgo];
switch plotgo
    case 'noneoff'
        TMATS.PlotCMap(NcV,WcA,PRA,EffA)
    case 'user definedoff'
        TMATS.PlotCMap(NcV,WcA,PRA,EffA,'scalar',sWc,sPR,sEff)
    case 'iDesignoff'
        FileName = evalin('base',get_param(gcb,'FVar_M'));
        P = load(FileName);
        sWci = P(2);
        sPRi = P(3);
        sEffi =P(4);
        
        TMATS.PlotCMap(NcV,WcA,PRA,EffA,'scalar',sWci,sPRi,sEffi)
    case 'noneon'
        TMATS.PlotCMap(NcV,WcA,PRA,EffA,'defsurge',WcSM,PRSM)
    case 'user definedon'
        TMATS.PlotCMap(NcV,WcA,PRA,EffA,'scalar',sWc,sPR,sEff,'defsurge',WcSM,PRSM)
    case 'iDesignon'
        FileName = evalin('base',get_param(gcb,'FVar_M'));
        P = load(FileName);
        sWci = P(2);
        sPRi = P(3);
        sEffi =P(4);
        
        TMATS.PlotCMap(NcV,WcA,PRA,EffA,'scalar',sWci,sPRi,sEffi,'defsurge',WcSM, PRSM, 'offset', 0.035)
end