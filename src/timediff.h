#ifndef TIMEDIFF_H
#define TIMEDIFF_H

#include <time.h>

typedef struct tm_diff {
    int years;
    int months;
    int days;
    int hours;
    int mins;
    int secs;
    int sign;
} tm_diff_t;

tm_diff_t time_diff(struct tm time1, struct tm time2);

#endif
