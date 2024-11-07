#include <stdint.h>
#include <stdlib.h>

#include "timeutil.h"
#include "timediff.h"

tm_diff_t time_diff(struct tm time1, struct tm time2) {
    tm_diff_t diff = {0};

    time_t t1 = mktime(&time1);
    time_t t2 = mktime(&time2);
    
    // make sure t1 is always first
    if (difftime(t1, t2) < 0) {
        diff.negative = 1;
        struct tm temp = time1;
        time1 = time2;
        time2 = temp;
    }

    // calculate year and month diffs
    diff.years = time1.tm_year - time2.tm_year;
    diff.months = time1.tm_mon - time2.tm_mon;

    // adjust years and months if months are negative
    if (diff.months < 0) {
        diff.months += 12;
        diff.years--;
    }

    // calculate day difference
    if (time1.tm_mday >= time2.tm_mday) {
        diff.days = time1.tm_mday - time2.tm_mday;
    } else {
        // adjust years and months
        diff.months--; 
        if (diff.months < 0) {
            diff.months += 12;
            diff.years--;
        }
        
        // calculate day difference
        int previous_month = (time1.tm_mon - 1 + 12) % 12;
        int previous_year = time1.tm_year + (time1.tm_mon == 0 ? -1 : 0);
        diff.days = (time1.tm_mday + days_in_month(previous_year + 1900, previous_month)) - time2.tm_mday;
    }

    diff.secs = time1.tm_sec - time2.tm_sec;
    if (diff.secs < 0) {
        diff.secs += 60;
        diff.mins--;
    }

    diff.mins += (time1.tm_min - time2.tm_min);
    if (diff.mins < 0) {
        diff.mins += 60;
        diff.hours--;
    }

    diff.hours += (time1.tm_hour - time2.tm_hour);
    if (diff.hours < 0) {
        diff.hours += 24;
        diff.days--;
    }

    return diff;
}
