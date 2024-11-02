#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "reminders.h"
#include "file.h"

int main(void) {
    // struct tm test_time = {
    //     .tm_year = 124,
    //     .tm_mon  = 10,
    //     .tm_mday = 25,
    //     .tm_hour = 22,
    // };
    //
    // reminder_t reminder = {
    //     .due = test_time,
    //     .severity = High,
    //     .title = "Hello",
    //     .description = "World"
    // };
    //
    // print_reminder(&reminder);

    char* reminder_file = read_file("reminders.trdr");

    reminder_arr_t reminders = parse_file(reminder_file);

    free(reminder_file);

    for (size_t i = 0; i < reminders.size; ++i) {
        print_reminder(&reminders.arr[i]);
    }
}
