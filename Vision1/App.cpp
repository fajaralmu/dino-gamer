#include "App.h"
//------------------------------------------------------------
//        Change to the classifiers location
//------------------------------------------------------------
#define CASCADE_CACTUS_SINGLE_PATH   "D:/Development/Visual Studio 2015 Computer Vision/Vision - Dyno Gamer/Vision1/classifier/classifier_cactus_single/cascade.xml"  
#define CASCADE_ALL_CACTUS_PATH   "D:/Development/Visual Studio 2015 Computer Vision/Vision - Dyno Gamer/Vision1/classifier/classifier_all_cactus/cascade.xml" 
#define CASCADE_CACTUS_PATH   "D:/Development/Visual Studio 2015 Computer Vision/Vision - Dyno Gamer/Vision1/classifier/classifier_cactus/cascade.xml" 
#define CASCADE_DYNO_PATH   "D:/Development/Visual Studio 2015 Computer Vision/Vision - Dyno Gamer/Vision1/classifier/classifier_dyno/cascade.xml"
#define CASCADE_CACTUS_TRIPLE_PATH   "D:/Development/Visual Studio 2015 Computer Vision/Vision - Dyno Gamer/Vision1/classifier/classifier_cactus_triple/cascade.xml"
#define CASCADE_ENEMY_PATH   "D:/Development/Visual Studio 2015 Computer Vision/Vision - Dyno Gamer/Vision1/classifier/classifier_enemy/cascade.xml"

using namespace std;

bool isEmpty(cv::Rect rect) {

	return rect.x == 0 &&
		rect.y == 0 &&
		rect.width == 0 &&
		rect.height == 0;
}

cv::Rect  getFarestXPos(std::vector<cv::Rect> * detections) {
	int x = 0;
	cv::Rect  max;
	max = cv::Rect(0, 0, 0, 0);
	for (int i = 0; i < detections->size(); i++) {
		cv::Rect  point = detections->at(i);
		if (point.x > x) {
			max = point;
			x = point.x;
		}
	}
	/*for each (cv::Rect * point in detections) {
		if (point->x > x) {
			max = point;
			x = point->x;
		}
	}*/
	return max;
}

cv::Rect  getNearestXPos(std::vector<cv::Rect> * detections) {
	try {
		cv::Rect   max = getFarestXPos(detections);
		int x = max.x;

		cv::Rect  min = max;
		for (int i = 0; i < detections->size(); i++) {
			cv::Rect  point = detections->at(i);
			if (point.x < x) {
				min = point;
				x = point.x;
			}
		}
		/*for each (cv::Rect * point in detections)
		{
			if (point->x < x) {
				min = point;
				x = point->x;
			}
		}*/
		return min;
	}
	catch (exception e) {
		/*
		error catched
		*/
		std::cout << "ERROR BRO: " << e.what() << std::endl;
		return cv::Rect(0, 0, 0, 0);
	}
}

App::App()
{
}


App::~App()
{
}

void App::detect(cv::CascadeClassifier * classifier, std::vector<cv::Rect> &object, cv::InputArray image)
{
	try {
		classifier->detectMultiScale(image, // input image
			object, // detection results
			1.5, // scale reduction factor
			3, // number of required neighbor 	detections
			0, // flags (not used)
			cv::Size(30, 30)  // minimum object size to be detected
			, cv::Size(180, 180) //max size
			);
	}
	catch (exception e) {
		std::cout << "ERROR BRO: " << e.what() << std::endl;
	}
}

//void preDetect(cv::CascadeClassifier classifier, std::vector<cv::Rect>& object, cv::InputArray image) {
//
//}

void App::detection(cv::CascadeClassifier classifier, std::vector<cv::Rect>& object, cv::InputArray image) {
	//std::vector<cv::Rect> fbject;
	//std::thread thread(detect, classifier, &fbject, image);

}

void App::setDynoPoint()
{
	try {
		if (dynoDetections.size() < 1) {
			//dynoDetected = false;
			//sendInput();
		}
		else {
			dynoDetected = true;
			dynoPoint = dynoDetections.at(0);
			//	cout << "dino detected" << endl;
		}
	}
	catch (exception e) {
		/*
			error catched
		*/
		std::cout << "ERROR BRO: " << e.what() << std::endl;
	}
}

