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
	void eepromWriteProfile(int prfN);
	void eepromReadProfile(int prfN);
	unsigned int switchGetTemp(int);
	unsigned int switchGetRamp(int);
	unsigned long switchGetHold(int); 
	bool getExit(void);
	void setExit(bool);

  private:
	//Create 9 segment objects for each profile
	  Segments seg[8];

	//Variables
    int8_t _segs;
	int8_t _selParam;
	int8_t _currentSeg;
	int32_t _eeAddress;
	bool _exitState;

	//Methods
	void switchSetTemp(int);
	void switchSetRamp(int);
	void switchSetHold(int); 
	
};

#endif