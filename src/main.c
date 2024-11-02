#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "reminders.h"
#include "file.h"

int main(void) {
    char* reminder_file = read_file("reminders.trdr");

    reminder_arr_t reminders = parse_file(reminder_file);

    // free reminder file now that we don't need it
    free(reminder_file);

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