void App::setNearestEnemyPoint()
{
	if (dynoDetected == false) {
		return;
	}

	int currentXPos = outPicture.cols;

	std::vector<cv::Rect> enemies = std::vector<cv::Rect>();

	cv::Rect cactusSingle = getNearestXPos(&cactusSingleDetections);
	cv::Rect cactusTriple = getNearestXPos(&cactusTripleDetections);
	cv::Rect cactusTwin = getNearestXPos(&cactusDetections);
	cv::Rect bird = getNearestXPos(&enemyDetections);

	if (!isEmpty(cactusSingle) && cactusSingle.x > dynoPoint.x) {
		enemies.push_back(cactusSingle);
	}
	if (!isEmpty(cactusTriple) && cactusTriple.x > dynoPoint.x) {
		enemies.push_back(cactusTriple);
	}
	if (!isEmpty(cactusTwin) && cactusTwin.x > dynoPoint.x) {
		enemies.push_back(cactusTwin);
	}
	if (!isEmpty(bird) && bird.x > dynoPoint.x) {
		enemies.push_back(bird);
	}
	//if (enemies.size() > 0)
	//	cout << "ENEMY SIZE: " << enemies.size() << endl;

	enemyPoint = getNearestXPos(&enemies);

}

void App::drawDistance()
{
	if (dynoDetected == false) {
		return;
	}
	if (isEmpty(enemyPoint)) {
		return;
	}
	int enemyPointX = enemyPoint.x + enemyPoint.width / 2;
	int dynoPointX = dynoPoint.x + dynoPoint.width / 2;
	int difference = enemyPointX - dynoPointX;
	int middlePointX = dynoPointX + difference / 2;

	cv::Point dyno = cv::Point(dynoPointX, dynoPoint.y);
	cv::Point enemy = cv::Point(enemyPointX, dynoPoint.y);

	cv::line(outPicture, dyno, enemy, cv::Scalar(10, 200, 0), 2);
	cv::line(outPicture, cv::Point(enemyPointX, dynoPoint.y), cv::Point(enemyPointX, enemyPoint.y), cv::Scalar(0, 0, 0), 1);
	cv::putText(outPicture, std::to_string(difference), cv::Point(middlePointX, dynoPoint.y), 1, 2, cv::Scalar(0, 0, 0), 2);

	cout << "difference: " << difference << endl;

	if (difference > 0 && difference < 300) {
		sendInput();
	}

}

void static detect(App * app, cv::CascadeClassifier * classifier, std::vector<cv::Rect>& object, cv::InputArray image)
{
	try {
		//app->detect(classifier, object, image);
	}
	catch (exception e) {
		std::cout << e.what() << std::endl;
	}

}

void App::sendInput()
{
	INPUT input;


	// Set up a generic keyboard event.
	input.type = INPUT_KEYBOARD;
	input.ki.wScan = 0; // hardware scan code for key
	input.ki.time = 0;
	input.ki.dwExtraInfo = 0;

	// Press  2 key
	input.ki.wVk = 0x20; // virtual-key code for the "SPACE BAR" key
	input.ki.dwFlags = 0; // 0 for key press
	SendInput(1, &input, sizeof(INPUT));

	// Release the "A" key
	input.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
	//SendInput(1, &ip, sizeof(INPUT));

	// Current date/time based on current system

	cout << time(0) << "SPACE BAR PRESSED" << endl;

}

int App::loadFiles() {
	if (!cascadeMyDyno.load(CASCADE_DYNO_PATH)) {
		std::cout << "Error when loading the cascade CASCADE_DYNO_PATH!" << std::endl;
		return -1;
	}

	if (!cascadeCactus.load(CASCADE_CACTUS_PATH)) {
		std::cout << "Error when loading the cascade CASCADE_CACTUS_PATH!" << std::endl;
		return -1;
	}

	if (!cascadeCactusSingle.load(CASCADE_CACTUS_SINGLE_PATH)) {
		std::cout << "Error when loading the cascade CASCADE_CACTUS_PATH!" << std::endl;
		return -1;
	}


	if (!cascadeCactusTriple.load(CASCADE_CACTUS_TRIPLE_PATH)) {
		std::cout << "Error when loading the cascade CASCADE_CACTUS_TRIPLE_PATH!" << std::endl;
		return -1;
	}

	if (!cascadeEnemy.load(CASCADE_ENEMY_PATH)) {
		std::cout << "Error when loading the cascade CASCADE_ENEMY_PATH!" << std::endl;
		return -1;
	}
	return 0;
}

