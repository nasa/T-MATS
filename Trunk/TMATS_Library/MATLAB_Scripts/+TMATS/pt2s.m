function s = pt2s(P,T,FAR)
%Determine entropy [BTU/(lbm*R)] based on pressure [psia], temperature [R], and fuel to air ratio
%Table lookup method
%s = TMATS.pt2s(P,T,FAR)

s = PT2SM_TMATS(P,T,FAR);
end