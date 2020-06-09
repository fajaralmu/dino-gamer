How to play:
Requirement:

-Microsoft Visual Studio 2015
-OpenCV packages
-WebCam

Usage:

-Open Google Chrome and type "chrome://dino"
-Put the webcam in front of the game screen
-Run as debug
-Adjust the camera distance so it can detect the objects precisely

HOW TO TRAIN DATA:

requirement:

1. positive images and file containing list of positive images infos
2. negative images and file containing list of negative images infos

step:

1. create sample
./opencv_createsamplesd -info  "directory\to\positives\files\pos.txt " -vec "directory\to\positives\files\output\pos.vec" -w "24 -h 24 -num 23

2. train data
./opencv_traincascaded -data classifier -vec "directory\to\positives\files\output\pos.vec" -bg "directory\to\positives\files\neg.txt" -numPos 18 -numNeg 30 -numStages 20 -minHitRate 0.95 -maxFalseAlarmRate 0.5 -w 24 -h 24
