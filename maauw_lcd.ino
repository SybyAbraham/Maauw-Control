/*
Maauw Control // Automatic Kiln Control Firmware // PRE-ALPHA

Maauw Control Is Designed to Run on the Atmega2560 MCU. 

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

Copyright (c) 2017 Syby Abraham.
*/

#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

#include <ClickEncoder.h>
#include <TimerOne.h>

#include "OrtonCones.h"
#include "Profiles.h"
#include "XBMPLogos.h"
#include "Segments.h"
#include "MaauwOperations.h"

//U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE); //HW I2C OLED
//U8G2_ST7565_ZOLEN_128X64_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
U8G2_ST7565_64128N_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8); //DEV VERSION HW SPI

ClickEncoder *encoder;
int16_t lastValue, value;

void timerIsr() {
  encoder->service();
}

OrtonCones cone;
XBMPLogos logo;
Profiles prf;
Segments seg;
MaauwOperations core;

char menuItem1 [] = "Kiln Settings";
char menuItem2 [] = "Ramp Profiles";
char menuItem3 [] = "Reduction Settings";
char menuItem4 [] = "Network Settings";
char menuItem5 [] = "Display";
char menuItem6 [] = "Maauw Control";

int backlightPin = 6;

int prevMenuItem = 1;
int frame = 1;
int page = 1;
int selMenuItem = 1;

boolean up = false;
boolean down = false;
boolean btnPress = false;

unsigned long previousMillis = 0;
int stateF = 1;

void readEncoderDirection() {
  value += encoder->getValue();

  if (value > lastValue) {
    lastValue = value;
    down = true;

  } else if (value < lastValue) {
    lastValue = value;
    up = true;
  }
}

void readEncoderButton() {
  ClickEncoder::Button b = encoder->getButton();
  if (b != ClickEncoder::Open) {
    switch (b) {
      case ClickEncoder::Clicked:
        btnPress = true;
        break;
      case ClickEncoder::Held:
        page = 1;
        break;
    }
  }
}

void drawMenuItems() {
  if (frame == 1 && selMenuItem == 1) {
    core.drawMenuItem(1, menuItem1, true);
	core.drawMenuItem(2, menuItem2, false);
	core.drawMenuItem(3, menuItem3, false);
  } else if (frame == 1 && selMenuItem == 2) {
	  core.drawMenuItem(1, menuItem1, false);
	  core.drawMenuItem(2, menuItem2, true);
	  core.drawMenuItem(3, menuItem3, false);
  } else if (frame == 1 && selMenuItem == 3) {
	  core.drawMenuItem(1, menuItem1, false);
	  core.drawMenuItem(2, menuItem2, false);
	  core.drawMenuItem(3, menuItem3, true);
  } else if (frame == 2 && selMenuItem == 2) {
	  core.drawMenuItem(1, menuItem2, true);
	  core.drawMenuItem(2, menuItem3, false);
	  core.drawMenuItem(3, menuItem4, false);
  } else if (frame == 2 && selMenuItem == 3) {
	  core.drawMenuItem(1, menuItem2, false);
	  core.drawMenuItem(2, menuItem3, true);
	  core.drawMenuItem(3, menuItem4, false);
  } else if (frame == 2 && selMenuItem == 4) {
	  core.drawMenuItem(1, menuItem2, false);
	  core.drawMenuItem(2, menuItem3, false);
	  core.drawMenuItem(3, menuItem4, true);
  } else if (frame == 3 && selMenuItem == 3) {
	  core.drawMenuItem(1, menuItem3, true);
	  core.drawMenuItem(2, menuItem4, false);
	  core.drawMenuItem(3, menuItem5, false);
  } else if (frame == 3 && selMenuItem == 4) {
	  core.drawMenuItem(1, menuItem3, false);
	  core.drawMenuItem(2, menuItem4, true);
	  core.drawMenuItem(3, menuItem5, false);
  } else if (frame == 3 && selMenuItem == 5) {
	  core.drawMenuItem(1, menuItem3, false);
	  core.drawMenuItem(2, menuItem4, false);
	  core.drawMenuItem(3, menuItem5, true);
  } else if (frame == 4 && selMenuItem == 4) {
	  core.drawMenuItem(1, menuItem4, true);
	  core.drawMenuItem(2, menuItem5, false);
	  core.drawMenuItem(3, menuItem6, false);
  } else if (frame == 4 && selMenuItem == 5) {
	  core.drawMenuItem(1, menuItem4, false);
	  core.drawMenuItem(2, menuItem5, true);
	  core.drawMenuItem(3, menuItem6, false);
  } else if (frame == 4 && selMenuItem == 6) {
	  core.drawMenuItem(1, menuItem4, false);
	  core.drawMenuItem(2, menuItem5, false);
	  core.drawMenuItem(3, menuItem6, true);
  }
}

