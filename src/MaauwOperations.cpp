/*
MaauwOperations.cpp - Maauw Automatic Kiln Control Firmware Core Functions
Copyright (c) 2017 Syby Abraham.
*/

#include "MaauwOperations.h"

//Default Constructor

MaauwOperations::MaauwOperations() {
	_valvepin = 28;
	_ignpin = 29;
	pinMode(_valvepin, OUTPUT);
	pinMode(_ignpin, OUTPUT);
}

//Public Definitions

////////////////////////// BEGIN ELECTROMECHANICAL DEFINTIONS //////////////////////////

uint8_t MaauwOperations::ignite(void) {

	//master ignition routine

	if (_valveState == 0) { //If valve is not already open, open it
		_valveState = 1;
		digitalWrite(_valvepin, _valveState);

	}

	if (_ignState == 0) { //If igniter is not already on, turn it on
		_ignState = 1;
		digitalWrite(_ignpin, _ignState);
		_igntimer = millis(); //Clock capture
		_igntimer = millis(); //Clock capture
		return 1; //Signal incomplete process
	}
	else { //if igniter is already on
	 //check timer
		u8g2.drawXBMP(38, 3, 52, 15, logo.maauwS_bits);
		u8g2.setFont(u8g2_font_helvB10_tf);
		u8g2.drawUTF8(centerLine("Lighting Burner"), 38, "Lighting Burner");
		int8_t igniteBar = map((currentMillis - _igntimer), 0, 1000, 0, 82);	//Map the elapsed time to 0 to 82 pixels
		u8g2.drawFrame(23, 54, 82, 9);											//for the progress bar
		u8g2.drawBox(23, 54, igniteBar, 9);

		if (currentMillis - _igntimer >= 1000) {
			_ignState = 0;
			digitalWrite(_ignpin, _ignState);
			u8g2.drawUTF8(centerLine("Ignited"), 41, "Ignited");
			u8g2.drawFrame(23, 54, 82, 9);
			u8g2.drawBox(23, 54, 82, 9);
			return 0; //Signal completed process
		}
		else {
			return 1; //Signal incomplete process
		}
	}
}

void MaauwOperations::extingush() {
	_valveState = 0;
	digitalWrite(_valvepin, _valveState);
}

uint16_t MaauwOperations::readPyro(void) {
	//Read Internal Kiln Temperature
	return max31855.readCelsius();
}

uint16_t MaauwOperations::readBurner(void) {
	//read burner thermocouple

}

uint16_t MaauwOperations::readExternalTemp(void) {
	//Read Outside Temperature
	return max31855.readInternal();
}

bool MaauwOperations::getValveState() {
	return _valveState;
}

////////////////////////// END ELECTROMECHANICAL DEFINTIONS //////////////////////////


////////////////////////// BEGIN USER INTERFACE DEFINITIONS //////////////////////////

//String to char array conversion
void MaauwOperations::convertToChar(String str, char outChar[]) {
	int16_t str_len = str.length() + 1;
	char convChar[str_len];
	str.toCharArray(convChar, str_len);
	strcpy(outChar, convChar);
}

//Find X center
int MaauwOperations::centerLine(char input[]) {
	int xVal = ((128 - u8g2.getStrWidth(input)) / 2);
	return xVal;
}

//Draw Menu Title
void MaauwOperations::drawTitle(char dtChar[]) {
	u8g2.setFont(u8g2_font_helvB08_tr);                    //Set font
	u8g2.drawBox(0, 0, 128, 13);                           //Draw a box at the top of the display
	u8g2.setDrawColor(0);                                  //Set font color to black
	u8g2.drawStr(centerLine(dtChar), 11, dtChar); //Center & Draw string on display
	u8g2.setDrawColor(1);                                  //Set following font colors back to white
}

//Draw Menu Item
void MaauwOperations::drawMenuItem(int pos, char dmText[], bool box) {
	u8g2.setFont(u8g2_font_crox1h_tf);                      //Set font for menu items
	if (pos == 1) {
		pos = 30;
	}
	else if (pos == 2) {
		pos = 46;
	}
	else if (pos == 3) {
		pos = 62;
	}
	if (box == true) {										//Draw a white box around the item if boolean is true
		u8g2.drawBox(0, (pos - 12), 128, 14);               //Draw a white box
		u8g2.setDrawColor(0);                               //Set font color to black
		u8g2.drawStr(3, pos, dmText);                       //Draw string on display
		u8g2.setDrawColor(1);                               //Set following font      colors back to white
	}
	else {
		u8g2.drawStr(3, pos, dmText);                       //Draw string on display
	}
}

void MaauwOperations::readEncoderDirection() {
	value += encoder->getValue();

	if (value > lastValue) {
		lastValue = value;
		down = true;

	}
	else if (value < lastValue) {
		lastValue = value;
		up = true;
	}
}

void MaauwOperations::readEncoderButton() {
	ClickEncoder::Button b = encoder->getButton();
	if (b != ClickEncoder::Open) {
		switch (b) {
		case ClickEncoder::Clicked:
			btnPress = true;
			break;
		case ClickEncoder::Held:
			if (page != 1) {
				page = 1;
			}

		}
	}
}

////////////////////////// END USER INTERFACE DEFINITIONS //////////////////////////