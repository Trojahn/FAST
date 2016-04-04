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

bool Utils::pairCompare(const pair<int, Mat> &fElem, const pair<int, Mat> &sElem) {
	return fElem.first < sElem.first;
}

vector<Mat> Utils::extractVideoHistograms(string videoPath) {
	vector< pair<int, Mat> > hTemp;
	unsigned nThreads = thread::hardware_concurrency();
	vector<thread> pool;
		
	int num = 0;
	Mat frame;
	try {	
		VideoCapture capture(videoPath);
	
		while(true) {
			bool temp = capture.read(frame);
			if(!temp) {
				break;
			}
			if(pool.size() >= nThreads) {
				for(int i = 0; i < pool.size(); i++) {
					pool[i].join();
				}
				pool.clear();
			}
			Mat fTemp;
			frame.copyTo(fTemp);
			pool.push_back(thread(&Utils::extractHistogram, fTemp, num, std::ref(hTemp)));			
			num++;
		}
		for(int i = 0; i < pool.size(); i++) {
			pool[i].join();
		}
		pool.clear();
		frame.release();
		capture.release();		
	} catch(exception &e) {
		cout << "The video file is corrupt or of an unsupported format" << endl;
		exit(1);
	}

	std::sort(hTemp.begin(), hTemp.end(), Utils::pairCompare);
	
	vector<Mat> histograms;
	for(pair<int, Mat> t : hTemp) {
		histograms.push_back(t.second);
	}

	return histograms;	
}