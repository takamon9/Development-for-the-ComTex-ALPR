#ifndef __PREFUNCTION_H_INCLUDED__
#define __PREFUNCTION_H_INCLUDED__

#include "setting.h"
#include <array>

void matrixConvert(Mat matrixName, string fileN,int x, int y)
{
	fstream inFile;
	stringstream fn;
	Mat resizedMat;

	cv::resize(matrixName, resizedMat, cv::Size(x, y));

	fn << "./dev/dat/" << fileN << ".dat";
	inFile.open(fn.str(), ios::out | ios::trunc); inFile.close(); //delete contents
	inFile.open(fn.str(), ios::in | ios::app | ios::binary);

	for (int i = 0; i < resizedMat.rows; i++) {
		for (int j = 0; j < resizedMat.cols; j++) {
			inFile << (int)(resizedMat.at<uchar>(i, j)) / 255;
		}
	}
	inFile.close();
}

string timestamp() {
	time_t timer = time(0);
	struct tm *timeStruct = localtime(&timer);
	int year = timeStruct->tm_year + 1900;
	int month = timeStruct->tm_mon + 1;
	int day = timeStruct->tm_mday;
	int hour = timeStruct->tm_hour;
	int minute = timeStruct->tm_min;
	int second = timeStruct->tm_sec;

	stringstream dateSS;
	dateSS << year << "_" << setw(2) << setfill('0') << month << "_" << setw(2) << setfill('0') << day << "_" << setw(2) << setfill('0') << hour << "_" << setw(2) << setfill('0') << minute << "_" << setw(2) << setfill('0') << second;
	string getDate = dateSS.str();
	string timeStamp = getDate.c_str();
	string rows0 = timeStamp;
	return rows0;
}

int loadNueron() {

	cv::String fileName("./dat/neuron.xml");
	cv::String objName("neuron");

	neuron = ANN_MLP::load<ANN_MLP>(fileName, objName);
	if (neuron.empty() == true) {
		cout << "Check your neuron.xml exists the folder" << endl;
		return -1;
	}
	else {
		cout << "neuron is loaded." << endl;
	}
}

int accessIpCamera() {

	string ip_address = ("http://" +ipCamID+ ":" + ipCamPassword + "@" + ip_Num + "/" + cameraAddress);
	capture.open(ip_address);
	
	if (capture.isOpened() == 0){
		cout << "We can not access to the IP camera" << endl;
		return 0;
	}

	else {
		cout << "Sucsessfully, connect to the IP camera" << endl;
	}

}

int accessCascade() {
		if (!normal700_cascade.load("./cascade/700_cascade.xml")){
		cout << "No cascade found!" << endl;
		system("pause");
		return -1;
		}
		else {
			cout << "cascade is loaded." << endl;
		}
}

void loadCameraSet() {

	vector<string> dataSetting;
	fstream cameraSet;

	string fileName = ("./dat/cameraSet.dat");
	cameraSet.open(fileName);
	if (cameraSet.is_open() == 0) {
		cout << "We could not open camera data file!" << endl;
	}

	string setting;
	int lineCount = 0;

	while (cameraSet.eof() == false) {
		getline(cameraSet, setting);
		istringstream parts(setting);
		string words;

		for (int i = 0; i < 5; i++) {
			parts >> words;
		}
		lineCount++;
	}
}

void saveCameraList() {

	int rowCount;
	int columnCount;

	fstream save_fs;

	save_fs.open("./dev/dat/cameraSet.dat", ios::out | ios::ate);
	if (save_fs.is_open() == 0) {
		cout << "We can not open the file" << endl;
		return;
	}

	save_fs.close();

	stringstream save_ss;
	string text_srt;

	for (int i = 0; i < rowCount - 1; i++) {
		for (int y = 0; y < columnCount - 1; y++) {
			save_ss << text_srt << " ";
			if (i < rowCount - 2 && y == 4) {
				save_ss << "\n";
			}
		}
	}

	string save_str = save_ss.str();
	const char* ss_char = save_str.c_str();
	save_fs.open("./dat/cameraSet.dat", ios::in | ios::out | ios::app);
	save_fs.write(ss_char, save_str.size());
	save_fs.close();
}


void createNumberArray() {
	for (int j = 0; j < 4; j++) {
		number[j].create(numArrWidth, numArrHight, CV_8SC3);
	}
}

int readOriginal() {
	if (!capture.read(original)) {
		return -1;
	}
	orignImage = original.clone();
	cvtColor(original, gray, CV_BGR2GRAY);
	cvtColor(original, matrix32s, CV_32S);
	//imshow("original", original);
	imshow("gray", gray);
}



void savePlate(Mat& imageSaved,int& plateDeteCount) {
	if (plateDeteCount % 5 == 0) {
		stringstream savefileName;
		savefileName << "./dev/plateImage/" << timestamp() << ".png";
		//cout << savefileName.str() << endl;
		imwrite(savefileName.str(), imageSaved);
	}
}

void saveNum(Mat& numSaved, string& strx,int& a) {
	if (plateDetectCounter % 5 == 0) {
		stringstream savefileName;
		savefileName << "./dev/numImage/num" << strx << "/" << timestamp() << "_" << a << ".png";
		//cout << savefileName.str() << endl;
		imwrite(savefileName.str(), numSaved);
	}
}

void cropKana(Mat& resized) { //binaryNP

	const double kana_rightSide = 0.212;
	const double kana_upperSide = 0.424;
	const double kana_lowerSide = 0.090;

	int matHeight = resized.rows * kana_upperSide;
	int matCols = resized.cols * kana_rightSide;
	int lower = resized.rows * kana_lowerSide;
	Point leftUpper = (0, matHeight);
	Point rightLower = (matCols, lower);
	Mat kanaMat;
	string kanaStr;
	kanaMat = Mat(resized, Rect(leftUpper, rightLower));
	stringstream saveMatKana;
	saveMatKana << "./dev/image/kana/" << timestamp() << ".png";
	imwrite(saveMatKana.str(), kanaMat);
	matrixConvert(kanaMat, kanaStr,kana_x,kana_y);

}

void cropBunrui(Mat& resized_bunrui) {  //binaryNP
	const double bunri_rightSide = 0.833;
	const double bunri_leftSide = 0.181;
	const double bunri_lowerSide = 0.424;

	Point leftUpper = (bunri_leftSide * resized_bunrui.cols,0);
	Point rightLower = (bunri_rightSide*resized_bunrui.cols,0);

	Mat bunrui;
	bunrui = Mat(resized_bunrui, Rect(leftUpper, rightLower));
	stringstream saveMatBunrui;
	saveMatBunrui << "./dev/image/bunrui/" << bunrui << ".png";
	imwrite(saveMatBunrui.str(), bunrui);

	string bunruiStr;
	matrixConvert(resized_bunrui, bunruiStr,bunrui_x,bunrui_y);

}

void cropArea(Mat& resized) { //binaryNP

	const int leftSide = resized.cols * 0.212;
	const int rightSide = resized.cols * 0.515;
	const int lower = resized.rows * 0.515;
	Point leftUpper = (leftSide, 0);
	Point rightLower = (rightSide, lower);
	Mat area;
	area = Mat(resized, Rect(leftUpper, rightLower));
	stringstream saveAreaMat;
	saveAreaMat << "./dev/image/area/" << timestamp() << ".png";
	imwrite(saveAreaMat.str(), area);

	string areaStr;
	matrixConvert(resized, areaStr,area_x,area_y);
}

#endif