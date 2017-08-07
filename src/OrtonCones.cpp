/*
  OrtonCones.h - Look Up Table for Orton Cone Temperature Equivalents in Celcius.
  Copyright (c) 2017 Syby Abraham.
*/

#include "OrtonCones.h"

// Constructor //
OrtonCones::OrtonCones() {
}

// Public Methods //
float OrtonCones::getCone(int cone)
{
  _cone = cone;

  if (_cone < 586.00) {
    return -255;
  };
  if (_cone >= 586.00 && (_cone < 600.00)) {
    return -22;
  };
  if (_cone >= 600.00 && (_cone < 626.00)) {
    return -21;
  };
  if (_cone >= 626.00 && (_cone < 676.00)) {
    return -20;
  };
  if (_cone >= 676.00 && (_cone < 712.00)) {
    return -19;
  };
  if (_cone >= 712.00 && (_cone < 736.00)) {
    return -18;
  };
  if (_cone >= 736.00 && (_cone < 769.00)) {
    return -17;
  };
  if (_cone >= 769.00 && (_cone < 788.00)) {
    return -16;
  };
  if (_cone >= 788.00 && (_cone < 807.00)) {
    return -15;
  };
  if (_cone >= 807.00 && (_cone < 837.00)) {
    return -14;
  };
  if (_cone >= 837.00 && (_cone < 858.00)) {
    return -13;
  };
  if (_cone >= 858.00 && (_cone < 873.00)) {
    return -12;
  };
  if (_cone >= 873.00 && (_cone < 898.00)) {
    return -11;
  };
  if (_cone >= 898.00 && (_cone < 917.00)) {
    return -10;
  };
  if (_cone >= 917.00 && (_cone < 942.00)) {
    return -9;
  };
  if (_cone >= 942.00 && (_cone < 973.00)) {
    return -8;
  };
  if (_cone >= 973.00 && (_cone < 995.00)) {
    return -7;
  };
  if (_cone >= 995.00 && (_cone < 1012.00)) {
    return -6;
  };
  if (_cone >= 1012.00 && (_cone < 1030.00)) {
    return -5;
  };
  if (_cone >= 1030.00 && (_cone < 1060.00)) {
    return -5.5;
  };
  if (_cone >= 1060.00 && (_cone < 1086.00)) {
    return -4;
  };
  if (_cone >= 1086.00 && (_cone < 1101.00)) {
    return -3;
  };
  if (_cone >= 1101.00 && (_cone < 1117.00)) {
    return -2;
  };
  if (_cone >= 1117.00 && (_cone < 1136.00)) {
    return -1;
  };
  if (_cone >= 1136.00 && (_cone < 1142.00)) {
    return 1;
  };
  if (_cone >= 1142.00 && (_cone < 1152.00)) {
    return 2;
  };
  if (_cone >= 1152.00 && (_cone < 1160.00)) {
    return 3;
  };
  if (_cone >= 1160.00 && (_cone < 1184.00)) {
    return 4;
  };
  if (_cone >= 1184.00 && (_cone < 1203.00)) {
    return 5;
  };
  if (_cone >= 1203.00 && (_cone < 1220.00)) {
    return 5.5;
  };
  if (_cone >= 1220.00 && (_cone < 1237.00)) {
    return 6;
  };
  if (_cone >= 1237.00 && (_cone < 1247.00)) {
    return 7;
  };
  if (_cone >= 1247.00 && (_cone < 1257.00)) {
    return 8;
  };
  if (_cone >= 1257.00 && (_cone < 1282.00)) {
    return 9;
  };
  if (_cone >= 1282.00 && (_cone < 1293.00)) {
    return 10;
  };
  if (_cone >= 1293.00 && (_cone < 1304.00)) {
    return 11;
  };
  if (_cone >= 1304.00 && (_cone < 1321.00)) {
    return 12;
  };
  if (_cone >= 1321.00 && (_cone < 1388.00)) {
    return 13;
  };
  if (_cone >= 1388.00 && (_cone <= 1398.00)) {
    return 14;
  };
  if (_cone > 1398) {
    return 15;
  };
}

// Private Methods /////////////////////////////////////////////////////////////
// Functions only available to other functions in this library

