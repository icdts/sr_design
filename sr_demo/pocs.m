function image=pocs(image,lr_image,ds,sh)

% shift image and reduce its size by factor of ds
sh_image=gen_shift_downsample_image(image,ds,sh);

%right division: A/B = A*B^-1
scale=lr_image./sh_image;

% a=kron(scale,ones(ds))
%    expands scale to the output size
% shift_image(a,-sh)
%   shift a in the opposite direction of sh_image
scale=shift_image(kron(scale,ones(ds)),-sh);

% multiple every element by scale
image=image.*scale;
