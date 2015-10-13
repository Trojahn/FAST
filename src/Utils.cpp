#include "Utils.hpp"

Mat Utils::extractHistogram(Mat frame){
	float Hsize[] = {0,180};
	float Ssize[] = {0,256};
	float Vsize[] = {0,256};
	const float* space[] = {Hsize, Ssize, Vsize};
	int channels[] = {0,1,2};
	int Hbins = 8;
	int Sbins = 4;
	int Vbins = 4;
	int histogramBins[] = {Hbins, Sbins, Vbins};
	Mat histogram;

	calcHist(&frame,1,channels,Mat(),histogram,3,histogramBins,space);
	histogram = histogram / cv::sum(histogram)[0];

	return histogram;
}

void Utils::writeOutputFile(string outFile, vector< pair<int,int> > shots) {
	ofstream file (outFile.c_str());
	for(int i = 0; i < shots.size(); i++) {
		file << shots[i].first << "," << shots[i].second << endl;
	}
	file.close();
}

bool Utils::checkFile(string name) {
  if(FILE *file = fopen(name.c_str(),"r")) {
		fclose(file);
		return true;
	}
	return false;
}

bool Utils::checkOutputFile(string name) {
	if(FILE *file = fopen(name.c_str(),"w")) {
		fclose(file);
		return true;
	}
	return false;	
}