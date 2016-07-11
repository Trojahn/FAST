#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <utility>
#include <algorithm>
#include <iostream>
#include <cmath>

using namespace std;
using namespace cv;

class ShotSegmentation {
	private:
		vector<Mat> histograms;
		double intersectionThreshold;
		double euclideanThreshold;
		int heuristicThreshold;
		/* Methods */
		double histogramEuclideanDistance(Mat histogram1, Mat histogram2);
		double histogramIntersectionDistance(Mat histogram1, Mat histogram2);
		double calcThresholdIntersection(vector<double> distances, pair<int,int> window);
		double calcThresholdEuclidean(vector<double> distances, pair<int,int> window);
		bool heuristicEuclidean(vector<double> distances, int pos, double threshold);
		bool heuristicIntersec(vector<double> distances, int pos, double threshold);
		vector< pair<int,int> > segmentSlidingWindows(vector<double> distEuclidean, vector<double> distIntersec,
		double thresholdIntersec, double thresholdEuclidean, pair<int,int> window);
		

	public:
		ShotSegmentation(vector<Mat> histograms, int heuristicThreshold, double intersectionThreshold, double euclideanThreshold);
		vector< pair<int,int> > segment();
};