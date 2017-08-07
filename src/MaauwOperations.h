/*
MaauwOperations.h - Maauw Automatic Kiln Control Firmware Core Functions
Copyright (c) 2017 Syby Abraham.
*/

#ifndef MaauwOperations_h
#define MaauwOperations_h

#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <U8g2lib.h>

extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;
//extern U8G2_ST7565_64128N_F_4W_HW_SPI u8g2;

class MaauwOperations
{
  public:
    MaauwOperations();
    void convertToChar(String , char[]);
    int centerLine(char[]);
	void drawTitle(char[]);
	void drawMenuItem(int, char[], bool);
};

#endif