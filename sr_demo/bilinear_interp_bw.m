function imgout=bilinear_interp_bw(img)
	% expand widthwise
	imgout=bilinear_interp_1d(img);
	% flip and expand again
	imgout=bilinear_interp_1d(imgout');
	% flip back
	imgout=imgout';


% Expand the image widthwise, filling in intermediate values
	% between columns.
function imgout=bilinear_interp_1d(img)

	% a = img(:,1:size(img,2)-1)
		% a is img with the last column removed
	% b = img(:,2:size(img,2))
		% b is img with the first column removed
	% now add both togeter, and divide all elements by 2
	interp=(img(:,1:size(img,2)-1)+img(:,2:size(img,2)))/2;

	% create zero matrix with same num of rows, but twice the columns (minus 1)
	imgout=zeros(size(img,1),size(img,2)*2-1);

	%a = kron(interp,[0 1])
		% insert columns of 0's before each column.
	%b = kron(img(:,1:size(img,2)-1),[1 0])
		% insert columns of 0's after each column
	%a+b
		% combine the two, so interp's columns end up being inserted after img's
	% imgout(:,1:size(imgout,2)-1)=
		% set every column but the last to a+b.  So imgout will look like a+b with a
		% zero column vector added to the end
	imgout(:,1:size(imgout,2)-1)=kron(img(:,1:size(img,2)-1),[1 0])+kron(interp,[0 1]);

	% add last column of img to imgout
	imgout(:,size(imgout,2))=img(:,size(img,2));
