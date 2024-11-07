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
    if (
            due_diff.years == 0 &&
            due_diff.months == 0 &&
            due_diff.days == 0 &&
            due_diff.hours == 0 &&
            due_diff.mins == 0 &&
            due_diff.secs == 0
        ) {
        printf(ANSI_UNDERLINE ANSI_BOLD ANSI_COLOR_YELLOW "Due now");
    } else {
        if (due_diff.negative) {
            printf(ANSI_UNDERLINE ANSI_BOLD ANSI_COLOR_RED "Overdue by ");
        } else {
            printf(ANSI_UNDERLINE ANSI_BOLD ANSI_COLOR_MAGENTA "Due in ");
        }

        if (due_diff.years != 0) {
            printf("%d year%s", abs(due_diff.years), (due_diff.years > 1) ? "s" : "");

            if (due_diff.months != 0) {
                printf(", %d month%s", abs(due_diff.months), (due_diff.months > 1) ? "s" : "");
            }

        } else if (due_diff.months != 0) {
            printf("%d month%s", abs(due_diff.months), (due_diff.months > 1) ? "s" : "");

            if (due_diff.days != 0) {
                printf(", %d day%s", abs(due_diff.days), (due_diff.days > 1) ? "s" : "");
            }

        } else if (due_diff.days != 0) {
            printf("%d day%s", abs(due_diff.days), (due_diff.days > 1) ? "s" : "");

            if (due_diff.hours != 0) {
              printf(", %d hour%s", abs(due_diff.hours), (due_diff.hours > 1) ? "s" : "");
            }

        } else if (due_diff.hours != 0) {
            printf("%d hour%s", abs(due_diff.hours), (due_diff.hours > 1) ? "s" : "");

            if (due_diff.mins != 0) {
                printf(", %d minute%s", abs(due_diff.mins), (due_diff.mins > 1) ? "s" : "");
            }

        } else if (due_diff.mins != 0) {
            printf("%d minute%s", abs(due_diff.mins), (due_diff.mins > 1) ? "s" : "");

            if (due_diff.secs != 0) {
                printf(", %d second%s", abs(due_diff.secs), (due_diff.secs > 1) ? "s" : "");
            }

        } else if (due_diff.secs != 0) {
            printf("%d second%s", abs(due_diff.secs), (due_diff.secs > 1) ? "s" : "");
        }
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
    
    if (rem->title != NULL) {
        printf(ANSI_BOLD "%s", rem->title);
        if(rem->description != NULL) {
            printf(": " ANSI_RESET);
        }
    }

    if (rem->description) {
        printf(ANSI_RESET ANSI_ITALIC "%s" ANSI_RESET, rem->description);
    }

    printf(ANSI_RESET "\n");

    return PRINT_OK;
}

reminder_arr_t parse_file(char* file_contents) {
    size_t reminder_count = 0;
    for (size_t i = 0; file_contents[i]; ++i) {
        if (file_contents[i] == '\n') ++reminder_count;
    }
    size_t reminder_i = 0;

    reminder_t* reminders = (reminder_t*)malloc(reminder_count * sizeof(reminder_t));
    char year_str[5], month_str[3], day_str[3], hour_str[3], min_str[3], sec_str[3];
    int16_t year;
    uint8_t month, day, hour, min, sec;

    while (*file_contents != '\0') {
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
        if (month > 12) {
            fprintf(stderr, "ERROR: Malformed date in reminder, got month of %d (should be 1-12).\n", month + 1);
            exit(EXIT_FAILURE);
        }
        file_contents += 3;

        // day
        memcpy(day_str, file_contents, 2);
        day_str[2] = '\0';
        day = atoi(day_str);
        if (day == 0 || day > days_in_month(year, month)) {
            fprintf(stderr, "ERROR: Malformed date in reminder, got day of %d with month %d (should be 1-%d).\n", day, month, days_in_month(year,  month));
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
        
        // skip to next space or end of line/file
        for (; *file_contents != ' '
            && *file_contents != '\0'
            && *file_contents != '\n'; ++file_contents);

        // if not an end line/file, next character
        if (*file_contents == ' ') ++file_contents;

        // get length of title
        size_t title_len = 0;
        for (; *file_contents != ':'
            && *file_contents != '\0'
            && *file_contents != '\n'; ++file_contents, ++title_len);
        file_contents -= title_len;

        char* title = NULL;
        if (title_len) {
            // get title
            title = (char*)malloc((title_len + 1) * sizeof(char));
            if (!title && errno != 0) {
                perror("ERROR:");
                exit(errno);
            }
            memcpy(title, file_contents, title_len);
            title[title_len] = '\0';
        }

        ////
        // parse description

        // skip to after the title
        for (; *file_contents != ':'
            && *file_contents != '\0'
            && *file_contents != '\n'; ++file_contents);

        if (*file_contents == ':') ++file_contents;
        if (*file_contents == ' ') ++file_contents;

        // get length of description
        size_t desc_len = 0;
        for (; *file_contents != '\n'
            && *file_contents != '\0'; ++file_contents, ++desc_len);
        file_contents -= desc_len;

        char* desc = NULL;
        if (desc_len) {
            // get description
            desc = (char*)malloc((desc_len + 1) * sizeof(char));
            if(!desc && errno != 0) {
                perror("ERROR:");
                exit(errno);
            }
            memcpy(desc, file_contents, desc_len);
            desc[desc_len] = '\0';
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
            .severity = temp_sev,
            .title = title,
            .description = desc,
        };

        // add to reminders
        reminders[reminder_i] = temp_reminder;
        ++reminder_i;

        // skip to next line / end of file
        while(*file_contents != '\n' && *file_contents != '\0') ++file_contents;
        if (*file_contents == '\n') ++file_contents;
    }

    reminder_arr_t out_arr = {
        .arr = reminders,
        .size = reminder_count,
    };

    return out_arr;
}
