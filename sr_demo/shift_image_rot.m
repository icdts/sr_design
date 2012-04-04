function image=shift_image_rot(image,sh)

% image=image(1:floor(size(image,1)/ds)*ds,1:floor(size(image,2)/ds)*ds);

if sh(1)>0 % pad above
    %image=[repmat(image(1,:),[sh(1),1]); image];
    %pull bottom row to top and push image down
    %The delete the old bottom row
    %i.e rotated the image around a tube
    image=[image(size(image,1)-sh(1)+1:size(image,1),:); image];
    image(size(image,1)-sh(1)+1:size(image,1),:)=[];
elseif sh(1)<0 % pad below
    %image=[image; repmat(image(size(image,1),:),[-sh(1),1])];
    %same same same
    image=[image; image(1:-sh(1),:)];
    image(1:-sh(1),:)=[];
end

if sh(2)>0 % pad left
    %image(:,1),[1,sh(2)]), image];
    %same same same
    image=[image(:,size(image,2)-sh(2)+1:size(image,2)), image];
    image(:,size(image,2)-sh(2)+1:size(image,2))=[];
elseif sh(2)<0 % pad right
    %image=[image, repmat(image(:,size(image,2)),[1,-sh(2)])];
    %same same same
    image=[image, image(:,1:-sh(2))];
    image(:,1:-sh(2))=[];
end
