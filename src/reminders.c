#include <stdio.h>
#include <time.h>
#include <stdint.h>

#include "reminders.h"

int print_reminder(reminder_t* rem) {
    char buf[70];

    if(!strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%SZ", &rem->due)) {
        return FORMAT_TIME_FAIL;
    }

    printf("%s ", buf);

    switch (rem->severity) {
        case Routine:
            printf("R  ");
            break;
        case Low:
            printf("0  ");
            break;
        case Medium:
            printf("!  ");
            break;
        case High:
            printf("!! ");
            break;
    }

    printf("%s: %s\n", rem->title, rem->description);

    return PRINT_OK;
}
