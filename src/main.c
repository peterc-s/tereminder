#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "reminders.h"
#include "file.h"
#include "debug.h"

int DEBUG = 0;

static inline void usage(char* exe_name) {
    eprintf("\033[1mUsage:\033[0m %s [-v] [-o output_file] <input_file>\n", exe_name);
}

int main(int argc, char** argv) {
    // argument parsing
    int opt;
    opterr = 0;
    while ((opt = getopt(argc, argv, "v")) != -1) {
        switch (opt) {
            case 'v':
                DEBUG++;
                break;
            case '?':
                error("Unknown option -%c.\n", optopt);
                usage(argv[0]);
                return EXIT_FAILURE;
        }
    }

    // get reminder file
    info("Getting reminders file contents.\n");
    char* reminders_file = getenv("HOME");
    trace("Got home directory from env: %s\n", reminders_file);
    strcat(reminders_file, "/.reminders");
    char* reminders_raw = read_file(reminders_file);

    info("Parsing reminders file.\n");
    reminder_arr_t reminders = parse_file(reminders_raw);

    // free reminder file now that we don't need it
    trace("Freeing reminder file contents string.\n");
    free(reminders_raw);

    for (size_t i = 0; i < reminders.size; ++i) {
        print_reminder(&reminders.arr[i]);
    }

    // finally, free all reminder memory
    info("Freeing heap allocated memory.\n");
    for(size_t i = 0; i < reminders.size; ++i) {
        trace("Freeing reminder %ld.\n", i);
        free(reminders.arr[i].title);
        free(reminders.arr[i].description);
    }
    trace("Freeing reminder array.\n");
    free(reminders.arr);
}
