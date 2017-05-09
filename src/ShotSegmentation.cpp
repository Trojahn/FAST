#include "ShotSegmentation.hpp"

ShotSegmentation::ShotSegmentation(vector<Mat> histograms) {
	this->histograms = histograms;
}

void ShotSegmentation::setGradualThreshold(int value) {
	this->gradualHeuristicThreshold = value;
}

void ShotSegmentation::setSlidingWindowsIntersect(float value) {
	this->swIntersectThreshold = value;
}

void ShotSegmentation::setSlidingWindowsEuclidean(float value) {
	this->swEuclideanThreshold = value;
}

void ShotSegmentation::setLocalSlidingWindowIntersect(float value) {
	this->localSlidingWindowIntersectThreshold = value;
}

void ShotSegmentation::setLocalSlidingWindowEuclidean(float value) {
	this->localSlidingWindowEuclideanThreshold = value;
}

double ShotSegmentation::histogramEuclideanDistance(Mat histogram1, Mat histogram2) {
	double distance = 0.0;
	for(int h = 0; h < 8; h++) {
		for(int s = 0; s < 4; s++) {
			for(int v = 0; v < 4; v++) {
				float val1 = histogram1.at<float>(h,s,v);
				float val2 = histogram2.at<float>(h,s,v);
				distance = distance + sqrt(pow(val1-val2,2.0));
			}
		}
	}
	return distance;
}

double ShotSegmentation::histogramIntersectionDistance(Mat histogram1, Mat histogram2) {
	return compareHist(histogram1, histogram2, CV_COMP_INTERSECT);
}

double ShotSegmentation::calcThresholdIntersection(vector<double> distances, pair<int,int> window) {
	double avg = 0.0;
	
	for(int i = window.first; i < window.second; i++) {
		avg = avg + distances[i];
	}
	avg = avg / (double) (window.second - window.first);

	return avg * this->localSlidingWindowIntersectThreshold;
}

double ShotSegmentation::calcThresholdEuclidean(vector<double> distances, pair<int,int> window) {
	double avg = 0.0;
	
	for(int i = window.first; i < window.second; i++) {
		avg = avg + distances[i];
	}
	avg = avg / (double) (window.second - window.first);
	return avg * this->localSlidingWindowEuclideanThreshold;
}

bool ShotSegmentation::heuristicIntersec(vector<double> distances, int pos, double threshold) {
	for(int i = pos; i <= pos+this->gradualHeuristicThreshold && i < distances.size(); i++) {
		if(distances[i] < threshold) {
			return true;
		}
	}
	return false;
}

bool ShotSegmentation::heuristicEuclidean(vector<double> distances, int pos, double threshold) {
	for(int i = pos; i <= pos+this->gradualHeuristicThreshold && i < distances.size(); i++) {
		if(distances[i] > threshold) {
			return true;
		}
	}
	return false;
}

vector< pair<int,int> > ShotSegmentation::segmentSlidingWindows(vector<double> distEuclidean, vector<double> distIntersec, double thresholdIntersec, double thresholdEuclidean, pair<int,int> window) {
	int actual, gradual, ant;
	vector< pair<int,int> > shots;
	for(ant = window.first, actual = window.first, gradual = 0; actual < window.second; actual++) {
		/* The frame is a transition */
		if(distIntersec[actual] <= thresholdIntersec || distEuclidean[actual] >= thresholdEuclidean) {
			gradual++;
		} else {
			/* The frame is not a transition, but there was a transition before it */
			if (gradual > 0) {
				/* There is a very close frame transition after it*/
				if(heuristicIntersec(distIntersec, actual, thresholdIntersec) || heuristicEuclidean(distEuclidean, actual, thresholdEuclidean)) {
					gradual++;
				} else {
					shots.push_back(make_pair(ant, actual - gradual));
					/* As this frame is not a transition, it is the begining of a new shot. So reset the relevant values */
					ant = actual;
					gradual = 0;
				}
			}
		}
	}
	/* The very last shot of the local window is also a shot so add it. But only if its not a part of a gradual transition... */
	if(shots.size() > 0 && shots[shots.size()-1].second != window.second && gradual == 0) {
		shots.push_back(make_pair(shots[shots.size()-1].second+1,window.second));
	}
	/* If there isn't a single transition, than add the whole window as a shot */
	if(shots.size() == 0) {
		shots.push_back(make_pair(window.first, window.second));
	}

	return shots;
}

vector< pair<int,int> > ShotSegmentation::segment() {
	vector<double> distEuclidean, distIntersec, thresholdIntersec, thresholdEuclidean;	
	vector<double>::iterator maxVal, minVal;
	int previousPos = 0;
	vector<pair<int,int> > slidingWindow;
	pair<int,int> window;
	
	/* Calculate the distances between consecutive histograms*/
	for(int i = 1; i < this->histograms.size(); i++) {
		distEuclidean.push_back(this->histogramEuclideanDistance(this->histograms[i-1],this->histograms[i]));
		distIntersec.push_back(this->histogramIntersectionDistance(this->histograms[i-1],this->histograms[i]));
	}

	/* Find the max and min distances among the histograms */
	maxVal = max_element(distEuclidean.begin(), distEuclidean.end());
	minVal = min_element(distIntersec.begin(), distIntersec.end());
	
	/* Generate the sliding windows and its threholds */
	int i;
	for(i = 0; i < distIntersec.size(); i++) {
		if(distIntersec[i] <= this->swIntersectThreshold || (distIntersec[i] <= (*minVal * 1.5) && distIntersec[i] <= 0.4) ||
		   (distEuclidean[i] >= (*maxVal * 0.85) && distEuclidean[i] >= 0.9) || distEuclidean[i] >= this->swEuclideanThreshold ) {
			   window = make_pair(previousPos,i);
			   thresholdIntersec.push_back(this->calcThresholdIntersection(distIntersec, window));
			   thresholdEuclidean.push_back(this->calcThresholdEuclidean(distEuclidean, window));
			   slidingWindow.push_back(window);
			   previousPos = i+1;
		   }
	}
	if(previousPos < distIntersec.size() && i <= distIntersec.size() && slidingWindow.size() > 0) {
		window = make_pair(previousPos,i);
		thresholdIntersec.push_back(this->calcThresholdIntersection(distIntersec, window));
		thresholdEuclidean.push_back(this->calcThresholdEuclidean(distEuclidean, window));
		slidingWindow.push_back(window);
	} else if (slidingWindow.size() == 0) {
		window = make_pair(0,distIntersec.size()-1);
		thresholdIntersec.push_back(this->calcThresholdIntersection(distIntersec, window));
		thresholdEuclidean.push_back(this->calcThresholdEuclidean(distEuclidean, window));
		slidingWindow.push_back(window);
	}
	
	/* Do the shot segmentation for each sliding window */
	vector< pair<int,int> > shots;
	for(int i = 0; i < slidingWindow.size(); i++) {
		vector< pair<int,int> > temp = this->segmentSlidingWindows(distEuclidean, distIntersec, thresholdIntersec[i], thresholdEuclidean[i], slidingWindow[i]);
		shots.insert(shots.end(), temp.begin(), temp.end());
	}
	
	/* Remove all shots with a duration of 1 frame or less */
	for(int i = 0; i < shots.size(); i++) {
		if(shots[i].second - shots[i].first <= 1) {
			shots.erase(shots.begin() + i);
			i--;
		}
	}
	
	/* Increase all values by 1. The first frame should begin with 1 :) */
	for(int i = 0; i < shots.size(); i++) {
		shots[i] = make_pair(shots[i].first+1, shots[i].second+1);
	}

	return shots;
}