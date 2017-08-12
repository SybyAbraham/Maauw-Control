/*
  Segments.h - Segmented Temperature, Ramp and Hold Adjustment Library for the Maauw Automatic Kiln
  Copyright (c) 2017 Syby Abraham.
*/

#ifndef Segments_h
#define Segments_h

#include "Arduino.h"
#include <U8g2lib.h>
#include "OrtonCones.h"
#include <U8g2lib.h>
#include "XBMPLogos.h"
#include <ClickEncoder.h>
#include "MaauwOperations.h"


extern uint16_t lastValue, value;
extern bool btnPress;
//extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2; //HW I2C OLED
extern U8G2_ST7565_64128N_F_4W_HW_SPI u8g2; //DEV VERSION HW SPI LCD
extern OrtonCones cone;
extern XBMPLogos logo;
extern MaauwOperations core;

class Segments{
  public:
    Segments();
	//Gets Temp, Ramp or Hold from the private _temp, _ramp or _hold private variables, respectively
    unsigned int getTemp();
    unsigned int getRamp();
    unsigned int getHold();

	//Writes Temp, Ramp or Hold to the _temp, _ramp or _hold private variables, respectively
	void setTemp(uint16_t);
	void setRamp(uint16_t);
	void setHold(uint16_t);

	//User interface(LCD) for setting the Temp, Ramp or Hold variables
    void setTempUI(uint8_t);
    void setRampUI(uint8_t);
    void setHoldUI(uint8_t);

  private:
    char _tempChar[10];
	uint16_t _temp; //Temp in °C
	uint16_t _ramp; //Ramp rate in °C/HR
	uint16_t _hold; //Hold time in minutes
	uint8_t _holdHours;
	uint8_t _holdMinutes;
    
};
#endif