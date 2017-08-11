/*
  Profiles.h - Firing Profile Management Library for the Maauw Automatic Kiln
  Copyright (c) 2017 Syby Abraham.
*/

#ifndef Profiles_h
#define Profiles_h

#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "Segments.h"
#include <EEPROM.h>

class Profiles : public Segments {

  public:
	//Constructor
    Profiles();

	//Methods
    void setSegs(void);
    void review();
	void setTRH(int);
	void eepromWriteProfile(uint8_t prfN);
	void eepromReadProfile(uint8_t prfN);
	uint16_t switchGetTemp(uint8_t);
	uint16_t switchGetRamp(uint8_t);
	uint16_t switchGetHold(uint8_t);
	bool getExit(void);
	void setExit(bool);

  private:
	//Create 9 segment objects for each profile
	  Segments seg[9];

	//Variables
    int8_t _segs;
	int8_t _selParam;
	int8_t _currentSeg;
	int32_t _eeAddress;
	bool _exitState;

	//Methods
	void switchSetTemp(uint8_t);
	void switchSetRamp(uint8_t);
	void switchSetHold(uint8_t);
	
};

#endif