

function [Win, TtIn, PtIn, htIn, tempcomp] = compWrite( Name, Species, compNN )

tempcomp = '';
for cName = 1:6
    for cSpecies = 1:6
        if Species{cName,cSpecies} ~= 0
            tempcomp = sprintf( '%s %s:%f', tempcomp, Name{cName,cSpecies},Species{cName,cSpecies}*compNN(cName+4) );
        end
    endsfsf
end
Win = compNN(1);
TtIn = compNN(2);
PtIn = compNN(3)
htIn = compNN(4);
