/*
  XBMPLogos.h - Logos for the Maauw Display Interface
  Copyright (c) 2017 Syby Abraham.
*/

#ifndef XBMPLogos_h
#define XBMPLogos_h

#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
class XBMPLogos {
  public:
    XBMPLogos();
    static const unsigned char PROGMEM maauw_logo_bits[];
    static const unsigned char PROGMEM  maauwS_bits[];
    static const unsigned char PROGMEM thermL_bits[];
    static const unsigned char PROGMEM wifiL_bits[];
    static const unsigned char PROGMEM fireAl_bits[];
    static const unsigned char PROGMEM fireNAl_bits[];
    static const unsigned char PROGMEM cone11px_bits[];
};

#endif
