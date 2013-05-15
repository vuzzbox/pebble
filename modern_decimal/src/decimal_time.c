/**
 * Convert archaic time to decimal time. 
 * Learn more here: http://en.wikipedia.org/wiki/Decimal_time)
 *  
 * Copyright (c) 2013 Ed McLaughlin <epmclaughlin@gmail.com>
 */

#include <math.h>
#include "pebble_os.h"
#include "decimal_time.h"

void get_decimal_time(PblDeciTm *t) {
  PblTm archaic_time;
  unsigned int decimal_time;
  int decimal_hour;
  int decimal_minute;
  int decimal_seconds;

  get_time(&archaic_time);
  decimal_time = floor(((archaic_time.tm_hour * 3600) + (archaic_time.tm_min * 60) + archaic_time.tm_sec) / .864);

  decimal_hour = floor(decimal_time / 10000);
  decimal_time = decimal_time - (decimal_hour * 10000);

  decimal_minute = floor(decimal_time / 100);
  decimal_time = decimal_time - (decimal_minute * 100);
  
  decimal_seconds = decimal_time;

  t->tm_sec = decimal_seconds;
  t->tm_min = decimal_minute;
  t->tm_hour = decimal_hour;
  t->tm_mday = archaic_time.tm_mday;
  t->tm_mon = archaic_time.tm_mon;
  t->tm_year = archaic_time.tm_year;
  t->tm_wday = archaic_time.tm_wday;
  t->tm_yday = archaic_time.tm_yday;
  t->tm_isdst = archaic_time.tm_isdst;
}
