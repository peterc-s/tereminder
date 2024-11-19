#include <stdio.h>
#include <stdarg.h>

#include "debug.h"
#include "ansi.h"

void error(const char *format, ...) {
    fprintf(stderr, ANSI_COLOR_RED "[ERROR] " ANSI_RESET);

    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
}

void warn(const char *format, ...) {
    if (DEBUG >= WARN) {
        fprintf(stderr, ANSI_COLOR_YELLOW "[WARN] " ANSI_RESET);

        va_list args;
        va_start(args, format);
        vfprintf(stderr, format, args);
        va_end(args);
    }
}

void info(const char *format, ...) {
    if (DEBUG >= INFO) {
        fprintf(stderr, ANSI_COLOR_BLUE "[INFO] " ANSI_RESET);

        va_list args;
        va_start(args, format);
        vfprintf(stderr, format, args);
        va_end(args);
    }
}

void trace(const char *format, ...) {
    if (DEBUG >= TRACE) {
        fprintf(stderr, ANSI_COLOR_CYAN "[TRACE] " ANSI_RESET);

        va_list args;
        va_start(args, format);
        vfprintf(stderr, format, args);
        va_end(args);
    }
}

void eprintf(const char *restrict format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
}
