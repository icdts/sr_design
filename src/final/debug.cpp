#include "debug.h"

using namespace std;

void debug(std::string str){
#ifdef DEBUG
	cerr << "DEBUG: " << str << endl;
#endif
}