#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "reminders.h"
#include "ansi.h"
#include "debug.h"
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
    char due_str[20];
    struct tm due = { 0 }; 

    while (*file_contents != '\0') {
        ////
        // parse due date
        // YYYY-MM-DDTHH:MM:SS
        
        // %Y-%m-%dT%H:%M:%S

        for (uint8_t i = 0; i < 20; ++i) {
            if (*file_contents == '\n' || *file_contents == '\0') {
                error("Couldn't parse reminder %ld, date too short.\n", reminder_i + 1);
                exit(FORMAT_TIME_FAIL);
            }
        }

        memcpy(due_str, file_contents, 19);
        due_str[19] = '\0';
        if (!strptime(due_str, "%Y-%m-%dT%H:%M:%S", &due)) {
            error("Couldn't parse reminder %ld, bad date.\n", reminder_i + 1);
            exit(FORMAT_TIME_FAIL);
        }


        ////
        // parse severity
        
        Severity sev;

        // skip to next space
        for(; *file_contents != ' '; ++file_contents);
        switch (*(++file_contents)) {
            case 'R':
                sev = Routine;
                break;
            case 'L':
                sev = Low;
                break;
            case 'M':
                sev = Medium;
                break;
            case 'H':
                sev = High;
                break;
            default:
                error("Couldn't parse reminder %ld, bad severity `%c`.\n", reminder_i + 1, *file_contents);
                exit(FORMAT_TIME_FAIL);
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
                error("%s\n", strerror(errno));
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
                error("%s\n", strerror(errno));
                perror("ERROR:");
                exit(errno);
            }
            memcpy(desc, file_contents, desc_len);
            desc[desc_len] = '\0';
        }

        reminder_t temp_reminder = {
            .due = due,
            .severity = sev,
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
