function [RoRotor] = RotorStrainExp(rho,TEVec,EVec,TvVec,vVec,T,N,RoRotorBase)

%BladeStrainExp Summary 
%   This function computs the strained blade length due to centrifugal
%   forces. The final blade length is computed with the following inputs:
%       rho - density, slug/ft^3
%       TEVec - Young's Modulus temperature data vector, degR
%       EVec - Young's Modulus data vector, lbf/sqft
%       TvVec - Poisson's Ratio temperature data vector, degR
%       vVec - Poisson's Ratio data vector
%       T - blade temperature, degR
%       N - rototional speed, rpm
%       RoRotorBase - unstrained radius of the rotor, ft

if T > max(TEVec)
    T = max(TEVec);
elseif T < min(TEVec)
    T = min(TEVec);
end

if T > max(TvVec)
    T = max(TvVec);
elseif T < min(TvVec)
    T = min(TvVec);
end

E = interp1(TEVec,EVec,T,'linear');
v = interp1(TvVec,vVec,T,'linear');

RoRotor = RoRotorBase + rho*((1-v)/(4*E))*((2*pi/60)*N)^2*RoRotorBase^3;

end