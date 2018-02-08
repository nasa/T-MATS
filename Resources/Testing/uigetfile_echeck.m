function [filename, pathname, filterindex] = uigetfile_echeck(varargin)

%UIGETFILE_ECHECK Standard open file dialog box.
%  This is a version of the UIGETFILE built-in MATLAB function.  The
%  difference is that if the user presses the Cancel button, and error
%  occurs with test displayed to the screen.  Execution of the program then
%  ends.
%
%   See also UIGETFILE, UIGETDIR, UIPUTFILE, UIGETDIR_ECHECK.

%   Modified by R. May - 2008-08-18

[filename, pathname, filterindex] = uigetfile(varargin{:});

assert(~isequal(filename,0) && ~isequal(pathname,0), 'User Canceled Selection');

