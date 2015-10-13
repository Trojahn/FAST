#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;
using namespace cv;

class Utils {
	public:
		static Mat extractHistogram(Mat frame);
		static void writeOutputFile(string outFile, vector< pair<int,int> > shots);
		static bool checkFile(string name);
		static bool checkOutputFile(string name);
};