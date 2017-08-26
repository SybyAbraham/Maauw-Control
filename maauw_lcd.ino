/*
Maauw Control // Automatic Kiln Control Firmware for ATmega2560 // PRE-ALPHA

Copyright (c) 2017, Syby Abraham, sybyabraham@hotmail.com
All rights reserved.

Maauw Control is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

#include <ClickEncoder.h>
#include <TimerOne.h>
#include <EEPROM.h>

#include "src/OrtonCones.h"
#include "src/Profiles.h"
#include "src/XBMPLogos.h"
#include "src/Segments.h"
#include "src/MaauwOperations.h"
#include "src/Reduction.h"

//U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE); //HW I2C OLED

U8G2_ST7565_64128N_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8); //DEV VERSION HW SPI

//U8G2_ST7565_64128N_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 2, /* dc=*/ 3, /* reset=*/ 4); //Maauw Control HW SPI


ClickEncoder *encoder;
uint16_t lastValue, value;

void timerIsr() {
	encoder->service();
}

OrtonCones cone;
Profiles profile[9]; //Create 9 profiles
Reduction reduce;
XBMPLogos logo;
MaauwOperations maauw;

char menuItem1[15];
char menuItem2[] = "Firing Profiles";
char menuItem3[] = "Reduction Settings";
char menuItem4[] = "Extingush Burner";
char menuItem5[] = "Display";
char menuItem6[] = "Maauw Control";

unsigned long currentMillis = 0;

uint8_t prevMenuItem = 1, frame = 1, selMenuItem = 1;

uint16_t page = 1;

bool up = false, down = false, btnPress = false;

unsigned long previousMillis = 0;

void drawMenuItems() {
	if (frame == 1 && selMenuItem == 1) {
		maauw.drawMenuItem(1, menuItem1, true);
		maauw.drawMenuItem(2, menuItem2, false);
		maauw.drawMenuItem(3, menuItem3, false);
	}
	else if (frame == 1 && selMenuItem == 2) {
		maauw.drawMenuItem(1, menuItem1, false);
		maauw.drawMenuItem(2, menuItem2, true);
		maauw.drawMenuItem(3, menuItem3, false);
	}
	else if (frame == 1 && selMenuItem == 3) {
		maauw.drawMenuItem(1, menuItem1, false);
		maauw.drawMenuItem(2, menuItem2, false);
		maauw.drawMenuItem(3, menuItem3, true);
	}
	else if (frame == 2 && selMenuItem == 2) {
		maauw.drawMenuItem(1, menuItem2, true);
		maauw.drawMenuItem(2, menuItem3, false);
		maauw.drawMenuItem(3, menuItem4, false);
	}
	else if (frame == 2 && selMenuItem == 3) {
		maauw.drawMenuItem(1, menuItem2, false);
		maauw.drawMenuItem(2, menuItem3, true);
		maauw.drawMenuItem(3, menuItem4, false);
	}
	else if (frame == 2 && selMenuItem == 4) {
		maauw.drawMenuItem(1, menuItem2, false);
		maauw.drawMenuItem(2, menuItem3, false);
		maauw.drawMenuItem(3, menuItem4, true);
	}
	else if (frame == 3 && selMenuItem == 3) {
		maauw.drawMenuItem(1, menuItem3, true);
		maauw.drawMenuItem(2, menuItem4, false);
		maauw.drawMenuItem(3, menuItem5, false);
	}
	else if (frame == 3 && selMenuItem == 4) {
		maauw.drawMenuItem(1, menuItem3, false);
		maauw.drawMenuItem(2, menuItem4, true);
		maauw.drawMenuItem(3, menuItem5, false);
	}
	else if (frame == 3 && selMenuItem == 5) {
		maauw.drawMenuItem(1, menuItem3, false);
		maauw.drawMenuItem(2, menuItem4, false);
		maauw.drawMenuItem(3, menuItem5, true);
	}
	else if (frame == 4 && selMenuItem == 4) {
		maauw.drawMenuItem(1, menuItem4, true);
		maauw.drawMenuItem(2, menuItem5, false);
		maauw.drawMenuItem(3, menuItem6, false);
	}
	else if (frame == 4 && selMenuItem == 5) {
		maauw.drawMenuItem(1, menuItem4, false);
		maauw.drawMenuItem(2, menuItem5, true);
		maauw.drawMenuItem(3, menuItem6, false);
	}
	else if (frame == 4 && selMenuItem == 6) {
		maauw.drawMenuItem(1, menuItem4, false);
		maauw.drawMenuItem(2, menuItem5, false);
		maauw.drawMenuItem(3, menuItem6, true);
	}
}


