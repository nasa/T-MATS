function T = sp2t(s,P,FAR)
%Determine temperature [R] based on entropy [BTU/(lbm*R)], pressure [psia], and fuel to air ratio
%Table lookup method
%T = TMATS.sp2t(s,P,FAR)

T = SP2TM_TMATS(s,P,FAR);
end