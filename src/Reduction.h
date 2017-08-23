/*
Reduction.h - Segmented Reduction Firing Library for the Maauw Automatic Kiln
Copyright (c) 2017 Syby Abraham.
*/

#ifndef Reduction_h
#define Reduction_h

#include "Arduino.h"
#include "Segments.h"


class Reduction : public Segments
{
public:
	Reduction();
	void setRSegsUI();
	void setReductionUI();

private:
	uint8_t _rsegs;
	int8_t _reductVal;
};

#endif