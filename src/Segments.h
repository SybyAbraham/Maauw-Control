/*
  Segments.h - Segmented Temperature, Ramp and Hold Adjustment Library for the Maauw Automatic Kiln
  Copyright (c) 2017 Syby Abraham.
*/

#ifndef Segments_h
#define Segments_h

#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <U8g2lib.h>

#include "OrtonCones.h"
#include <U8g2lib.h>
#include "XBMPLogos.h"
#include <ClickEncoder.h>
#include "MaauwOperations.h"


extern int16_t lastValue, value;
extern bool btnPress;
//extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2; //HW I2C OLED
extern U8G2_ST7565_64128N_F_4W_HW_SPI u8g2; //DEV VERSION HW SPI LCD
extern OrtonCones cone;
extern XBMPLogos logo;
extern MaauwOperations core;

class Segments{
  public:
    Segments();
    int getTemp();
    int getRamp();
    int getHold();
    void setTemp(int);
    void setRamp(int);
    void setHold(int);

  private:
    char _tempChar[10];
	int16_t _temp; //Temp in °C
	int16_t _ramp; //Ramp rate in °C/HR
	int8_t _holdHours;
	int8_t _holdMinutes;
    unsigned long _hold; //Hold time in millis
};
#endif