/*
 * Decimal Time routines
 * Copyright (c) 2013 Ed McLaughlin <epmclaughlin@gmail.com>
 */
#include "pebble_os.h"

typedef struct PblDeciTm
{
  int tm_sec;
  int tm_min;
  int tm_hour;
  int tm_mday;
  int tm_mon;
  int tm_year;
  int tm_wday;
  int tm_yday;
  int tm_isdst;
} PblDeciTm;

void get_decimal_time(PblDeciTm *t);
uint32_t get_epoch_seconds(PblTm *t);
