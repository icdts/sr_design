%Called from sr_bw.m with (image,t2,'average',ds,search_range,sigma,th_prob1,handle_wb,offset_wb,scale_wb/2)
% Which is PROBABLY (image, t2, average, 4, [-8 8 -8 8], -1, 0.9, 0.85, h, offset (0 for bw,(color-1)/3 for color), scale (1 for bw, 1/3 for color) 
%In 'start super-reslution 1st phase...'

%Called from sr_bw.m with (image,t2,'sort_pocs',ds,search_range,sigma,th_prob2,handle_wb,offset_wb+(iter-1)*scale_wb/2/no_iter+scale_wb/2,scale_wb/2/no_iter);
%Which is PROBABLY (image, t2, 'sort_pocs', 4, [-8 8 -8 8], -1, 0.85, h, offset(iter=0->0.25, iter=1->0.5, iter=2->0.75), scale (prob 0.25)
%In 'start super-resolution 2nd phase...'

%Returns an array with the image
function [image,probs,shs,scores]=sr_one_step_wb(image,t,method,ds,sr,sigma,th_prob,wb_handle,offset,scale,orig_sh)

if ~exist('th_prob','var')
    th_prob=0.9;
end

%creates an array of zeros with 2 columns and number of rows equal to the
%number of input images
shs=zeros(length(t),2);

orig_image=image;
%image=kron(t{1},ones(ds));
%apparently orig_sh never exists, so only the else block will be executed
if exist('orig_sh','var') % OLD COMMENT cheat mode
   scores=[];
   for tid=1:length(t)
     shs(tid,:)=orig_sh(tid,:);
     probs(tid)=1;
   end
else
  for tid=1:length(t)
      %drawing waitbar
      waitbar(tid/2/length(t)*scale+offset,wb_handle);
        [tmp_sh,tmp_prob,tmp_scores]=subpixel_register(image,t{tid},ds,sr,sigma);
        probs(tid)=tmp_prob;
%        tmp_sh=orig_sh{tid}-orig_sh{1}; % cheating
%        tmp_prob=1; % cheating
        %replaces the row in shs corresponding to the input image id with
        %its shift value from subpixel_register
        shs(tid,:)=tmp_sh;
        %replaces the index in scores corresponding to the input image id with
        %its scores from subpixel_register
        scores{tid}=tmp_scores;
  end
end

%based off the function alls, the only methods used are average and
%sort_pocs. There's no elseif for sort_pocs, so it should always give an
%error message
%if method is 'max_pocs'... seemingly never used
if strcmp(method,'max_pocs') % only project to the one with highest score
    [dummy,max_id]=max(probs);
    image=pocs(image,t{max_id},ds,shs(max_id,:));
%if method is 'pocs'... seemingly never called
elseif strcmp(method,'pocs')
  for tid=1:length(t)
      waitbar((length(t)+tid)/2/length(t)*scale+offset,wb_handle);
     if (probs(tid) > th_prob)
%               image=pocs(orig_image,t{tid},ds,shs(tid,:));
            image=pocs(image,t{tid},ds,shs(tid,:));        
%         imshow(image/255);
%         title(sprintf('tid: %d, prob: %f, shift: [%d,%d]',tid,probs(tid),shs(tid,1),shs(tid,2)));
%         pause;

     end
  end
  %if method is 'average'
elseif strcmp(method,'average')
  image=zeros(size(image));
  for tid=1:length(t)
      %draws waitbar
      waitbar((length(t)+tid)/2/length(t)*scale+offset,wb_handle);      
     %TO DO
      if (probs(tid) > th_prob)
            sh_image=shift_image(kron(t{tid},ones(ds)),-shs(tid,:));
            image=image+sh_image;
%         imshow(sh_image/255);
%         title(sprintf('tid: %d',tid));
%         pause;

     end
%         imshow(image/255);
%         title(sprintf('iter: %d, tid: %d',iter,tid));
%         pause;
  end   
  if length(find(probs>th_prob))>0
      image=image/length(find(probs>th_prob));
  else
      image=orig_image; % can't find suitable one
  end
else
  error('method is not recognized');
end  