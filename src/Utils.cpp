#include "Utils.hpp"

std::mutex Utils::mutex;

void Utils::extractHistogram(Mat frame, int num, vector< pair<int, Mat> > &hTemp){
	cvtColor(frame,frame,CV_BGR2HSV);
		
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
	frame.release();
	
	Utils::mutex.lock();
	hTemp.push_back(make_pair(num,histogram));
	Utils::mutex.unlock();
}

void Utils::writeOutputFile(string outFile, vector< pair<int,int> > shots) {
	fstream file (outFile, ios::out);
	for(int i = 0; i < shots.size(); i++) {
		file << shots[i].first << "," << shots[i].second << endl;
	}
	file.close();
}

bool Utils::checkFile(string name) {
	fstream file(name, ios::in);
	if(file.is_open()) {
		file.close();
		return true;	
	}
	return false;
}

bool Utils::checkOutputFile(string name) {
	fstream file(name, ios::out);
	if(file.is_open()) {
		file.close();
		return true;	
	}
	return false;
}

bool Utils::pairCompare(const pair<int, Mat> &fElem, const pair<int, Mat> &sElem) {
	return fElem.first < sElem.first;
}

vector<Mat> Utils::extractVideoHistograms(string videoPath) {
	vector< pair<int, Mat> > hTemp;
	vector<Mat> histograms;
	// Each threads will be real fast...
	unsigned nThreads = thread::hardware_concurrency() * 100;
	vector<thread> pool;

	try {	
		Mat frame;
		VideoCapture capture(videoPath);
		
		for(int num = 0; capture.read(frame); num++) {
			if(pool.size() > nThreads) {
				for(auto &t : pool) {
					t.join();
				}
				pool.clear();
			}
			Mat fTemp;
			frame.copyTo(fTemp);
			pool.push_back(thread(&Utils::extractHistogram, fTemp, num, std::ref(hTemp)));	
		}
		
		for(auto &t : pool) {
			t.join();
		}
		pool.clear();
		frame.release();
		capture.release();		
	} catch(exception &e) {
		cout << "The video file is corrupt or of an unsupported format" << endl;
	}

	std::sort(hTemp.begin(), hTemp.end(), Utils::pairCompare);
		
	for(pair<int, Mat> t : hTemp) {
		histograms.push_back(t.second);
	}

	return histograms;	
}