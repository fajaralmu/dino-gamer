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
	cv::Mat rgbPicture;
	bool drawingMode = false;
	bool running = true;

	App();
	~App();


	void detect(cv::CascadeClassifier classifier, std::vector<cv::Rect> &object, cv::InputArray image);
	 
	void sendInput();
	int run(); 
	int createBg();
	int drawDetection(std::vector<cv::Rect> detections, cv::Mat pict, cv::String name, cv::Scalar color);
};