//////SETUP//////SETUP//////SETUP//////SETUP//////SETUP//////SETUP//////SETUP//////SETUP//////SETUP////

void setup(void) {

  pinMode(backlightPin, OUTPUT);
  int loader = 0;

  u8g2.begin();
  u8g2.setFlipMode(1);
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.drawXBMP( 0, 5, 128, 35, logo.maauw_logo_bits);
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
  for (loader; loader < 82; loader++) {
    u8g2.drawBox(23, 49, loader, 6);
    //delay(30);
    u8g2.sendBuffer();
  }
  u8g2.clearBuffer();
  u8g2.drawXBMP( 0, 5, 128, 35, logo.maauw_logo_bits);
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

  readEncoderDirection();
  readEncoderButton();

  if (btnPress && page == 1 ) {
    btnPress = false;
    page = 2;
    selMenuItem = 1;
  }
  if (btnPress == true && page == 2 && selMenuItem == 2) {
    btnPress = false;
    page = 3;
  }

  if (btnPress == true && page == 2 && selMenuItem == 5) {
    btnPress = false;
    page = 66;
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
    page = 5;
  }

  if (btnPress == true && page == 5) {
    btnPress = false;
    page = 6;
  }


  drawMenu();
}



void drawMenu() {

  u8g2.clearBuffer();
  u8g2.setFontMode(1);        //Set fonts to transparent mode

  if (page == 1) {    //Draw main page
    u8g2.drawXBMP(38, 3, 52, 15, logo.maauwS_bits);

    u8g2.drawXBMP(109, 3, 15, 11, logo.wifiL_bits);


    if (stateF == 1) {
      u8g2.drawXBMP(3, 3, 16, 13, logo.fireAl_bits);
    }

    int tempC = 432;

    u8g2.setFont(u8g2_font_helvB10_tf);
    char tempChar[10]; //Maximum possible array lenth
    core.convertToChar((String(tempC) + String("°C")), tempChar);
    u8g2.drawUTF8(core.centerLine(tempChar) + 7, 41, tempChar);
    u8g2.drawXBMP(33, 28, 6, 15, logo.thermL_bits);

    u8g2.setFont(u8g2_font_helvR08_tf);
    char statusTxt[] = "FIRING TO 10OX";
    int mmx = (128 - u8g2.getStrWidth(statusTxt)) / 2;        //Math for centering
    u8g2.drawUTF8(mmx, 64, statusTxt);
  }

  if (page == 2) {

    core.drawTitle("Main Menu");
    if (down == true) {
      down = false;
      prevMenuItem = selMenuItem;
      selMenuItem++;
      if (selMenuItem > 6) {
        selMenuItem = 6;
      }
      if (selMenuItem == 3 && prevMenuItem == 2)
      {
        frame ++;
      } else  if (selMenuItem == 4 && prevMenuItem == 3)
      {
        frame ++;
      }
      else  if (selMenuItem == 5 && prevMenuItem == 4 && frame != 4)
      {
        frame ++;
      }
    } else if (up == true) {
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
    core.drawTitle(menuItem2);
    u8g2.setFont(u8g2_font_inb19_mn);
    core.drawMenuItem(1, "Profile 1", true);
    core.drawMenuItem(2, "Profile 2", false);
  }

  if (page == 4) {
    prf.setSegs("Ramp Segments");
  }

  if (page == 5) {
	  seg.setHold(1);
  }

  if (page == 6) {
    prf.setRamp("Ramp Segment 1");
  }

  if (page == 66) {
    core.drawTitle("Brightness");
    int brightness = value;

    if (brightness <= 0) {
      value = 0;
      brightness = 0;
    } else if (brightness >= 255) {
      value = 255;
      brightness = 255;
    }

    analogWrite(backlightPin, brightness);

    u8g2.setFont(u8g2_font_helvB10_tf);
    u8g2.setCursor(55, 48);
    u8g2.print(brightness);
  }

  if (page == 77) {
    core.drawTitle("Contrast");
    int contrast = value;
    u8g2.setContrast(contrast);
    u8g2.setCursor(55, 48);
    u8g2.print(contrast);
  }

  if (page == 88) {
    core.drawTitle("Get Temp");
    u8g2.setFont(u8g2_font_helvB10_tf);
    u8g2.setCursor(55, 48);
    u8g2.print(prf.getTemp());
  }

  btnPress = false;
  u8g2.sendBuffer();
}
