function [im2_reg,sh,f]=register_color_image(cim1,cim2)

% im1=double(sum(cim1,3));
% im2=double(sum(cim2,3));
% tmp=fft2(im1).*fft2(flipud(fliplr(im2)));
% f=fftshift(abs(ifft2(tmp)));
% 
% [dummy,shid]=max(f(:));
% sh(1)=(mod(shid-1,size(im1,1))+1)-size(im1,1)/2;
% sh(2)=ceil((shid-1)/size(im1,1))-size(im1,2)/2;
% im2_reg=shift_image(cim2,sh);
% THE ABOVE IS OLD COMMENTS THAT ARE JUST THE FULL TEXT OF
% REGISTER_IMAGE.M BECAUSE THESE ARE BASICALLY THE SAME FUNCTION

f=zeros([size(cim1,1) size(cim1,2)]);

for c=1:3
    im1=double(cim1(:,:,c)); %difference here, no pretend normalizing
    im2=double(cim2(:,:,c));
    %2D FFT on image 1 entry-by-entry multiplied with 180degree rotated FFT on image 2
    tmp=fft2(im1).*fft2(flipud(fliplr(im2)));
    f=f+fftshift(abs(ifft2(tmp)));
end
[dummy,shid]=max(f(:));

%get a shift value by looking at the middle of the normalized FFT images
sh(1)=(mod(shid-1,size(im1,1))+1)-size(im1,1)/2;
sh(2)=ceil((shid-1)/size(im1,1))-size(im1,2)/2;

%shift the images (how far and why?, not super clear on this)
im2_reg1=shift_image(cim2(:,:,1),sh);
im2_reg2=shift_image(cim2(:,:,2),sh);
im2_reg3=shift_image(cim2(:,:,3),sh);

%The 3 new images get consolidated back into a single file with 3 layers
im2_reg=zeros(size(cim1));
im2_reg(:,:,1)=im2_reg1;
im2_reg(:,:,2)=im2_reg2;
im2_reg(:,:,3)=im2_reg3;
