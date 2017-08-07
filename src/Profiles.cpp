/*
  Profiles.h - Firing Profile Management Library for the Maauw Automatic Kiln
  Copyright (c) 2017 Syby Abraham.
*/

#include "Profiles.h"

//Default Constructor

Profiles::Profiles() {
	_segs = 0;
	_selParam = 0;
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

void Profiles::review() {

}

void Profiles::setTRH(int8_t segN) {
	//Set Temperature, Ramp and Hold in an entire segment
	if (btnPress == true) {
		btnPress = false;
		_selParam++;
	}

	switch (_selParam) {
	case 0:
		switchSetTemp(segN);
		break;
	case 1:
		switchSetRamp(segN);
		break;
	case 2:
		switchSetHold(segN);
		break;
	}
}

int Profiles::getSelParam() {
	return _selParam;
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

//Private Methods

int Profiles::_getSegs() {
	return _segs;
}

void Profiles::_getVolatileSeg(void) {
	//Get user set segments from EEPROM
	//Set flags in EEPROM to check if user set value exists?
}