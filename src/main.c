#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "reminders.h"
#include "file.h"

int main(void) {
    char* reminders_file = getenv("HOME");
    strcat(reminders_file, "/.reminders");

    char* reminders_raw = read_file(reminders_file);

    reminder_arr_t reminders = parse_file(reminders_raw);

    // free reminder file now that we don't need it
    free(reminders_raw);

    for (size_t i = 0; i < reminders.size; ++i) {
        print_reminder(&reminders.arr[i]);
    }

    // finally, free all reminder memory
    for(size_t i = 0; i < reminders.size; ++i) {
        free(reminders.arr[i].title);
        free(reminders.arr[i].description);
    }
    free(reminders.arr);
}
