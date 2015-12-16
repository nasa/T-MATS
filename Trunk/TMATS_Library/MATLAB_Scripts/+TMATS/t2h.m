function h = t2h(T,FAR)
%Determine enthalpy [BTU/lbm] based on temperature [R] and fuel to air ratio
%Table lookup method
%h = TMATS.t2h(t,FAR)

h = T2HM_TMATS(T,FAR);
end