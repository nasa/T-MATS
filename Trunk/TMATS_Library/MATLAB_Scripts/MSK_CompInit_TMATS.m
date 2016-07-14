% 
% This file was designed to preprocess compressor mask inputs for insertion
% into the compressor S-function and produce warnings for improper Mask 
% input sizes. This file is called from the Initialization tab of the 
% Mask (edit Mask).

% if iDesign is set to 1, get scalars from file.
if iDesign_M == 1
    P = load(FVar_M);
    s_C_Nc_M = P(1);
    s_C_Wc_M = P(2);
    s_C_PR_M = P(3);
    s_C_Eff_M =P(4);
end

% Produce errors if map sizings (axis and tables) are incorrect.
if length(X_C_RlineVec_M) ~= size(T_C_Map_WcArray_M,2)
    warning('number of columns in T_C_Map_WcArray_M does not match length of X_C_RlineVec_M');
end
if length(X_C_RlineVec_M) ~= size(T_C_Map_PRArray_M,2)
    warning('number of columns in T_C_Map_PRArray_M does not match length of X_C_RlineVec_M');
end
if length(X_C_RlineVec_M) ~= size(T_C_Map_EffArray_M,2)
    warning('number of columns in T_C_Map_EffArray_M does not match length of X_C_RlineVec_M');
end
if length(Y_C_Map_NcVec_M) ~= size(T_C_Map_WcArray_M,1)
    warning('number of rows in T_C_Map_WcArray_M does not match length of Y_C_Map_NcVec_M');
end
if length(Y_C_Map_NcVec_M) ~= size(T_C_Map_PRArray_M,1)
    warning('number of rows in T_C_Map_PRArray_M does not match length of Y_C_Map_NcVec_M');
end
if length(Y_C_Map_NcVec_M) ~= size(T_C_Map_EffArray_M,1)
    warning('number of rows in T_C_Map_EffArray_M does not match length of Y_C_Map_NcVec_M');
end
if length(Z_C_AlphaVec_M) ~= size(T_C_Map_WcArray_M,3)
    if length(Z_C_AlphaVec_M) > 1
        warning('number of layers in T_C_Map_WcArray_M does not match length of Z_C_AlphaVec_M');
    else
        warning('T_C_Map_WcArray_M should be a 2-D matrix');
    end
end
if length(Z_C_AlphaVec_M) ~= size(T_C_Map_PRArray_M,3)
    if length(Z_C_AlphaVec_M) > 1
        warning('number of layers in T_C_Map_PRArray_M does not match length of Z_C_AlphaVec_M');
    else
        warning('T_C_Map_PRArray_M should be a 2-D matrix');
    end
end
if length(Z_C_AlphaVec_M) ~= size(T_C_Map_EffArray_M,3)
    if length(Z_C_AlphaVec_M) > 1    
        warning('number of layers in T_C_Map_EffArray_M does not match length of Z_C_AlphaVec_M'); 
    else
        warning('T_C_Map_EffArray_M should be a 2-D matrix');
    end
end

% transpose SM vectors, so matrix can be defined as "sxr"
X_C_Map_WcSurgeVec_M = X_C_Map_WcSurgeVec_M';
T_C_Map_PRSurgeVec_M = T_C_Map_PRSurgeVec_M';

%------------------------------------------
% if Stall line is defined as an R-line

if (DefSMEn_M > 0.5)
    % get all vector and table sizes
    LenNc = length(Y_C_Map_NcVec_M);
    LenRline = length(X_C_RlineVec_M);
    LenAlpha = length(Z_C_AlphaVec_M);
    ColWc = size(T_C_Map_WcArray_M,2);
    ColPR = size(T_C_Map_PRArray_M,2);
    RowWc = size(T_C_Map_WcArray_M,1);
    RowPR = size(T_C_Map_PRArray_M,1);
    LayWc = size(T_C_Map_WcArray_M,3);
    LayPR = size(T_C_Map_PRArray_M,3);
    
    if (LenNc == RowPR && LenNc == RowWc && LenRline == ColWc && LenRline == ColPR && LenAlpha == LayWc && LenAlpha == LayPR)
    % if sizes match  initialize stall line/plane vectors
    SMPR = ones(length(LenNc), length(LenAlpha));
    SMWc = ones(length(LenNc), length(LenAlpha));
    % populate stall margin lookup vectors
    for (i = 1:LenNc)
        if LenAlpha > 1
            for ii = 1:LenAlpha
                % if there is an alpha dimension make stall plane
                SMPR(i,ii) = interp3(X_C_RlineVec_M,Y_C_Map_NcVec_M,Z_C_AlphaVec_M, T_C_Map_PRArray_M, SRline_M,Y_C_Map_NcVec_M(i),Z_C_AlphaVec_M(ii));
                SMWc(i,ii) = interp3(X_C_RlineVec_M,Y_C_Map_NcVec_M,Z_C_AlphaVec_M, T_C_Map_WcArray_M, SRline_M,Y_C_Map_NcVec_M(i),Z_C_AlphaVec_M(ii));
            end
        else
            SMPR(i) = interp2(X_C_RlineVec_M,Y_C_Map_NcVec_M, T_C_Map_PRArray_M,SRline_M,Y_C_Map_NcVec_M(i));
            SMWc(i) = interp2(X_C_RlineVec_M,Y_C_Map_NcVec_M, T_C_Map_WcArray_M,SRline_M,Y_C_Map_NcVec_M(i));
        end
    end
    X_C_Map_WcSurgeVec_M = SMWc';
    T_C_Map_PRSurgeVec_M = SMPR';
    end
end

if ~isequal(size(T_C_Map_PRSurgeVec_M),size(X_C_Map_WcSurgeVec_M))
    warning('size of X_C_Map_WcSurgeVec_M must match size of T_C_Map_PRSurgeVec_M');
end

if length(Z_C_AlphaVec_M) > 1
    % produce warnings if Alpha vector size does not match stall plane
    % definition. (note: mask definition has been transposed
    if length(Z_C_AlphaVec_M) ~= size(X_C_Map_WcSurgeVec_M,1)
        warning('number of columns in X_C_Map_WcSurgeVec_M does not match length of Z_C_AlphaVec_M');
    end
    if length(Z_C_AlphaVec_M) ~= size(T_C_Map_PRSurgeVec_M,1)  
        warning('number of columns in T_C_Map_PRSurgeVec_M does not match length of Z_C_AlphaVec_M'); 
    end
end
