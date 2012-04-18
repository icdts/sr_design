#include "../final/load_images.h"

int main(int argc, char *argv[]){
	if( argc != 2) {
		std::cout <<" Usage: display_image ImageToLoadAndDisplay" << std::endl;
		return -1;
    }

	std::string dir = std::string(argv[1]);
	std::vector<input_image> files = load_images(std::string(argv[1]));

	std::cout << "Returned from load_images" << std::endl;

	for (unsigned int i = 0; i < files.size(); i++) {
		cv::imshow(files[i].name,files[i].file);
	}
	cv::waitKey(0);
	
	return 0;
}
