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
	//If igniter is not already on, turn it on
	if (_ignState == 0 && _igntimer == 0) {			//_igntimer == 0 condition to prevent this code from running twice
		_ignState = 1;
		digitalWrite(_ignpin, _ignState);
		_igntimer = millis();						//Clock capture. Non-zero value prevents this code running again
	}
	//If igniter is already on, check timer and turn it off when timer expires
	else if (_ignState == 1 && _ignExit == 0) {		//_ignExit == 0 condition to prevent this code from running after ignExit is set
		u8g2.drawXBMP(38, 3, 52, 15, logo.maauwS_bits);
		u8g2.setFont(u8g2_font_helvB10_tf);
		u8g2.drawUTF8(centerLine("Lighting Burner"), 38, "Lighting Burner");
		int8_t igniteBar = map((clock - _igntimer), 0, 1000, 0, 82);//Map the elapsed time to 0 to 82 pixels
		u8g2.drawFrame(23, 54, 82, 9);								//for the progress bar
		u8g2.drawBox(23, 54, igniteBar, 9);
		if (clock - _igntimer >= 1000 && _ignExit == 0) {			//check timer, if ignition timer expires
			_ignState = 0;											//turn off igniter
			digitalWrite(_ignpin, _ignState);
			_ignExit = millis();									//Clock capture. Non-zero value prevents this code running again
		}
	}
	else if (_ignExit != 0) { //if exit clock is active, show "Ignited" and exit when timer expires
		u8g2.drawXBMP(38, 3, 52, 15, logo.maauwS_bits);
		u8g2.setFont(u8g2_font_helvB10_tf);
		u8g2.drawUTF8(centerLine("Ignited!"), 38, "Ignited!");
		u8g2.drawFrame(23, 54, 82, 9);
		u8g2.drawBox(23, 54, 82, 9);
		if ((clock - _ignExit) > 1000) {	//Exit when timer expires
			_igntimer = 0;					//Clear Igniter Clock
			_ignExit = 0;					//Clear Exit Clock
			return 0;						//Signal completed process / Exit Ignition State
		}
	}
	return 1; //Signal incomplete process / Stay in Ignition State
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