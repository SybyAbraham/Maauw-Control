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

void Profiles::setSegs(char sName[]) {

	core.drawTitle(sName);
	u8g2.setFont(u8g2_font_crox1h_tf);
	u8g2.drawStr(core.centerLine("Enter number of segments"), 28, "Enter number of segments");

	_segs = value;

	if (_segs >= 9) {
		value = 9;
		_segs = 9;
	}
	else if (_segs <= 0) {
		value = 0;
		_segs = 0;
	}

	char segChar[2];
	core.convertToChar(String(_segs), segChar);
	u8g2.setFont(u8g2_font_helvB10_tf);
	u8g2.drawUTF8(core.centerLine(segChar), 48, segChar);
}

void Profiles::setTRH() {
	//Set Temperature, Ramp and Hold across all segments in an entire profile
	

	if (btnPress == true) {
		btnPress = false;
		_selParam++;
	} 
	if (_selParam > 2) {
		_selParam = 0;
		_currentSeg++;
	}
	else if (_currentSeg > _segs) {
		//exit 
		_selParam = 0;
		_currentSeg = 1;
	}

	Serial.print("Current Segment: ");
	Serial.println(_currentSeg);
	Serial.print("Selected Param: ");
	Serial.println(_selParam);
	switch (_selParam) {
	case 0:
		//setSegs();
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

void Profiles::switchSetTemp(int segN) {
	switch (segN) {
	case 1:
		seg[0].setTemp(1);
		break;
	case 2:
		seg[1].setTemp(2);
		break;
	case 3:
		seg[2].setTemp(3);
		break;
	case 4:
		seg[3].setTemp(4);
		break;
	case 5:
		seg[4].setTemp(5);
		break;
	case 6:
		seg[5].setTemp(6);
		break;
	case 7:
		seg[6].setTemp(7);
		break;
	case 8:
		seg[7].setTemp(8);
		break;
	case 9:
		seg[8].setTemp(9);
		break;
	}
}

void Profiles::switchSetRamp(int segN) {
	switch (segN) {
	case 1:
		seg[0].setRamp(1);
		break;
	case 2:
		seg[1].setRamp(2);
		break;
	case 3:
		seg[2].setRamp(3);
		break;
	case 4:
		seg[3].setRamp(4);
		break;
	case 5:
		seg[4].setRamp(5);
		break;
	case 6:
		seg[5].setRamp(6);
		break;
	case 7:
		seg[6].setRamp(7);
		break;
	case 8:
		seg[7].setRamp(8);
		break;
	case 9:
		seg[8].setRamp(9);
		break;
	}
}

void Profiles::switchSetHold(int segN) {
	switch (segN) {
	case 1:
		seg[0].setHold(1);
		break;
	case 2:
		seg[1].setHold(2);
		break;
	case 3:
		seg[2].setHold(3);
		break;
	case 4:
		seg[3].setHold(4);
		break;
	case 5:
		seg[4].setHold(5);
		break;
	case 6:
		seg[5].setHold(6);
		break;
	case 7:
		seg[6].setHold(7);
		break;
	case 8:
		seg[7].setHold(8);
		break;
	case 9:
		seg[8].setHold(9);
		break;
	}
}

int Profiles::switchGetTemp(int segN) {
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

void Profiles::review() {

}

//Private Methods

int Profiles::_getSegs() {
	return _segs;
}

void Profiles::_getVolatileSeg(void) {
	//Get user set segments from EEPROM
	//Set flags in EEPROM to check if user set value exists?
}