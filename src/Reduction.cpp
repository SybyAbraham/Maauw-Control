/*
Reduction.h - Segmented Reduction Firing Library for the Maauw Automatic Kiln
Copyright (c) 2017 Syby Abraham.
*/

#include "Reduction.h"

Reduction::Reduction()
{
}
/*
void Reduction::setRSegsUI() {
	core.drawTitle("Reduction Segments");
	u8g2.setFont(u8g2_font_crox1h_tf);
	u8g2.drawStr(core.centerLine("Enter number of segments"), 28, "Enter number of segments");

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
	core.convertToChar(String(_rsegs), segChar);
	u8g2.setFont(u8g2_font_helvB10_tf);
	u8g2.drawUTF8(core.centerLine(segChar), 48, segChar);
} */