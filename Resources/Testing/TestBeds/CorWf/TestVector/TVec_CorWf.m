function MWS = TVec_CorWf(MWS)
%		TMATS -- setup_CorWf.m
% *************************************************************************
% written by Jeffryes Chapman 
% NASA Glenn Research Center, Cleveland, OH
% Dec 18th, 2012
%
% This function creates the properties of the corrected fuel flow component.
% *************************************************************************


MWS.TV.VarNames = {'CorWf' 'Wf' 'Tt' 'Pt'};

WfIn = [3 4];
Tt = 500;
Pt = 40;


a = 1;
for i1 = 1:length(WfIn)
    for i2 = 1:length(Tt)
        for i3 = 1:length(Pt)
                    MWS.TV.TV(1,a) = WfIn(i1);
                    MWS.TV.TV(2,a) = Tt(i2);
                    MWS.TV.TV(3,a) = Pt(i3);
                    a = a + 1;
        end
    end
end




