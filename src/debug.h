#ifndef DEBUG_H
#define DEBUG_H

extern int DEBUG;

#define WARN 1
#define INFO 2
#define TRACE 3

void error(const char *format, ...);
void warn(const char *format, ...);
void info(const char *format, ...);
void trace(const char *format, ...);

void eprintf(const char *format, ...);

#endif
