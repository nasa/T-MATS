function [LBlade] = BladeStrainExp(rho,TEVec,EVec,T,N,RoRotor,LBladeBase)

%BladeStrainExp Summary 
%   This function computs the strained blade length due to centrifugal
%   forces. The final blade length is computed with the following inputs:
%       rho - density, slug/ft^3
%       TEVec - Young's Modulus temperature data vector, degR
%       EVec - Young's Modulus data vector, lbf/sqft
%       T - blade temperature, degR
%       N - rototional speed, rpm
%       RoRotor - outer radius of the rotor, ft
%       LBladeBase - unstrained blade length, ft

if T > max(TEVec)
    T = max(TEVec);
elseif T < min(TEVec)
    T = min(TEVec);
end

E = interp1(TEVec,EVec,T,'linear');

LBlade = LBladeBase + rho*((2*pi/60)*N)^2*(LBladeBase^2)*(LBladeBase+RoRotor)/E;

end