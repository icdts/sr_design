%Called from sr_gui by SRBWbutton_Callback (image,h,0,1 )
%Called from sr_gui by SRColorButton_Callback (image,h,(color-1)/3, 1/3)
function image=sr_bw(t2,handle_wb,offset_wb,scale_wb,ds,max_shift,th_prob1,th_prob2,no_iter)

%EDIT
%Theoretically these are never passed into this function 
if ~exist('ds','var')
    ds=4; % four time super resolution ??? what
end
if ~exist('max_shift','var')
    max_shift=8; %Can never move more than this ??? what
end
if ~exist('th_prob1','var')
    th_prob1=0.9;
end
if ~exist('th_prob2','var')
    th_prob2=0.85;
end
if ~exist('no_iter','var')
    no_iter=2;
end

fprintf('start SR\n...');
% [-8 8 -8 8]
search_range=[-max_shift max_shift -max_shift max_shift];   
%Kron image? (first chosen image? or all images?) with a 4x4 matrix of 1's
%Effectivey this turns each old pixel into 4 'new' pixels with the same val
image=kron(t2{1},ones(ds));

sigma=-1; % OLD COMMENT use the default sigma to estimate prob (see also subpixel_register.m)
fprintf('start super-resolution 1st phase...\n');
tic
%image, t2, average, 4, [-8 8 -8 8], -1, 0.9, 0.85, h, offset (0 for bw,
%(color-1)/3 for color), scale (1 for bw, 1/3 for color)
[image,probs,shs,scores]=sr_one_step_wb(image,t2,'average',ds,search_range,sigma,th_prob1,handle_wb,offset_wb,scale_wb/2);
toc

%sort_pocs as the method should always give an error message... weird.
fprintf('start super-resolution 2nd phase...\n');
for iter=1:no_iter
    tic
    %image, t2, 'sort_pocs', 4, [-8 8 -8 8], -1, 0.85, h, offset
    %(iter=0->0.25, iter=1->0.5, iter=2->0.75), scale (prob 0.25)
    %Only runs twice for some reason ???
    [image,probs,shs,scores]=sr_one_step_wb(image,t2,'sort_pocs',ds,search_range,sigma,th_prob2,handle_wb,offset_wb+(iter-1)*scale_wb/2/no_iter+scale_wb/2,scale_wb/2/no_iter);
    toc
    iter
    %IF number of probs greater than 0.85 is the same as the total number
    %of probs, break out of the loop early? (even though it only seems to
    %run twice?)
    if length(find(probs>th_prob2))==length(probs)
          break;
    end
end

