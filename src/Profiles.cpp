/*
  Profiles.cpp - Firing Profile Management Library for the Maauw Automatic Kiln
  Copyright (c) 2017 Syby Abraham.
*/

#include "Profiles.h"

//Default Constructor

Profiles::Profiles() {
	_segs = 0;
	_selParam = 0;
	_currentSeg = 1;
}

//Public Methods

void Profiles::setSegs() {

	core.drawTitle("Ramp Segments");
	u8g2.setFont(u8g2_font_crox1h_tf);
	u8g2.drawStr(core.centerLine("Enter number of segments"), 28, "Enter number of segments");

	_segs = value;

	if (_segs >= 9) { //Value Clamp
		value = 9;
		_segs = 9;
	}
	else if (_segs <= 1) {
		value = 1;
		_segs = 1;
	}

	char segChar[2];
	core.convertToChar(String(_segs), segChar);
	u8g2.setFont(u8g2_font_helvB10_tf);
	u8g2.drawUTF8(core.centerLine(segChar), 48, segChar);
}

void Profiles::setTRH(int prfN) {

	//Set Temperature, Ramp and Hold across all segments in an entire profile

	if (btnPress == true) {
		btnPress = false;
		_selParam++;
	}
	if (_selParam > 2) {
		_selParam = 0;
		_currentSeg++;
	}

	if (_currentSeg > _segs) {
		//1. EEPROM Write & EPPROM Read and Assign to variables
		Serial.println("WRITING TO EEPROM");
		eepromWriteProfile(prfN);
		Serial.println("READING FROM EEPROM");
		eepromReadProfile(prfN);

		//2. Exit.   Using while loop in main file?
		_selParam = 0;
		_currentSeg = 1;
		_exitState = true;
	}
	else { 
		_exitState = false;
	}
	switch (_selParam) {
	case 0:
		switchSetTemp(_currentSeg);
		break;
	case 1:
		switchSetRamp(_currentSeg);
		break;
	case 2:
		switchSetHold(_currentSeg);
		break;
	}
}

void Profiles::eepromWriteProfile(int prfN) {

	//72 bytes is the maximum size all the variables in a profile takes. 
	//Multiplying it by the (profile number -1) gives us the starting position(address) for each profile. 
	//Total EEPROM used by all profiles and its segments will be 648 bytes.

	_eeAddress = (prfN - 1) * 72;

	for (int i = 1; i < 9; i++) {
		EEPROM.update(_eeAddress, switchGetTemp(i));	//Write temprature to EEPROM
		_eeAddress += sizeof(int);						//Advance address by the byte size of int (2)
		EEPROM.update(_eeAddress, switchGetRamp(i));	//Write ramp to EEPROM
		_eeAddress += sizeof(int);						//Advance address by the byte size of int (2)
		EEPROM.update(_eeAddress, switchGetHold(i));	//Write hold to EEPROM
		_eeAddress += sizeof(unsigned long);			//Advance address by the byte size of unsigned long (4)
	}
	_eeAddress = 0;
}

void Profiles::eepromReadProfile(int prfN) {

	_eeAddress = (prfN - 1) * 72;

	for (int i = 0; i < 9; i++) {
		int eeTemp;
		int eeRamp;
		unsigned long eeHold;
		EEPROM.get(_eeAddress, eeTemp);
		seg[i].setTemp(eeTemp);
		_eeAddress += sizeof(int);
		EEPROM.get(_eeAddress, eeRamp);
		seg[i].setRamp(eeRamp);
		_eeAddress += sizeof(int);
		EEPROM.get(_eeAddress, eeHold);
		seg[i].setHold(eeHold);
		_eeAddress += sizeof(unsigned long);
	}
	_eeAddress = 0;
} 

bool Profiles::getExit() {
	if (_exitState == true) {
		return true;
	}
	else {
		return false;
	}
}

void Profiles::setExit(bool state) {
	_exitState = state;
}

