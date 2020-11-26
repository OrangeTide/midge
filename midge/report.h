#ifndef REPORT_H_
#define REPORT_H_
#include <stdarg.h>

void report_error(const char *fmt, ...);
void report_errorv(const char *fmt, va_list ap); 
void report_info(const char *fmt, ...);
void report_infov(const char *fmt, va_list ap);
#endif
