/*
Reduction.h - Segmented Reduction Firing Library for the Maauw Automatic Kiln
Copyright (c) 2017 Syby Abraham.
*/

#ifndef Reduction_h
#define Reduction_h

#include "Arduino.h"


class Reduction
{
public:
	Reduction();
	void setRSegsUI();

private:
	uint8_t _rsegs;
};

#endif