#include "debug.h"

using namespace std;
using namespace cv;

/*******************************************************************************

    debug simply outputs to the console whatever string is passed to it.
    
    debug:
        string str:
            This is a string variable to be output to the console.

*******************************************************************************/
void debug(string str){
#ifdef DEBUG
    cerr << "DEBUG: " << str << endl;
#endif
}

/*******************************************************************************

    printMatrix accepts a single Mat variable as a parameter.  It then breaks
    the matrix up into its individual panels and outputs its numeric values in a
    readable format.

    printMatrix:
        Mat m:
            m is a input matrix of type CV_32F

*******************************************************************************/
void printMatrix(Mat m){
    vector<Mat> panels;
    
    /* Split the Mat into individual channels */
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