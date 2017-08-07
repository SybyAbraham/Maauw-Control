/*
  ConeLUT.h - Look Up Table for Orton Cone Temperature Equivalents in Celcius.
  Copyright (c) 2017 Syby Abraham.
*/

#ifndef OrtonCones_h
#define OrtonCones_h

#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

// library interface description
class OrtonCones
{
    // Public Interface
  public:
    //Constructor
    OrtonCones();

    //Methods
    float getCone(int cone);

    // Private Interface
  private:
    int _cone;
};

#endif