ISR(TIMER4_COMPA_vect)          // timer compare interrupt service routine
{
	if(maauw.getValveState() == 1){
	digitalWrite(13, digitalRead(13) ^ 1);   // toggle LED pin
	}
}

SIGNAL(TIMER0_COMPA_vect) {
	currentMillis = millis(); //Call millis once a millisecond 
}

//////SETUP//////SETUP//////SETUP//////SETUP//////SETUP//////SETUP//////SETUP//////SETUP//////SETUP////

void setup(void) {

	pinMode(13, OUTPUT);

	// initialize timer4 
	TCCR4A = 0;
	TCCR4B = 0;
	TCNT4 = 0;

	OCR4A = 0x7D0; //Interrupt at 2000 ticks

	TCCR4B |= (1 << WGM12);   // Clear Timer on Compare
	TCCR4B |= (1 << CS12);    // 256 prescaler 
	TIMSK4 |= (1 << OCIE4A);  //Enable Timer Compare Interrupt 

	OCR0A = 0xAF;			//timer0 output compare register setup for millis() call timing	
	TIMSK0 |= _BV(OCIE0A);	//timer0 enable output compare interrupt

	int loader = 0;
	u8g2.begin();
	u8g2.setFlipMode(1);
	u8g2.clearBuffer();          // clear the internal memory
	u8g2.drawXBMP(0, 5, 128, 35, logo.maauw_logo_bits);
	u8g2.drawFrame(23, 49, 82, 6);
	u8g2.sendBuffer();          // transfer internal memory to the display

	for (loader = 0; loader < 41; loader++) {
		u8g2.drawBox(23, 49, loader, 6);
		//delay(30);
		u8g2.sendBuffer();
	}

	Serial.begin(9600);

	encoder = new ClickEncoder(A1, A0, A2, 4);

	encoder->setAccelerationEnabled(1);

	Timer1.initialize(1000);
	Timer1.attachInterrupt(timerIsr);



	for (int i = 0; i < 9; i++) {
		Serial.println(i);
		profile[i].eepromReadProfile(i + 1); //Load all profiles from EEPROM at startup
	}

	for (loader; loader < 82; loader++) {
		u8g2.drawBox(23, 49, loader, 6);

		//delay(30);
		u8g2.sendBuffer();
	}
	u8g2.clearBuffer();
	u8g2.drawXBMP(0, 5, 128, 35, logo.maauw_logo_bits);
	u8g2.setFont(u8g2_font_synchronizer_nbp_tf);
	u8g2.drawStr(23, 55, "Maauw Ready");
	delay(700);
	u8g2.sendBuffer();
	delay(1000);
	u8g2.clearDisplay();

	lastValue = -1;
}


//////SETUP//////SETUP//////SETUP//////SETUP//////SETUP//////SETUP//////SETUP//////SETUP//////SETUP/////


//////////LOOP//////////LOOP//////////LOOP//////////LOOP//////////LOOP//////////LOOP//////////LOOP//////////
void loop(void) {

	maauw.readEncoderDirection();
	maauw.readEncoderButton();

	if (maauw.getValveState() == 1){
		maauw.convertToChar(String("Re-light Burner"), menuItem1);
	}
	else {
		maauw.convertToChar(String("Ignite Burner"), menuItem1);
	}

	if (btnPress && page == 1) {
		btnPress = false;
		page = 2;
		selMenuItem = 1;
	}
	if (btnPress == true && page == 2 && selMenuItem == 2) {
		btnPress = false;
		page = 3;
	}

	if (btnPress == true && page == 2 && selMenuItem == 3) {
		btnPress = false;
		page = 66;
	}

	if (btnPress == true && page == 2 && selMenuItem == 4) {
		btnPress = false;
		maauw.extingush();
		page = 1;
	}

	if (btnPress == true && page == 2 && selMenuItem == 1) {
		btnPress = false;
		page = 88;
	}

	if (btnPress == true && page == 6) {
		btnPress = false;
		page = 77;
	}

	if (btnPress == true && page == 3) {
		btnPress = false;
		page = 4;
	}

	if (btnPress == true && page == 4) {
		btnPress = false;
		page = 444;
	}

	drawMenu();
}



