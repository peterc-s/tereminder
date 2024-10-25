#ifndef REMINDERS_H
#define REMINDERS_H

#include <time.h>

#define PRINT_OK 0
#define FORMAT_TIME_FAIL 1

typedef enum Severity {
    Routine = 0,
    Low = 1,
    Medium = 2,
    High = 3,
} Severity;

typedef struct reminder {
    struct tm due;
    Severity severity;
    char* title;
    char* description;
} reminder_t;

int print_reminder(reminder_t* rem);

#endif
