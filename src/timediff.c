#include <stdint.h>

#include "timeutil.h"
#include "timediff.h"

tm_diff_t time_diff(struct tm time1, struct tm time2) {
    tm_diff_t diff = { 0 };

    time_t t1 = mktime(&time1);
    time_t t2 = mktime(&time2);
 
    double diff_seconds = difftime(t1, t2);
    diff.sign = (diff_seconds < 0) ? -1 : 1;
    diff_seconds = (diff_seconds < 0) ? -diff_seconds : diff_seconds;

    diff.secs = (int)diff_seconds % 60;
    diff.mins = ((int)diff_seconds / 60) % 60;
    diff.hours = ((int)diff_seconds / 3600) % 24;

    int total_days = (int)(diff_seconds / (3600 * 24));
    
    while (total_days >= 0) {
        int days_in_current_month = MONTH_LEN_LOOKUP[time2.tm_mon];
        
        if (time2.tm_mon == 1 && !is_leap_year(time2.tm_year + 1900)) {
            days_in_current_month = 28;
        }

        if (total_days < days_in_current_month) {
            break;
        }
        
        total_days -= days_in_current_month;
        time2.tm_mon++;
        
        if (time2.tm_mon == 12) {
            time2.tm_mon = 0;
            time2.tm_year++;
        }
    }

    diff.years = time2.tm_year - time1.tm_year;
    diff.months = time2.tm_mon - time1.tm_mon;

    if (diff.months < 0) {
        diff.months += 12;
        diff.years--;
    }

    diff.days = total_days;

    return diff;
}