void drawMenu() {

	u8g2.clearBuffer();
	u8g2.setFontMode(1);        //Set fonts to transparent mode

	if (page == 1) {

		////////////////////Draw main page////////////////

	//	if (ClickEncoder::Held) {
	//		page = 88;
		//}

		u8g2.drawXBMP(38, 3, 52, 15, logo.maauwS_bits);

		u8g2.drawXBMP(109, 3, 15, 11, logo.wifiL_bits);


		if (maauw.getValveState() == 1) {
			u8g2.drawXBMP(3, 3, 16, 13, logo.fireAl_bits);
		}
		else {
			u8g2.drawXBMP(3, 3, 16, 13, logo.fireNAl_bits);
		}

		// Temperature HUD
		uint16_t tempC = maauw.readExternalTemp();
		u8g2.setFont(u8g2_font_helvB10_tf);
		char tempChar[10]; //Maximum possible array lenth
		maauw.convertToChar((String(tempC) + String("°C")), tempChar);
		u8g2.drawUTF8(maauw.centerLine(tempChar) + 7, 41, tempChar);
		u8g2.drawXBMP(33, 28, 6, 15, logo.thermL_bits);

		//Autopilot HUD
		u8g2.setFont(u8g2_font_chikita_tr);
		u8g2.drawStr(0, 38, "AUTO");

		//Profile HUD
		u8g2.drawStr(105, 38, "PRF1");

		//Bottom Status Bar
		u8g2.setFont(u8g2_font_helvR08_tf);
		char statusTxt[12];
		if (maauw.getValveState() == 1) {
			maauw.convertToChar(String("FIRING TO 10R"), statusTxt);
		}
		else {
			maauw.convertToChar(String("IDLE"), statusTxt);
		}
		u8g2.drawUTF8(maauw.centerLine(statusTxt), 64, statusTxt);


		//profile[6].setExit(false); //Make profiles accessible again
	}

	if (page == 2) {
		maauw.drawTitle("Main Menu");
		if (down == true) {
			down = false;
			prevMenuItem = selMenuItem;
			selMenuItem++;
			if (selMenuItem > 6) {
				selMenuItem = 6;
			}
			if (selMenuItem == 3 && prevMenuItem == 2)
			{
				frame++;
			}
			else  if (selMenuItem == 4 && prevMenuItem == 3)
			{
				frame++;
			}
			else  if (selMenuItem == 5 && prevMenuItem == 4 && frame != 4)
			{
				frame++;
			}
		}
		else if (up == true) {
			up = false;
			prevMenuItem = selMenuItem;
			selMenuItem--;
			if (selMenuItem < 1) {
				selMenuItem = 1;
			}
			if (selMenuItem == 2 && frame == 2)
			{
				frame--;
			}

			if (selMenuItem == 4 && frame == 4)
			{
				frame--;
			}
			if (selMenuItem == 3 && frame == 3)
			{
				frame--;
			}
			Serial.print("Selected Menu Item: ");
			Serial.println(selMenuItem);
			Serial.print("Previous Menu Item: ");
			Serial.println(prevMenuItem);
			Serial.print("Frame: ");
			Serial.println(frame);
		}

		drawMenuItems();

	}

	if (page == 3) {
		maauw.drawTitle(menuItem2);
		u8g2.setFont(u8g2_font_inb19_mn);
		maauw.drawMenuItem(1, "Profile 1", true);
		maauw.drawMenuItem(2, "Profile 2", false);
	}
	if (page == 4) {
		profile[6].setSegs();
		}

	if (page == 444) {
		Serial.println("Page 444");
			if (profile[6].setTRH(7) == 0) {
				page = 1;
		}
	}

	/*if (page == 88) {
		maauw.drawTitle("Debug");
		u8g2.setFont(u8g2_font_helvB10_tf);
		u8g2.setCursor(0, 25);
		u8g2.print(profile[6].switchGetTemp(1));
		u8g2.setCursor(30, 25);
		u8g2.print(profile[6].switchGetRamp(1));
		u8g2.setCursor(60, 25);
		u8g2.print(profile[6].switchGetHold(1));
		u8g2.setCursor(0, 45);
		u8g2.print(profile[6].switchGetTemp(2));
		u8g2.setCursor(30, 45);
		u8g2.print(profile[6].switchGetRamp(2));
		u8g2.setCursor(60, 45);
		u8g2.print(profile[6].switchGetHold(2));
	} */

	if (page == 66) {
		reduce.setReductionUI();
	}

	if (page == 88) {
		if (maauw.ignite() == 0) {
			page = 1;
		}
	}

	btnPress = false;
	u8g2.sendBuffer();
}
