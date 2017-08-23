/*
  Profiles.h - Firing Profile Management Library for the Maauw Automatic Kiln
  Copyright (c) 2017 Syby Abraham.
*/

#ifndef Profiles_h
#define Profiles_h

#include "Arduino.h"
#include "Segments.h"
#include <EEPROM.h>

extern unsigned long currentMillis;

class Profiles:Segments{

  public:
	//Constructor
    Profiles();

	//Methods
    void reviewUI(uint8_t prfN);
	uint16_t switchGetTemp(uint8_t);
	uint16_t switchGetRamp(uint8_t);
	uint16_t switchGetHold(uint8_t);
	void eepromReadProfile(uint8_t);
	uint8_t setTRH(uint8_t);
	void setSegs(void);

  private:
	//Create 9 segment objects for each profile
	Segments seg[9];

	//Variables
    int8_t _segs;
	int8_t _selParam;
	int8_t _currentSeg;
	int32_t _eeAddress;
	unsigned long _saveTimer;
	bool _exit;

	//Methods
	

	void eepromWriteProfile(uint8_t);
	void switchSetTemp(uint8_t);
	void switchSetRamp(uint8_t);
	void switchSetHold(uint8_t);
	
};

#endif