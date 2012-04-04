function imgout=bilinear_interp(img)

% s=size(img);
% s(1:2)=2*s(1:2);
% imgout=zeros(s);

%for color images create 3 simgle color images and run each of them through
%bw bilinear interp function, return image is a 3-D matrix 3 'colors' deep
if length(size(img))==3
    for id=1:size(img,3)
        imgout(:,:,id)=bilinear_interp_bw(img(:,:,id));
    end
else
    %otherwise this is just a bw image already so just b_i_bw
    imgout=bilinear_interp_bw(img);
end

