/*
MaauwOperations.cpp - Maauw Automatic Kiln Control Firmware Core Functions
Copyright (c) 2017 Syby Abraham.
*/

#include "MaauwOperations.h"

//Default Constructor
MaauwOperations::MaauwOperations() {
}

//Public Definitions

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
	u8g2.drawStr(MaauwOperations::centerLine(dtChar), 11, dtChar); //Center & Draw string on display
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