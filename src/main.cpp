#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "Utils.hpp"
#include "ShotSegmentation.hpp"
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <utility>
#include <algorithm>

using namespace std;
using namespace cv;

vector<Mat> extractVideoHistograms(string videoPath) {
	vector<Mat> histograms;
	Mat frame;
	VideoCapture capture(videoPath);
	
	while(true) {
		bool temp = capture.read(frame);
		if(!temp) {
			break;
		}
		cvtColor(frame,frame,CV_BGR2HSV);
		histograms.push_back(Utils::extractHistogram(frame));
	}
	frame.release();
	capture.release();
	return histograms;	
}

int main(int argc, char* argv[]) {
	if(argc != 3) {
		cout << "Incorrect parameter count. Please, use './FAST <videoFilePath> <outputFilePath>'" << endl;
		return 1;
	}
	string videoPath = string(argv[1]);
	string outputPath = string(argv[2]);
	if(!Utils::checkFile(videoPath)) {
		cout << "The videoFilePath seems to be invalid or cannot be read" << endl;
		return 1;
	}
	if(!Utils::checkOutputFile(outputPath)) {
		cout << "The outputFilePath seems to be invalid or cannot be written" << endl;
		return 1;
	}
	vector<Mat> histograms = extractVideoHistograms(videoPath);
	
	ShotSegmentation ss(histograms, 3, 0.5, 9);
	vector< pair<int,int> > shots = ss.segment();
	
	Utils::writeOutputFile(outputPath, shots);
	return 0;
}