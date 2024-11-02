#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "reminders.h"

//TODO: leap years
static uint8_t month_length_lut[12] =
{
    31, // Jan
    29, // Feb
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

int print_reminder(reminder_t* rem) {
    char buf[70];

    if(!strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%S", &rem->due)) {
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

reminder_arr_t parse_file(char* file_contents) {
    char* file_end = file_contents + strlen(file_contents);

    size_t reminder_count = 0;
    for (size_t i = 0; file_contents[i]; ++i) {
        if (file_contents[i] == '\n') ++reminder_count;
    }
    size_t reminder_i = 0;

    reminder_t* reminders = (reminder_t*)malloc(reminder_count * sizeof(reminder_t));
    uint16_t year;
    uint8_t month, day, hour, min, sec;

    while (file_contents != file_end) {
        // YYYY-MM-DDTHH:MM:SS
        
        year = atoi(file_contents);
        file_contents += 5;
        month = atoi(file_contents);
        if (month == 0 || month > 12) {
            fprintf(stderr, "ERROR: Malformed date in reminder, got month of %d (should be 1-12).\n", month);
            exit(EXIT_FAILURE);
        }
        file_contents += 3;
        day = atoi(file_contents);
        if (day == 0 || day > month_length_lut[(month%12)-1]) {
            fprintf(stderr, "ERROR: Malformed date in reminder, got day of %d with month %d (should be 1-%d).\n", day, month, month_length_lut[(month%12)-1]);
            exit(EXIT_FAILURE);
        }
        file_contents += 3;
        hour = atoi(file_contents);
        if (hour > 23) {
            fprintf(stderr, "ERROR: Malformed date in reminder, got hour of %d (should be 0-23).\n", hour);
            exit(EXIT_FAILURE);
        }
        file_contents += 3;
        min = atoi(file_contents);
        if (min > 59) {
            fprintf(stderr, "ERROR: Malformed date in reminder, got minute of %d (should be 0-59).\n", min);
            exit(EXIT_FAILURE);
        }
        file_contents += 3;
        sec = atoi(file_contents);
        if (sec > 59) {
            fprintf(stderr, "ERROR: Malformed date in reminder, got second of %d (should be 0-60).\n", sec);
            exit(EXIT_FAILURE);
        }

        struct tm temp_due = {
            .tm_year = year,
            .tm_mon = month,
            .tm_mday = day,
            .tm_hour = hour,
            .tm_min = min,
            .tm_sec = sec,
        };

        reminder_t temp_reminder = {
            .due = temp_due,
            .severity = High,
            .title = "TEST",
            .description = "TEST!",
        };

        //debug
        print_reminder(&temp_reminder);

        reminders[reminder_i] = temp_reminder;
        ++reminder_i;

        while(*file_contents != '\n') ++file_contents;
        ++file_contents;
    }

    reminder_arr_t out_arr = {
        .arr = reminders,
        .size = reminder_count,
    };

    return out_arr;
}
