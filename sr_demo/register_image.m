function [im2_reg,sh,f]=register_image(im1,im2)
oim2=im2;
w=gen_window(size(im1),0.05,0.05);
im1=w.*im1; % mitigate boundary effect
im2=w.*im2; % by using w as a kernel
%dot-star is entry-by-entry multiplication

%matrix right division (mrdivide)
%must have same number of columns

%mean(im(:)) returns a single value for mean of all elements
im1=im1/mean(im1(:)); %pretend normalizing
im2=im2/mean(im2(:));

%2D FFT on image 1 entry-by-entry multipled with FFT on image 2 rotated 180
%degrees
tmp=fft2(im1).*fft2(flipud(fliplr(im2)));
f=fftshift(abs(ifft2(tmp))); %fftshift moves fft so zero frequency 
                             %component is in middle, ifft2 inverse fft
%find max value in the new image (value: dummy, index: shid)
[dummy,shid]=max(f(:));

%make a point based on the middle of the image
sh(1)=(mod(shid-1,size(im1,1))+1)-size(im1,1)/2;
sh(2)=ceil((shid-1)/size(im1,1))-size(im1,2)/2;
%shift the image based on the (new) midpoint?
%this is still somewhat unclear
im2_reg=shift_image(oim2,sh);
