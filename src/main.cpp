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

int main(int argc, char* argv[]) {
	if(argc < 3) {
		cout << "./FAST videoFilePath outputFilePath [ght [hiv [hev [eav [iav]]]]]" << endl;
		cout << "Options:" << endl;
		cout << "ght : Gradual heuristic strenght. Merge consecutive transitions found with distance up to the specified value. Zero disables it. [0 ~ N] (Default: 3)" << endl;
		cout << "hiv : The histogram intersection value which identifies a sliding windows transition. Also a shot transtion. [0.01 ~ 0.99] (Default: 0.25)." << endl;
		cout << "hev : The histogram euclidean distance value which identifies a sliding windows transition. Also a shot transtion. [0.01 ~ 1.99] (Default: 1.5)." << endl;
		cout << "eav : A multiplier value applied to the average histogram intersection value within each local sliding windows to detect subtle transitions. [0.01 ~ N] (Default: 0.5)" << endl;
		cout << "isv : A multiplier value applied to the average histogram euclidean distance value within each local sliding windows to detect subtle transitions. [0.01 ~ N] (Default: 9.00)" << endl;
		return 0;
	}
	string videoPath = string(argv[1]);
	string outputPath = string(argv[2]);
	if(!Utils::checkFile(videoPath)) {
		cout << "The videoFilePath seems to be invalid or cannot be read" << endl;
		return 1;
	}
	if(Utils::checkFile(outputPath)) {		
		while(true) {
			string in;
			cout << "File '" << outputPath << "' already exists. Overwrite [y/N]? ";
			getline(std::cin,in);
			if(in == "Y" || in == "y") {
				break;
			}
			if(in == "" || in == "N" || in == "n") {
				return 0;
			}
		}
	} else {
		if(!Utils::checkOutputFile(outputPath)) {
			cout << "The outputFilePath seems to be invalid or cannot be written" << endl;
			return 1;
		} else {
			std::remove(outputPath.c_str());
		}
	}
	
	int gradualHeuristic = 3;	
	float swIntersection = 0.25;
	float swEuclidean = 1.5;
	float localSWIntersection = 0.5;
	float localSWEuclidean = 9.0;	
	
	
	try {
		try {
			/* ght heuristics value */
			if(argc > 3) {
				gradualHeuristic = stoi(string(argv[3]));
				if(gradualHeuristic < 0) {
					gradualHeuristic = 0;
				}
			}

			/* hiv value */	
			if(argc > 4) {
				swIntersection = stof(string(argv[4]));
				if(swIntersection < 0.01) {
					swIntersection = 0.01;
				} else {
					if (swIntersection > 0.99) {
						swIntersection = 0.99;
					}
				}
			}
			
			/* hev value */
			if(argc > 5) {
				swEuclidean = stof(string(argv[5]));
				if(swEuclidean < 0.01) {
					swEuclidean = 0.01;
				} else {
					if(swEuclidean >= 1.99) {
						swEuclidean = 1.99;
					}
				}
			}

			
			/* eav value */
			if(argc > 6) {
				localSWIntersection = stof(string(argv[6]));
				if(localSWIntersection < 0.01) {
					localSWIntersection = 0.01;
				}
			}
			/* iav value */
			if(argc > 7) {
				localSWEuclidean = stof(string(argv[7]));
				if(localSWEuclidean < 0.01) {
					localSWEuclidean = 0.01;
				}
			}				
			
		} catch(out_of_range& oor) {
			cout << "Out of range error!" << endl;
			throw;
		} catch(invalid_argument& ia) {
			cout << "Invalid argument!" << endl;
			throw;
		}
	} catch (exception &e) {
		cout << "Some of the options were invalid. Exiting." << endl;
		return 1;
	}	



	// Extract the video histograms
	vector<Mat> histograms = Utils::extractVideoHistograms(videoPath);
	if(histograms.size() > 0) {
		ShotSegmentation ss(histograms);
	
		// Seting the needed thresholds.
		ss.setGradualThreshold(gradualHeuristic);
		ss.setSlidingWindowsIntersect(swIntersection);
		ss.setSlidingWindowsEuclidean(swEuclidean);
		ss.setLocalSlidingWindowIntersect(localSWIntersection);
		ss.setLocalSlidingWindowEuclidean(localSWEuclidean);
			
		// Performing the shotSegmentation
		vector<pair<int,int>> shots = ss.segment();
		if(shots.size() > 0) {
			Utils::writeOutputFile(outputPath, shots);	
		}		
	}

	return 0;
}
