#ifndef STRFTIME_H
#define STRFTIME_H

#include <time.h>
#include <stdint.h>

size_t strftime(char* buffer, size_t maxSize, const char* format, const struct tm* timp);

#endif // STRFTIME_H
