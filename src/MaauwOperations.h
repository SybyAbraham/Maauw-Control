/*
MaauwOperations.h - Maauw Automatic Kiln Control Firmware Core Functions
Copyright (c) 2017 Syby Abraham.
*/

#ifndef MaauwOperations_h
#define MaauwOperations_h

#include "Arduino.h"

#include <U8g2lib.h>
#include <ClickEncoder.h>
#include <Adafruit_MAX31855.h>
#include <PID_v1.h>
#include "XBMPLogos.h"

//extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;
extern U8G2_ST7565_64128N_F_4W_HW_SPI u8g2;

extern uint16_t lastValue, value;
extern ClickEncoder *encoder;
extern uint16_t page;
extern boolean up, down, btnPress;
extern unsigned long currentMillis;
extern XBMPLogos logo;

static Adafruit_MAX31855 max31855(3); //HW SPI / Chip Select Pin 3

class MaauwOperations {

public:
	//UI Functions
	MaauwOperations();
	void convertToChar(String, char[]);
	int centerLine(char[]);
	void drawTitle(char[]);
	void drawMenuItem(int, char[], bool);
	void readEncoderDirection();
	void readEncoderButton();

	//Firing Functions
	uint8_t ignite(void);
	uint16_t readPyro(void);
	uint16_t readBurner(void);
	uint16_t readExternalTemp(void);
	bool getValveState();
	void extingush();

private:
	bool _valveState;
	bool _ignState;
	uint8_t PROGMEM _valvepin;
	uint8_t PROGMEM _ignpin;
	unsigned long _igntimer;
	unsigned long _ignExit;
};

#endif