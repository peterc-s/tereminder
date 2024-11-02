#ifndef TIMEUTIL_H
#define TIMEUTIL_H

#include <stdint.h>

extern const uint8_t MONTH_LEN_LOOKUP[12];

int is_leap_year(int year);
int days_in_month(int year, int month);

#endif
