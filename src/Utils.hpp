#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <cmath>

using namespace std;
using namespace cv;

class Utils {
	private:
		static void extractHistogram(Mat frame, int num, vector< pair<int, Mat> > &hTemp);
		static std::mutex mutex;
	
	public:		
		static void writeOutputFile(string outFile, vector< pair<int,int> > shots);
		static bool checkFile(string name);
		static bool checkOutputFile(string name);
		static vector<Mat> extractVideoHistograms(string videoPath);
		static bool pairCompare(const pair<int, Mat> &fElem, const pair<int, Mat> &sElem);
};