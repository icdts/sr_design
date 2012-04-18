/*
%This just gets images from a file, nothing else and will be totally
%different when we have to implement it:
function files=get_images_from_files(prefix, suffix, frange);

id=1;
for imid=frange
    files{id}=imread(sprintf('%s%d.%s',prefix,imid,suffix));
    id=id+1;
    imid
end
*/
#include "load_images.h"

std::vector<input_image> load_images(std::string dir){
	std::vector<input_image> files;

	DIR *dp;
	struct dirent *dirp;
	if((dp = opendir(dir.c_str())) == NULL) {
		std::cout << "Error(" << errno << ") opening " << dir << std::endl;
		exit(errno);
	}

	while ((dirp = readdir(dp)) != NULL) {
		if( std::string(dirp->d_name) != "." && std::string(dirp->d_name) != ".."){
			input_image f;
			f.name = dir + std::string(dirp->d_name);

			f.file = cv::imread(f.name.c_str(), CV_LOAD_IMAGE_COLOR);
			f.name = std::string(dirp->d_name);

			files.push_back(f);
		}
	}

	closedir(dp);

	return files;
}

