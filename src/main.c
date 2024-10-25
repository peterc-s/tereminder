#include <time.h>

#include "reminders.h"

int main(void) {
    
    struct tm test_time = {
        .tm_year = 124,
        .tm_mon  = 10,
        .tm_mday = 25,
        .tm_hour = 22,
    };

    reminder_t reminder = {
        .due = test_time,
        .severity = High,
        .title = "Hello",
        .description = "World"
    };

    print_reminder(&reminder);
}
