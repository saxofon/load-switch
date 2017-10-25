#ifndef __SIMPLE_IOT_H__
#define __SIMPLE_IOT_H__

#include <stdarg.h>

#include "simple-iot-security.h"
#include "simple-iot-transport.h"

#define DEBUG 1

#if DEBUG
static inline void debug(const char* format, ...)
{
        va_list arglist;

        va_start(arglist, format);
        vprintf(format, arglist);
        va_end(arglist);
}
#else
static inline void debug(const char* format, ...){}
#endif

#endif
