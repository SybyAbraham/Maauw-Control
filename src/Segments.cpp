/*
  Segments.cpp - Segmented Temperature, Ramp and Hold Adjustment Library for the Maauw Automatic Kiln
  Copyright (c) 2017 Syby Abraham.
*/

#include "Segments.h"

//DEFAULT CONSTRUCTOR
Segments::Segments() {
	//init private variables
	char _tempChar[10] = "";
	_temp = 0;
	_ramp = 0;
	_hold = 0; 
	_holdHours = 0;
	_holdMinutes = 0;
}

//ACCESSORS
uint16_t Segments::getTemp() {
	return _temp;
}

uint16_t Segments::getRamp() {
	return _ramp;
}

uint16_t Segments::getHold() {
	return _hold;
}


//Mutators
void Segments::setTemp(uint16_t t) {
	_temp = t;
}
void Segments::setRamp(uint16_t r) {
	_ramp = r;
}
void Segments::setHold(uint16_t h) {
	_hold = h;
}

//MUTATE AND DRAW
void Segments::setTempUI(uint8_t segN) {

	char menuTitle[15]; //Create variable to hold converted char
	maauw.convertToChar(String(F("Ramp Segment ")) + String(segN), menuTitle); //Convert to char and store in above var
	maauw.drawTitle(menuTitle); //Draw the title using the converted var

	u8g2.setFont(u8g2_font_crox1h_tf);
	u8g2.drawStr(maauw.centerLine("Enter Temperature"), 30, "Enter Temperature");

	_temp = value; //Rot Enc Assign

	if (_temp >= 1400) {  //Value Clamping
		value = 1400;
		_temp = 1400;
	}

	String strC;
	float coneLCD = cone.getCone(_temp);
	int intConeLCD;                                     //Int variable to strip the decimal point
	if (coneLCD == -255) {                              //Temp Lower than lowest cone
		strC = (String("<") + String(22));
	}
	else if (coneLCD != -255 && coneLCD != -5.5 && (coneLCD < 0)) {
		intConeLCD = coneLCD * -1;                       //Multiply by -1 to convert to a positive integer
		strC = (String(0) + String(intConeLCD));         //Replace the negative sign with a 0
	}
	else if (coneLCD == -5.5) {
		strC = String("05½");
	}
	else if (coneLCD == 5.5) {
		strC = String("5½");
	}
	else if (coneLCD >= 0 && coneLCD <= 14) {
		intConeLCD = coneLCD;
		strC = String(intConeLCD);
	}
	else if (coneLCD == 15) {
		strC = (String(15) + String("+"));
	}

	u8g2.setFont(u8g2_font_helvB10_tf);
	maauw.convertToChar((String(_temp) + String("°C")), _tempChar);
	u8g2.drawUTF8(10, 51, _tempChar);
	u8g2.setFont(u8g2_font_helvB10_tf);
	u8g2.drawXBMP(79, 40, 9, 11, logo.cone11px_bits);
	char coneChar[5];
	maauw.convertToChar(strC, coneChar);
	u8g2.drawUTF8(90, 51, coneChar);
}

void Segments::setRampUI(uint8_t segN) {

	char menuTitle[15];		//Create variable to hold converted char
	maauw.convertToChar(String(F("Ramp Segment ")) + String(segN), menuTitle); //Convert to char and store in above var
	maauw.drawTitle(menuTitle); //Draw the title using the converted var

	u8g2.setFont(u8g2_font_crox1h_tf);
	u8g2.drawStr(maauw.centerLine("Enter Ramp Rate"), 30, "Enter Ramp Rate");

	_ramp = value;

	if (_ramp >= 500) {  //Value Clamping
		value = 500;
		_ramp = 500;
	}
	char rampChar[10];
	maauw.convertToChar((String(_ramp) + String("°C/HR")), rampChar);
	u8g2.setFont(u8g2_font_helvB10_tf);
	u8g2.drawUTF8(maauw.centerLine(rampChar), 51, rampChar);
}

void Segments::setHoldUI(uint8_t segN) {
	char menuTitle[15];			//Create variable to hold converted char
	maauw.convertToChar(String(F("Ramp Segment ")) + String(segN), menuTitle); //Convert to char and store in above var
	maauw.drawTitle(menuTitle);	//Draw the title using the converted var

	u8g2.setFont(u8g2_font_crox1h_tf);
	u8g2.drawStr(maauw.centerLine("Enter Hold Time"), 34, "Enter Hold Time");

	_holdMinutes = value;
	if (_holdMinutes > 59) {
		value = 0;
		_holdMinutes = 0;		// Clamp min minutes to 0
		_holdHours++;
		if (_holdHours > 72) {	//Clamp max hours to 72
			_holdHours = 72;
		}
	}
	else if (_holdMinutes < 0) {
		value = 59;
		_holdMinutes = 59;		//Clamp max minutes to 59
		_holdHours--;
		if (_holdHours < 0) {	//Clamp min hours to 0
			_holdHours = 0;
		}
	}

	_hold = _holdMinutes + (_holdHours * 60); //Convert hours and minutes to minutes

	char charHrMin[25];
	String hrsStr;
	String minStr;

	//Grammar is important

	if (_holdHours == 1 && _holdMinutes > 0) {
		hrsStr = String(_holdHours) + String(" Hour & ");
	}
	else if (_holdHours == 1 && _holdMinutes == 0) {
		hrsStr = String(_holdHours) + String(" Hour");
	}
	else if (_holdHours > 1 && _holdMinutes > 0) {
		hrsStr = String(_holdHours) + String(" Hours & ");
	}
	else if (_holdHours > 1 && _holdMinutes == 0) {
		hrsStr = String(_holdHours) + String(" Hours");
	}
	else {
		hrsStr = "";
	}

	if (_holdMinutes == 1) {
		minStr = String(_holdMinutes) + String(" Minute");
	}
	else if (_holdMinutes > 1) {
		minStr = String(_holdMinutes) + String(" Minutes");
	}
	else {
		minStr = "";
	}
	
	if (_holdMinutes == 0 && _holdHours == 0) {
		hrsStr = "";
		minStr = "No Hold";
	}

	u8g2.setFont(u8g2_font_crox1hb_tr);
	maauw.convertToChar(String(hrsStr + minStr), charHrMin);
	u8g2.drawStr(maauw.centerLine(charHrMin), 53, charHrMin);
}