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

class Profiles : public Segments {

  public:
    Profiles();
    void setSegs(char[]);
    void switchSetTemp(int);
	void switchSetRamp(int);
	void switchSetHold(int);
    void review();
	void setTRH(int8_t);
	int getSelParam();

  private:
    Segments seg[8]; //Create 9 segments for each profile
    int8_t _segs;
	int8_t _selParam;
    int _getSegs();
    void _getVolatileSeg(void);
};

#endif