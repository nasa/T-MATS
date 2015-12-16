function T = h2t(h,FAR)
%Determine temperature [R] based on enthalpy [BTU/lbm] and fuel to air ratio
%Table lookup method
%T = TMATS.h2t(h,FAR)

T = H2TM_TMATS(h,FAR);
end