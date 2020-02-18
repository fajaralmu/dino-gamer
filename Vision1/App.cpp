#include "App.h"
#define CASCADE_SPOON_PATH   "classifier/classifier_spoon/cascade.xml"
#define CASCADE_BALSEM_PATH   "classifier/classifier_balsem/cascade.xml"
#define CASCADE_PERFUME_PATH   "classifier/classifier_perfume/cascade.xml"

#define CASCADE_DYNO_PATH   "classifier/classifier_dyno/cascade.xml"

using namespace std;

App::App()
{
}


App::~App()
{
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

	 
	cv::CascadeClassifier cascadeBalsem;
	if (!cascadeBalsem.load(CASCADE_DYNO_PATH)) {
		std::cout << "Error when loading the cascade classfier!"
			<< std::endl;
		return -1;
	}
	/*cv::CascadeClassifier cascadePerfume;
	if (!cascadePerfume.load(CASCADE_PERFUME_PATH)) {
		std::cout << "Error when loading the cascade classfier!"
			<< std::endl;
		return -1;
	}*/

	std::cout << "FILE LOADED" << std::endl;

	while (running) {

		cap >> mainPicture;
		rgbPicture = mainPicture;

		/*mirror*/
		//cv::flip(mainPicture, mainPicture, 2); 
		std::vector<cv::Rect> balsemDetections;

		std::vector<cv::Rect> perfumeDetections;

		/*
			balsem
		*/
		cascadeBalsem.detectMultiScale(mainPicture, // input image
			balsemDetections, // detection results
			1.1, // scale reduction factor
			10, // number of required neighbor 	detections
			0, // flags (not used)
			cv::Size(50,50), // minimum object size to be detected
			cv::Size(400, 400)); // maximum object size to be detected

		if (balsemDetections.size() > 0) {
			sendInput();
		}

		drawDetection(balsemDetections, rgbPicture, "DYNO", cv::Scalar(0,255,255));
		 

		imshow(ORIGINAL_WINDOW, rgbPicture);
		 

		/*
		process key input
		*/
		cv::waitKey(1);
	}
	/*cv::waitKey(0);*/
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


