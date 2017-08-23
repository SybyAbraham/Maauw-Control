/*
Reduction.cpp - Segmented Reduction Firing Library for the Maauw Automatic Kiln
Copyright (c) 2017 Syby Abraham.
*/

#include "Reduction.h"

Reduction::Reduction()
{
}

void Reduction::setRSegsUI() {
	maauw.drawTitle("Reduction Segments");
	u8g2.setFont(u8g2_font_crox1h_tf);
	u8g2.drawStr(maauw.centerLine("Enter number of segments"), 28, "Enter number of segments");

	_rsegs = value;

	if (_rsegs >= 3) { //Value Clamp
		value = 3;
		_rsegs = 3;
	}
	else if (_rsegs <= 1) {
		value = 1;
		_rsegs = 1;
	}

	char segChar[2];
	maauw.convertToChar(String(_rsegs), segChar);
	u8g2.setFont(u8g2_font_helvB10_tf);
	u8g2.drawUTF8(maauw.centerLine(segChar), 48, segChar);
} 

void Reduction::setReductionUI(){

maauw.drawTitle("Reduction Segment 1");
u8g2.setFont(u8g2_font_crox1h_tf);
u8g2.drawStr(maauw.centerLine("Reduction amount"), 28, "Reduction amount");

_reductVal = value;
char _reductChar[5];

if (_reductVal >= 100) { //Value Clamp
	value = 100;
	_reductVal = 100;
}
else if (_reductVal <= 0) {
	value = 0;
	_reductVal = 0;
}

int8_t _reductBar = map(_reductVal, 0, 100, 0, 82);

u8g2.setFont(u8g2_font_helvB10_tf);
maauw.convertToChar((String(_reductVal) + String("%")), _reductChar);
u8g2.drawUTF8(maauw.centerLine(_reductChar), 47, _reductChar);

u8g2.drawFrame(23, 54, 82, 9);
u8g2.drawBox(23, 54, _reductBar, 9);

}