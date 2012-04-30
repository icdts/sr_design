#include "debug.h"

using namespace std;
using namespace cv;

void debug(string str){
#ifdef DEBUG
	cerr << "DEBUG: " << str << endl;
#endif
}

void printMatrix(Mat m){
	vector<Mat> panels;
	
	split(m, panels);

	for(int channels = 0; channels < m.channels(); channels++){
		for(int row = 0; row < m.rows; row++){
			cout << "[";
			for(int col = 0; col < m.cols; col++){
				cout << panels[channels].at<float>(row, col)<<"\t";
			}
			cout << "]\n";
		}
		cout << "\n\n";
	}
}