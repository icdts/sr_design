% Shift the image in a direction (throwing out data), and then reduce it by a factor of ds.
%	Average the pixels in an area to come up with a image reduced by a factor of ds.
%	So, the image:
% 
%	    16     2     3    13
%        5    11    10     8
%	     9     7     6    12
%		 4    14    15     1
%
%	Gets shifted into:
%  		16    16     2     3
%		16    16     2     3
%		5     5    11    10
%		9     9     7     6
%
%	And then reduced to:
%
%		16.0000    2.5000
%		7.0000    8.5000
%
%	If sh is [1,1] and ds = 2

function imout=gen_shift_downsample_image(image,ds,sh)

% Make sure image is of size divisible by ds.  Throw out right and bottom edges if necessary
image=image(1:floor(size(image,1)/ds)*ds,1:floor(size(image,2)/ds)*ds);

% shift the image the way I'm told
	% throw out information in those two directions
image=shift_image(image,sh);

% create output of some fraction of the size of image
imouts=floor(size(image)/ds);
imout=zeros(imouts);

% Add each region into imout
for id=1:ds
    for jd=1:ds
        imout=imout+image(id:ds:size(image,1),jd:ds:size(image,2));
    end
end

% And divide to get the average of each region
imout=imout/ds/ds;