void detectDino(App * app) {
	app->detect(&app->cascadeMyDyno, app->dynoDetections, app->mainPicture);
}

void detectCactus1(App* app) { 
	app->detect(&app->cascadeCactus, app->cactusDetections, app->mainPicture); 
}


void detectCactus2(App* app) {
	app->detect(&app->cascadeCactusSingle, app->cactusSingleDetections, app->mainPicture); 
}

void detectCactus3(App * app) {
	app->detect(&app->cascadeCactusTriple, app->cactusTripleDetections, app->mainPicture); 
}

void detectEnemy(App * app) {
	app->detect(&app->cascadeEnemy, app->enemyDetections, app->mainPicture);
}

int App::run()
{
	points.push_back({});


	//initWindowDialogs();  

	int loadResult = this->loadFiles();
	if (loadResult != 0) {

		return loadResult;
	}

	std::cout << "FILE LOADED" << std::endl;

	cv::setNumThreads(5);
	bg_ = cv::imread("bg.jpg");
	cv::VideoCapture cap(0);

	while (running) {

		try {

			cap >> mainPicture;
			outPicture = mainPicture;

			/*mirror*/
			//cv::flip(mainPicture, mainPicture, 2);  
			/*std::thread thread1(&App::detect, cascadeMyDyno, dynoDetections, mainPicture);
			std::thread thread2(&App::detect, cascadeMyDyno, dynoDetections, mainPicture);
			std::thread thread3(&App::detect, cascadeCactus, cactusDetections, mainPicture);
			std::thread thread4(&App::detect, cascadeCactusSingle, cactusSingleDetections, mainPicture);
			std::thread thread5(&App::detect, cascadeCactusTriple, cactusTripleDetections, mainPicture);
			std::thread thread6(&App::detect, cascadeEnemy, enemyDetections, mainPicture);*/
			std::thread thread1(detectDino,this);
			std::thread thread2(detectCactus1, this);
			std::thread thread3(detectCactus2, this);
			std::thread thread4(detectCactus3, this);
			std::thread thread5(detectEnemy, this); 
			//this->detect(&cascadeMyDyno, dynoDetections, mainPicture);
			/*this->detect(&cascadeMyDyno, dynoDetections, mainPicture);
			this->detect(&cascadeCactus, cactusDetections, mainPicture);
			this->detect(&cascadeCactusSingle, cactusSingleDetections, mainPicture);
			this->detect(&cascadeCactusTriple, cactusTripleDetections, mainPicture);
			this->detect(&cascadeEnemy, enemyDetections, mainPicture);*/
			 thread1.join();
			thread2.join();
			thread3.join();
			thread4.join();
			thread5.join();
			 
			 

			setDynoPoint();
			setNearestEnemyPoint();
			drawDistance();

			drawDetection(&cactusDetections, "CACTUS-TWIN", cv::Scalar(0, 0, 255));
			drawDetection(&dynoDetections, "DYNO", cv::Scalar(255, 0, 0));
			drawDetection(&cactusSingleDetections, "CACTUS-SINGLE", cv::Scalar(0, 0, 255));
			drawDetection(&cactusTripleDetections, "CACTUS-TRIPLE", cv::Scalar(0, 0, 255));
			drawDetection(&enemyDetections, "ENEMY", cv::Scalar(0, 0, 255));


			/*
				Show Windows...........................................
			*/
			imshow(ORIGINAL_WINDOW, outPicture);

			/*
			process key input
			*/
			cv::waitKey(1);

		}
		catch (exception e) {
			std::cout << e.what() << std::endl;
		}
	}
	/*cv::waitKey(0);*/
	getchar();
	std::cout << "end" << std::endl;
	return 0;
}


int App::createBg()
{

	return 0;
}

int App::drawDetection(std::vector<cv::Rect> * detections, cv::String name, cv::Scalar color)
{
	for (int i = 0; i < detections->size(); i++) {
		cv::Rect detectionRect = detections->at(i);

		std::string text = name + " - " + std::to_string(detectionRect.x) + "," + std::to_string(detectionRect.y);
		text += "|" + std::to_string(detectionRect.width) + "x" + std::to_string(detectionRect.height);

		cv::putText(outPicture, text, cv::Point(detectionRect.x, detectionRect.y),
			1, 1, color, 1);
		cv::rectangle(outPicture, detectionRect,
			color, 2);
	}
	return 0;
}


