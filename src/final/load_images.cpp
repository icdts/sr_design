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

using namespace std;
using namespace cv;

vector<input_image> load_images(string dir){
	vector<input_image> files;
	Mat tmp;
	DIR *dp;

	struct dirent *dirp;
	if((dp = opendir(dir.c_str())) == NULL) {
		cout << "Error(" << errno << ") opening " << dir << endl;
		exit(errno);
	}

	while ((dirp = readdir(dp)) != NULL) {
		if( string(dirp->d_name) != "." && string(dirp->d_name) != ".."){
			input_image f;
			f.name = dir + string(dirp->d_name);

			tmp = imread(f.name.c_str(), CV_LOAD_IMAGE_COLOR);
			tmp.convertTo(f.file,CV_32F);

			f.name = string(dirp->d_name);

			files.push_back(f);
		}
	}

	closedir(dp);

	return files;
}

