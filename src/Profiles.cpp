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

	maauw.drawTitle("Ramp Segments");
	u8g2.setFont(u8g2_font_crox1h_tf);
	u8g2.drawStr(maauw.centerLine("Enter number of segments"), 28, "Enter number of segments");

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
	maauw.convertToChar(String(_segs), segChar);
	u8g2.setFont(u8g2_font_helvB10_tf);
	u8g2.drawUTF8(maauw.centerLine(segChar), 48, segChar);
}


uint8_t Profiles::setTRH(uint8_t prfN) {

	//Set Temperature, Ramp and Hold across all segments in an entire profile


	if (btnPress == true) {
		btnPress = false;
		_selParam++;
	}
	if (_selParam > 2) {
		_selParam = 0;
		_currentSeg++;
	}

	Serial.print("Sel Param: ");
	Serial.println(_selParam);
	Serial.print("Current Seg:");
	Serial.println(_currentSeg);
	Serial.print("Exit: ");
	Serial.println(_exit);

	if (_currentSeg > _segs && _exit == false) {
		_saveTimer = millis();
		//1. EEPROM Write & EPPROM Read and Assign to variables
		eepromWriteProfile(prfN);
		Serial.println("EEPROM Write Complete!");
		_exit = true; //2. Activate Exit State

		//eepromReadProfile(prfN); //Read from EEPROM and assign variables
	}
	if (_exit == true) {
		char titleChar[10];
		maauw.convertToChar((String("Profile ") + String(prfN)), titleChar);
		maauw.drawTitle(titleChar);
		u8g2.setFont(u8g2_font_helvB10_tf);
		char savePrompt[17];
		maauw.convertToChar((String("Profile ") + String(prfN) + String(" Saved!")), savePrompt);
		u8g2.drawUTF8(maauw.centerLine(savePrompt), 45, savePrompt);
		if (currentMillis - _saveTimer >= 1000) {
			_exit = false;
			_currentSeg = 1;
			return 0;
		}
	}

	if (_exit != true) {
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
	return 1;
}

void Profiles::eepromWriteProfile(uint8_t prfN) {

	//The maximum size all the variables in a profile can take is 52 bytes. 2 bytes * 3 variables * 9 segments
	//Multiplying it by the profile number - 1 (same as segment array index), gives us the starting position (address) for each profile. 
	//Total EEPROM used by all profiles and its segments is 468 bytes.

	_eeAddress = (prfN - 1) * 52; //_eeAddress var works like a mechanical read/write head for the EEPROM

	for (int i = 1; i < 9; i++) {
		Serial.print("Temp: ");
		Serial.println(switchGetTemp(i));
		EEPROM.put(_eeAddress, switchGetTemp(i));	//Write temprature to EEPROM
		_eeAddress += sizeof(uint16_t);				//Advance address by the byte size of uint16_t (2)
		Serial.print("Ramp: ");
		Serial.println(switchGetRamp(i));
		EEPROM.put(_eeAddress, switchGetRamp(i));	//Write ramp to EEPROM
		_eeAddress += sizeof(uint16_t);				//Advance address by the byte size of uint16_t (2)
		Serial.print("Hold: ");
		Serial.println(switchGetHold(i));
		EEPROM.put(_eeAddress, switchGetHold(i));	//Write hold to EEPROM
		_eeAddress += sizeof(uint16_t);				//Advance address by the byte size of uint16_t (2)
	}
	_eeAddress = 0;
}

void Profiles::eepromReadProfile(uint8_t prfN) {

	_eeAddress = (prfN - 1) * 52;

	for (int i = 0; i < 9; i++) { //Read from all of the EEPROM segments for the profile
		uint16_t eeTemp;
		uint16_t eeRamp;
		uint16_t eeHold;
		EEPROM.get(_eeAddress, eeTemp);
		seg[i].setTemp(eeTemp);
		_eeAddress += sizeof(uint16_t);
		EEPROM.get(_eeAddress, eeRamp);
		seg[i].setRamp(eeRamp);
		_eeAddress += sizeof(uint16_t);
		EEPROM.get(_eeAddress, eeHold);
		seg[i].setHold(eeHold);
		_eeAddress += sizeof(uint16_t);
	}
	_eeAddress = 0;
}

void Profiles::switchSetTemp(uint8_t segN) {
	seg[segN - 1].setTempUI(segN);	//segN - 1 for the array index and segN for the segment number
}

void Profiles::switchSetRamp(uint8_t segN) {
	seg[segN - 1].setRampUI(segN);
}

void Profiles::switchSetHold(uint8_t segN) {
	seg[segN - 1].setHoldUI(segN);
}

uint16_t Profiles::switchGetTemp(uint8_t segN) {
	return seg[segN - 1].getTemp();
}

uint16_t Profiles::switchGetRamp(uint8_t segN) {
	return seg[segN - 1].getRamp();
}

uint16_t Profiles::switchGetHold(uint8_t segN) {
	return seg[segN - 1].getHold();
}


void Profiles::reviewUI(uint8_t prfN) {
	//get segment
	//get all the temps, holds, ramps from all the segments
	//limit what gets shown using the user set segs 
}

//Private Methods