# Introduction
FAST (**FA**st **S**hot segmen**T**ation) is a simple and fast video shot segmentation application. Just provide a input video and enjoy!

# Requirements
*   A C++11 compiler, like g++
*   A functional OpenCV installation (version 4.x and above, built using 'OPENCV_GENERATE_PKGCONFIG=YES')
*   An appropriate set of CODECs


# Arguments
	./FAST videoFilePath outputFilePath [ght [hiv [hev [eav [iav]]]]]
The optional parameters are :
- ght : Gradual heuristic strenght. Merge consecutive transitions found with distance up to the specified value. Zero disables it. \[0 ~ N]\ (Default: 3).
- hiv : The histogram intersection value which identifies a sliding windows transition. Also a shot transtion. \[0.01 ~ 0.99\] (Default: 0.25).
- hev : The histogram euclidean distance value which identifies a sliding windows transition. Also a shot transtion. \[0.01 ~ 1.99\] (Default: 1.5).
- eav : A multiplier value applied to the average histogram intersection value within each local sliding windows to detect subtle transitions. \[0.01 ~ N]\ (Default: 0.5)
- isv : A multiplier value applied to the average histogram euclidean distance value within each local sliding windows to detect subtle transitions. \[0.01 ~ N\] (Default: 9.00).


# Output
FAST will save it's output into a CSV file (the second provided parameter). In this file, each line corresponds to a detected shot and each number to the corresponding *start,end* of the shot. The first frame is represented by '1'.

If FAST detects a gradual transition such as swipe, fade in/out or other transition effect, the corresponding frames are discarted of the output CSV. For example, the following snippet...

	87213,87311
	87339,87390
	
...represents two shots (87213 to 87311 and 87339 to 87390) and a gradual transition (87312 to 87338).

The last video frame can be ignored in some cases.

# Example
	$ ./FAST video.avi shots.csv
	$ cat shots.csv
		1,240
		241,260
		272,290
		291,313
		320,325
		326,700

# Publications
FAST was mentioned, described or used in the following publications/software:
	
Trojahn, T. H., 2014. *Automatic video scene segmentation based on shot coherence*. M.S. thesis. Universidade de São Paulo. Avaliable in http://www.teses.usp.br/teses/disponiveis/55/55134/tde-20052014-152446/. (In portuguese).

# About
FAST was developed by Tiago H. Trojahn under the coordination of Rudinei Goularte.



