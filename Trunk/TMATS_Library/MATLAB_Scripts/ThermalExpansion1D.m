function [L] = ThermalExpansion1D(TalphaVec,alphaVec,Lref,Tref,T)

%ThermalExpansion1Dlm Summary 
%   This function performs a 1D elastic thermal expansion calculation. It
%   outputs the new length or radius, L, based on the following inputs:
%       TalphaVec - Thermal expansion coefficient temperature data
%       alphaVec - Thermal expansion coefficient data
%       Lref - reference length
%       Tref - reference temperature at known length Lref
%       T - temperature of the component

if T > max(TalphaVec)
    T = max(TalphaVec);
elseif T < min(TalphaVec)
    T = min(TalphaVec);
end

if Tref > max(TalphaVec)
    Tref = max(TalphaVec);
elseif Tref < min(TalphaVec)
    Tref = min(TalphaVec);
end

n = 50;
TVec = linspace(Tref,T,n);
alpha = zeros(1,n);
for i = 1:n
    alpha(i) = interp1(TalphaVec,alphaVec,TVec(i),'linear');
end

Int_alphadT = trapz(TVec,alpha);
dL = Lref*Int_alphadT;

L = Lref + dL;

end