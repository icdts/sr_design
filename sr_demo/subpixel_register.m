%First: with ds = 4, sr=[-8 8 -8 8], sigma = -1

% move around in search range (sr), scoring each window,
%	return shift with highest score

% So this figures out how to best align the low resolution images 

function [sh,prob,score]=subpixel_register(hr_image,lr_image,ds,sr,sigma)

if ~exist('sigma','var')
    sigma=40;
end
if sigma < 0
    sigma=40;
end
sid=1;
sranges={};
sigma2=sigma*sigma;

%for -8 to 8 in x direction then -8 to 8 iny direction
%sranges{n} = x, y  ??? why curly braces?
%score(n) = ??? something
for id=sr(1):sr(2) % search range
    for jd=sr(3):sr(4)
        sranges{sid}=[id jd];
        t=gen_shift_downsample_image(hr_image,ds,[id jd]);
		
		% Who knows why, but just copy these maths
        %prod(x): treat columns of x as arrays and returns a row vector of the product for each column 
        %exp(x): e raised to the x
        %x.^a: element-by-element power raising (each element of x is
        %raised to the a individually
        score(sid)=prod(prod(exp(-(t-lr_image).^2/(2*sigma*sigma))));
        sid=sid+1;
    end
end

[dummy,mid]=max(score);
prob=dummy/sum(score);
sh=sranges{mid};
