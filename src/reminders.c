#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "timeutil.h"
#include "reminders.h"
#include "ansi.h"
#include "timediff.h"

int print_reminder(reminder_t* rem) {
    // get current time
    time_t current_time_epoch = time(NULL);
    struct tm current_time = *localtime(&current_time_epoch);

    // get difference
    tm_diff_t due_diff = time_diff(rem->due, current_time);

    // represent the difference
    printf(ANSI_UNDERLINE ANSI_BOLD ANSI_COLOR_MAGENTA);
    if (due_diff.years != 0) {
        if (due_diff.sign > 0) {
            printf("Due in ");
        } else {
            printf("Overdue by ");
        }

        printf("%d years", abs(due_diff.years));

        if (due_diff.months != 0) {
            printf(", %d months", abs(due_diff.months));
        }
    } else if (due_diff.months != 0) {
        if (due_diff.sign > 0) {
            printf("Due in ");
        } else {
            printf("Overdue by ");
        }

        printf("%d months", abs(due_diff.months));

        if (due_diff.days != 0) {
            printf(", %d days", abs(due_diff.days));
        }
    } else if (due_diff.days != 0) {
        if (due_diff.sign > 0) {
            printf("Due in ");
        } else {
            printf("Overdue by ");
        }

        printf("%d days", abs(due_diff.days));

        if (due_diff.hours != 0) {
            printf(", %d hours", abs(due_diff.hours));
        }
    } else if (due_diff.hours != 0) {
        if (due_diff.sign > 0) {
            printf("Due in ");
        } else {
            printf("Overdue by ");
        }

        printf("%d hours", abs(due_diff.hours));

        if (due_diff.mins != 0) {
            printf(", %d minutes", abs(due_diff.mins));
        }
    } else if (due_diff.mins != 0) {
        if (due_diff.sign > 0) {
            printf("Due in ");
        } else {
            printf("Overdue by ");
        }

        printf("%d minutes", abs(due_diff.mins));

        if (due_diff.secs != 0) {
            printf(", %d seconds", abs(due_diff.secs));
        }
    } else if (due_diff.secs != 0) {
        if (due_diff.sign > 0) {
            printf("Due in ");
        } else {
            printf("Overdue by ");
        }

        printf("%d seconds", abs(due_diff.secs));
    } else {
        printf("Due now");
    }

    printf(ANSI_RESET " ");

    // print severity
    switch (rem->severity) {
        case Routine:
            printf(ANSI_BOLD ANSI_COLOR_BLUE "R ");
            break;
        case Low:
            printf(ANSI_BOLD ANSI_COLOR_GREEN "L ");
            break;
        case Medium:
            printf(ANSI_BOLD ANSI_COLOR_YELLOW "M ");
            break;
        case High:
            printf(ANSI_BOLD ANSI_COLOR_RED "H ");
            break;
    }

    // print title and description
    printf(ANSI_BOLD "%s:" ANSI_RESET, rem->title);
    printf(ANSI_ITALIC " %s\n" ANSI_RESET, rem->description);

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
    char year_str[5], month_str[3], day_str[3], hour_str[3], min_str[3], sec_str[3];
    int16_t year;
    uint8_t month, day, hour, min, sec;

    while (file_contents != file_end) {
        ////
        // parse due date
        // YYYY-MM-DDTHH:MM:SS
        
        // year
        memcpy(year_str, file_contents, 4);
        year_str[4] = '\0';
        // tm_year is time since 1900
        year = atoi(year_str) - 1900;
        file_contents += 5;

        // month
        memcpy(month_str, file_contents, 2);
        month_str[2] = '\0';
        month = atoi(month_str) - 1;
        if (month == 0 || month > 11) {
            fprintf(stderr, "ERROR: Malformed date in reminder, got month of %d (should be 1-12).\n", month);
            exit(EXIT_FAILURE);
        }
        file_contents += 3;

        // day
        memcpy(day_str, file_contents, 2);
        day_str[2] = '\0';
        day = atoi(day_str);
        if (day == 0 || day > MONTH_LEN_LOOKUP[(month%12)-1]) {
            fprintf(stderr, "ERROR: Malformed date in reminder, got day of %d with month %d (should be 1-%d).\n", day, month, MONTH_LEN_LOOKUP[(month%12)-1]);
            exit(EXIT_FAILURE);
        }
        file_contents += 3;

        // hour
        memcpy(hour_str, file_contents, 2);
        hour_str[2] = '\0';
        hour = atoi(hour_str);
        if (hour > 23) {
            fprintf(stderr, "ERROR: Malformed date in reminder, got hour of %d (should be 0-23).\n", hour);
            exit(EXIT_FAILURE);
        }
        file_contents += 3;

        // minute
        memcpy(min_str, file_contents, 2);
        min_str[2] = '\0';
        min = atoi(min_str);
        if (min > 59) {
            fprintf(stderr, "ERROR: Malformed date in reminder, got minute of %d (should be 0-59).\n", min);
            exit(EXIT_FAILURE);
        }
        file_contents += 3;

        // second
        memcpy(sec_str, file_contents, 2);
        sec_str[2] = '\0';
        sec = atoi(sec_str);
        if (sec > 59) {
            fprintf(stderr, "ERROR: Malformed date in reminder, got second of %d (should be 0-60).\n", sec);
            exit(EXIT_FAILURE);
        }

        ////
        // parse severity
        
        Severity temp_sev;

        // skip to next space
        for(; *file_contents != ' '; ++file_contents);
        switch (*(++file_contents)) {
            case 'R':
                temp_sev = Routine;
                break;
            case 'L':
                temp_sev = Low;
                break;
            case 'M':
                temp_sev = Medium;
                break;
            case 'H':
                temp_sev = High;
                break;
            default:
                //TODO: error handling
                temp_sev = Routine;
                break;
        }

        ////
        // parse title
        
        // skip to next space
        for(; *file_contents != ' '; ++file_contents);
        ++file_contents;

        // get length of title
        size_t title_len = 0;
        for (; *file_contents != ':'; ++file_contents, ++title_len);
        file_contents -= title_len;

        // get title
        char* title = (char*)malloc((title_len + 1) * sizeof(char));
        if (!title && errno != 0) {
            perror("ERROR:");
            exit(errno);
        }
        memcpy(title, file_contents, title_len);
        title[title_len] = '\0';

        ////
        // parse description

        // skip to next space
        for(; *file_contents != ' '; ++file_contents);
        ++file_contents;

        // get length of description
        size_t desc_len = 0;
        for (; *file_contents != '\n' && *file_contents != '\0'; ++file_contents, ++desc_len);
        file_contents -= desc_len;

        // get description
        char* desc = (char*)malloc((desc_len + 1) * sizeof(char));
        if(!desc && errno != 0) {
            perror("ERROR:");
            exit(errno);
        }
        memcpy(desc, file_contents, desc_len);
        desc[desc_len] = '\0';

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
            .severity = temp_sev,
            .title = title,
            .description = desc,
        };

        // add to reminders
        reminders[reminder_i] = temp_reminder;
        ++reminder_i;

        // skip to next line
        while(*file_contents != '\n') ++file_contents;
        ++file_contents;
    }

    reminder_arr_t out_arr = {
        .arr = reminders,
        .size = reminder_count,
    };

    return out_arr;
}
