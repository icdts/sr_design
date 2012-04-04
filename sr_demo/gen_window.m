function w=gen_window(s,d1,d2);
%size s is an array s(1) = x, s(2) = y

if ~exist('d1','var')
    d1=0.1;
end
if ~exist('d2','var');
    d2=0.1;
end

% Create a s(1) by 1 matrix (column matrix) filled with ones
x=ones(s(1),1);
% Create a linear vector (row matrix) with s(1)*d1 evenly spaced numbers
%between 0 and 1
slope=linspace(0,1,round(s(1)*d1));
% Replace values in x with values in slope for full length of slope
x(1:length(slope))=slope;
% flip x around middle of x (assuming small d1 relative to s(1))
% ex: a,b,c,1,1,1,1,1,1,1,1,1,1
%    results in array a,b,c,1,1,1,1,1,1,1,c,b,a
x(length(x)-length(slope)+1:length(x))=fliplr(slope);

% Same for y
y=ones(1,s(2));
slope=linspace(0,1,round(s(2)*d2));
y(1:length(slope))=slope;
y(length(y)-length(slope)+1:length(y))=fliplr(slope);

% Results in a s(1) by s(2) matrix where all borders are 0 and the middle
%is 1 with s(1)*d1 numbers fading from 1 to 0 at each x edge and s(2)*d2 at
%each y edge
w=x*y;

