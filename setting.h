#ifndef __SETTING_H_INCLUDED__
#define __SETTING_H_INCLUDED__


#include "programHeader.h"

string ipCamID = "root";
string ipCamPassword = "taka1974";
string ip_Num = "192.168.100.30";
string cameraAddress;

enum class camMaker { canon, panasonic, axis, sony };
camMaker makerName = camMaker::canon ; // choose camra maker.

void seletCamera() {

	switch (makerName)
	{
	case camMaker::canon:
		cameraAddress = "-wvhttp-01-/GetOneShot?image_size=640x480&frame_count=0";
		break;
	case camMaker::panasonic:
		ipCamID = "panaroot";
		cameraAddress = "nphMotionJpeg?Resolution=640x480&Quality=Standard";
		break;
	case camMaker::axis:
		cameraAddress = "";
		break;
	case camMaker::sony:
		cameraAddress = "";
		break;
	default:
		cout << "Choose Camera Maker" << endl;
		break;
	}

}

#endif