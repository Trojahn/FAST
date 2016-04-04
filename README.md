# Introduction
FAST (**FA**st **S**hot segmen**T**ation) is a simple and fast video shot segmentation application. Just provide a input video and enjoy!

# Requirements
*   A C++11 compiler, like g++
*   A functional OpenCV installation (version 2.4.x and above)
*   An appropriate set of CODECs

# Parameters
1.  The video which will be segmented.
2.  A file path to the CSV output which describes the shots found.

# Output
FAST will save it's output into a CSV file (the second provided parameter). In this file, each line corresponds to a detected shot and each number to the corresponding *start,end* of the shot. The first frame is represented by '1'.

If FAST detects a gradual transition such swipe, fade in/out or other transition effect, the corresponding frames are discarted of the output CSV. For example, the following snippet...

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
FAST was mentioned, described or used in the following publications:
	
Trojahn, T. H., 2014. *Automatic scenes video segmentation based on shot coherence*. M.S. thesis. Universidade de São Paulo. Avaliable in http://www.teses.usp.br/teses/disponiveis/55/55134/tde-20052014-152446/. (In portuguese).

# Authors
FAST was developed by Tiago H. Trojahn under the coordination of Rudinei Goularte



