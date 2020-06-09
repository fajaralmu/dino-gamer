#pragma once
#include <iostream>
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\core\core.hpp"
#include "opencv2\opencv.hpp"
#include "opencv2\ml\ml.hpp" 
#include <conio.h>
#include <Windows.h>
#include <stdlib.h>
#include <thread>
#include "Util.hpp"

class App
{
public:
	//contour
	std::vector<std::vector<cv::Point>> contours;
	// testing the convex hull
	std::vector<std::vector<cv::Point> >hull;

	/*
	for storing polygons
	*/
	std::vector<std::vector<cv::Point>> points;
	int polygonCount = 0;
	int x = 0, y = 0;

	/*
	Treshlods
	*/
	int threshold_value = 57;
	int threshold_type = 0;
	int const max_value = 255;
	int const max_type = 4;
	int const max_BINARY_value = 255;
	cv::Mat tresholded;
	cv::Mat imageGrayScale, bg, bg_;
	cv::Mat edge;

	/*
	window names
	*/
	const char* TRESHOLD_WINDOW = "Threshold Demo";
	const char * HSV_TRACK_WINDOW = "HRV TRACK";
	const char * ORIGINAL_WINDOW = "Main Window";

	char* trackbar_type = "Type: \n 0: Binary \n 1: Binary Inverted \n 2: Truncate \n 3: To Zero \n 4: To Zero Inverted";
	char* trackbar_value = "Value";

	//hsv
	cv::Mat hsvPicture, hsv_out;
	int hueMin = 109, valMin = 121, satMin = 47;
	int hueMax = 167, valMax = 250, satMax = 111;

	cv::Mat mainPicture;
	cv::Mat outPicture;
	bool drawingMode = false;
	bool running = true;

	cv::CascadeClassifier cascadeMyDyno;
	cv::CascadeClassifier cascadeCactus;
	cv::CascadeClassifier cascadeCactusSingle;
	cv::CascadeClassifier cascadeCactusSmall;
	cv::CascadeClassifier cascadeCactusTriple;
	cv::CascadeClassifier cascadeEnemy;

	std::vector<cv::Rect>  dynoDetections;
	std::vector<cv::Rect>  cactusDetections;
	std::vector<cv::Rect>  cactusSingleDetections;
	std::vector<cv::Rect>  cactusSmallDetections;
	std::vector<cv::Rect>  cactusTripleDetections;
	std::vector<cv::Rect>  enemyDetections;

	cv::Rect dynoPoint;
	cv::Rect enemyPoint;
	bool dynoDetected = false;

	App();
	~App();


	void detect(cv::CascadeClassifier  classifier, std::vector<cv::Rect> &object, cv::InputArray image);
	void detection(cv::CascadeClassifier classifier, std::vector<cv::Rect> &object, cv::InputArray image);

	void setDynoPoint();
	void setNearestEnemyPoint();
	void drawDistance();

	void sendInput();
	int loadFiles();
	int run();
	int createBg();
	int drawDetection(std::vector<cv::Rect> * detections,  cv::String name, cv::Scalar color);
};

