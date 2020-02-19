#include "App.h"
#define CASCADE_CACTUS_SINGLE_PATH   "D:/Development/Visual Studio 2015 Computer Vision/Vision - Dyno Gamer/Vision1/classifier/classifier_cactus_single/cascade.xml" 
#define CASCADE_CACTUS_PATH   "D:/Development/Visual Studio 2015 Computer Vision/Vision - Dyno Gamer/Vision1/classifier/classifier_cactus/cascade.xml" 
#define CASCADE_DYNO_PATH   "D:/Development/Visual Studio 2015 Computer Vision/Vision - Dyno Gamer/Vision1/classifier/classifier_dyno/cascade.xml"

using namespace std;

App::App()
{
}


App::~App()
{
}
 
void App::detect(cv::CascadeClassifier classifier, std::vector<cv::Rect>& object, cv::InputArray image)
{
	classifier.detectMultiScale(image, // input image
		object, // detection results
		1.1, // scale reduction factor
		5, // number of required neighbor 	detections
		0, // flags (not used)
		cv::Size(40, 40), // minimum object size to be detected
		cv::Size(80, 80));
}

void staticdetect(cv::CascadeClassifier classifier, std::vector<cv::Rect>& object, cv::InputArray image)
{
	classifier.detectMultiScale(image, // input image
		object, // detection results
		1.1, // scale reduction factor
		5, // number of required neighbor 	detections
		0, // flags (not used)
		cv::Size(40, 40), // minimum object size to be detected
		cv::Size(90, 90)); // maximum object size to be detected
}

void App::sendInput()
{
	INPUT ip;
	 

	// Set up a generic keyboard event.
	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0; // hardware scan code for key
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;

	// Press  2 key
	ip.ki.wVk = 0x20; // virtual-key code for the "SPACE BAR" key
	ip.ki.dwFlags = 0; // 0 for key press
	SendInput(1, &ip, sizeof(INPUT));

	// Release the "A" key
	ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
	SendInput(1, &ip, sizeof(INPUT));

}

int App::run()
{
	points.push_back({});
	bg_ = cv::imread("bg.jpg");

	cv::VideoCapture cap(0);

	//initWindowDialogs(); 

	 
	cv::CascadeClassifier cascadeMyDyno;
	if (!cascadeMyDyno.load(CASCADE_DYNO_PATH)) {
		std::cout << "Error when loading the cascade CASCADE_DYNO_PATH!"
			<< std::endl;
		return -1;
	}
	 cv::CascadeClassifier cascadeCactus;
	if (!cascadeCactus.load(CASCADE_CACTUS_PATH)) {
		std::cout << "Error when loading the cascade CASCADE_CACTUS_PATH!"
			<< std::endl;
		return -1;
	} 
	cv::CascadeClassifier cascadeCactusSingle;
	if (!cascadeCactusSingle.load(CASCADE_CACTUS_SINGLE_PATH)) {
		std::cout << "Error when loading the cascade CASCADE_CACTUS_PATH!"
			<< std::endl;
		return -1;
	}

	std::cout << "FILE LOADED" << std::endl;

	std::vector<cv::Rect> MyDynoDetections;
	std::vector<cv::Rect> cactusDetections;
	std::vector<cv::Rect> cactusSingleDetections;

	while (running) {

		cap >> mainPicture;
		rgbPicture = mainPicture;

		/*mirror*/
		//cv::flip(mainPicture, mainPicture, 2); 
	 

		 detect(cascadeMyDyno, MyDynoDetections, mainPicture);
		 detect(cascadeCactus, cactusDetections, mainPicture);
		 detect(cascadeCactusSingle, cactusSingleDetections, mainPicture);

		//if (MyDynoDetections.size() > 0) {
		//	//sendInput();
		//}  

		//if (cactusDetections.size() > 0) {
		//	//sendInput();
		//}
		//   
		//if (cactusSingleDetections.size() > 0) {
		//	//sendInput();
		//} 

		 drawDetection(cactusDetections, rgbPicture, "CACTUS-TWIN", cv::Scalar(0, 0, 255));
		 drawDetection(MyDynoDetections, rgbPicture, "DYNO", cv::Scalar(0, 0, 0));
		 drawDetection(cactusSingleDetections, rgbPicture, "CACTUS-SINGLE", cv::Scalar(0, 0, 255));



		/*
			Show Windows...........................................
		*/

		imshow(ORIGINAL_WINDOW, rgbPicture);
		 

		/*
		process key input
		*/
		cv::waitKey(1);
	}
	/*cv::waitKey(0);*/
	getchar();
	return 0;
}







int App::createBg()
{
	 
	return 0;
}

int App::drawDetection(std::vector<cv::Rect> detections, cv::Mat pict, cv::String name, cv::Scalar color)
{
	for (int i = 0; i < detections.size(); i++) {
		cv::Rect detectionRect = detections[i];

		std::string text = name+" - " + std::to_string(detectionRect.x) + "." + std::to_string(detectionRect.y);
		text += "|" + std::to_string(detectionRect.width) + "x" + std::to_string(detectionRect.height);

		cv::putText(pict, text, cv::Point(detectionRect.x, detectionRect.y),
			1, 1.50, color, 2);
		cv::rectangle(pict, detectionRect,
			color, 2);
	}
	return 0;
}


