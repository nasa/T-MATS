function makeInfo = rtwmakecfg
% RTWMAKECFG Add include and source directories to make files.
% makeInfo = RTWMAKECFG returns a structured array containing
% following fields:
%
%     makeInfo.includePath - cell array containing additional include
%                            directories. Those directories will be 
%                            expanded into include instructions of 
%                            generated make files.
%     
%     makeInfo.sourcePath  - cell array containing additional source
%                            directories. Those directories will be
%                            expanded into rules of generated make
%                            files.
%
%     makeInfo.library     - structure containing additional runtime library
%                            names and module objects.  This information
%                            will be expanded into rules of generated make
%                            files.


% Copyright 2010-2012 The MathWorks, Inc.

makeInfo.includePath = { ...
    pwd
    };

makeInfo.library     = { };