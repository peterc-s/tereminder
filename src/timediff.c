#include "timediff.h"

tm_diff_t time_diff(struct tm time1, struct tm time2) {
    tm_diff_t diff = {
        .years  = time1.tm_year - time2.tm_year,
        .months = time1.tm_mon  - time2.tm_mon,
        .days   = time1.tm_mday - time2.tm_mday,
        .hours  = time1.tm_hour - time2.tm_hour,
        .mins   = time1.tm_min  - time2.tm_min,
        .secs   = time1.tm_sec  - time2.tm_sec,
    };

    return diff;
}