void Profiles::switchSetTemp(int segNT) {
	switch (segNT) {
	case 1:
		seg[0].setTempUI(1);
		break;
	case 2:
		seg[1].setTempUI(2);
		break;
	case 3:
		seg[2].setTempUI(3);
		break;
	case 4:
		seg[3].setTempUI(4);
		break;
	case 5:
		seg[4].setTempUI(5);
		break;
	case 6:
		seg[5].setTempUI(6);
		break;
	case 7:
		seg[6].setTempUI(7);
		break;
	case 8:
		seg[7].setTempUI(8);
		break;
	case 9:
		seg[8].setTempUI(9);
		break;
	}
}

void Profiles::switchSetRamp(int segNR) {
	switch (segNR) {
	case 1:
		seg[0].setRampUI(1);
		break;
	case 2:
		seg[1].setRampUI(2);
		break;
	case 3:
		seg[2].setRampUI(3);
		break;
	case 4:
		seg[3].setRampUI(4);
		break;
	case 5:
		seg[4].setRampUI(5);
		break;
	case 6:
		seg[5].setRampUI(6);
		break;
	case 7:
		seg[6].setRampUI(7);
		break;
	case 8:
		seg[7].setRampUI(8);
		break;
	case 9:
		seg[8].setRampUI(9);
		break;
	}
}

void Profiles::switchSetHold(int segNH) {
	switch (segNH) {
	case 1:
		seg[0].setHoldUI(1);
		break;
	case 2:
		seg[1].setHoldUI(2);
		break;
	case 3:
		seg[2].setHoldUI(3);
		break;
	case 4:
		seg[3].setHoldUI(4);
		break;
	case 5:
		seg[4].setHoldUI(5);
		break;
	case 6:
		seg[5].setHoldUI(6);
		break;
	case 7:
		seg[6].setHoldUI(7);
		break;
	case 8:
		seg[7].setHoldUI(8);
		break;
	case 9:
		seg[8].setHoldUI(9);
		break;
	}
}

unsigned int Profiles::switchGetTemp(int segN) {
	switch (segN) {
	case 1:
		return seg[0].getTemp();
		break;
	case 2:
		return seg[1].getTemp();
		break;
	case 3:
		return seg[2].getTemp();
		break;
	case 4:
		return seg[3].getTemp();
		break;
	case 5:
		return seg[4].getTemp();
		break;
	case 6:
		return seg[5].getTemp();
		break;
	case 7:
		return seg[6].getTemp();
		break;
	case 8:
		return seg[7].getTemp();
		break;
	case 9:
		return seg[8].getTemp();
		break;
	}
}

unsigned int Profiles::switchGetRamp(int segN) {
	switch (segN) {
	case 1:
		return seg[0].getRamp();
		break;
	case 2:
		return seg[1].getRamp();
		break;
	case 3:
		return seg[2].getRamp();
		break;
	case 4:
		return seg[3].getRamp();
		break;
	case 5:
		return seg[4].getRamp();
		break;
	case 6:
		return seg[5].getRamp();
		break;
	case 7:
		return seg[6].getRamp();
		break;
	case 8:
		return seg[7].getRamp();
		break;
	case 9:
		return seg[8].getRamp();
		break;
	}
}

unsigned long Profiles::switchGetHold(int segN) {
	switch (segN) {
	case 1:
		return seg[0].getHold();
		break;
	case 2:
		return seg[1].getHold();
		break;
	case 3:
		return seg[2].getHold();
		break;
	case 4:
		return seg[3].getHold();
		break;
	case 5:
		return seg[4].getHold();
		break;
	case 6:
		return seg[5].getHold();
		break;
	case 7:
		return seg[6].getHold();
		break;
	case 8:
		return seg[7].getHold();
		break;
	case 9:
		return seg[8].getHold();
		break;
	}
} 

void Profiles::review() {

}

//Private Methods