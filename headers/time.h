#ifndef _TIME_H
#define _TIME_H

#include "cmos.h"

int second() {
  return (int)_CMOS_read(0x00, 1);
}

int minute() {
  return (int)_CMOS_read(0x02, 1);
}

int hour() {
  return (int)_CMOS_read(0x04, 1);
}

int day() {
  return (int)_CMOS_read(0x07, 1);
}

int month() {
  return (int)_CMOS_read(0x08, 1);
}

int year() {
  return (int)_CMOS_read(0x09, 1);
}

#endif
