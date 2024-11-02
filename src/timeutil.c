#include "timeutil.h"

const uint8_t MONTH_LEN_LOOKUP[12] =
{
    31, // Jan
    28, // Feb
    31, // Mar
    30, // Apr
    31, // May
    30, // Jun
    31, // Jul
    31, // Aug
    30, // Sep
    31, // Oct
    30, // Nov
    31, // Dec
};

int is_leap_year(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int days_in_month(int year, int month) {
    if (month == 1 && is_leap_year(year)) return 29;
    return MONTH_LEN_LOOKUP[month];
}
