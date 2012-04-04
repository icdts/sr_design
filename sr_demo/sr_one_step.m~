function [image,probs,shs,scores]=sr_one_step(image,t,method,ds,sr,sigma,th_prob,orig_sh)

if ~exist('th_prob','var')
    th_prob=0.9;
end

shs=zeros(length(t),2);

orig_image=image;
%image=kron(t{1},ones(ds));
if exist('orig_sh','var') % cheat mode
   scores=[];
   for tid=1:length(t)
     shs(tid,:)=orig_sh(tid,:);
     probs(tid)=1;
   end
else
  for tid=1:length(t)
        [tmp_sh,tmp_prob,tmp_scores]=subpixel_register(image,t{tid},ds,sr,sigma);
        probs(tid)=tmp_prob;
%        tmp_sh=orig_sh{tid}-orig_sh{1}; % cheating
%        tmp_prob=1; % cheating
        shs(tid,:)=tmp_sh;
        scores{tid}=tmp_scores;
  end
end

if strcmp(method,'max_pocs') % only project to the one with highest score
    [dummy,max_id]=max(probs);
    image=pocs(image,t{max_id},ds,shs(max_id,:));
elseif strcmp(method,'pocs')
  for tid=1:length(t)
     if (probs(tid) > th_prob)
%               image=pocs(orig_image,t{tid},ds,shs(tid,:));
            image=pocs(image,t{tid},ds,shs(tid,:));        
%         imshow(image/255);
%         title(sprintf('tid: %d, prob: %f, shift: [%d,%d]',tid,probs(tid),shs(tid,1),shs(tid,2)));
%         pause;

     end
  end
elseif strcmp(method,'average')
  image=zeros(size(image));
  for tid=1:length(t)
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