#include "report.h"

#include <stdio.h>

void
report_error(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	report_errorv(fmt, ap);
	va_end(ap);
}

void
report_errorv(const char *fmt, va_list ap)
{
	fprintf(stderr, "ERROR: ");
        vfprintf(stderr, fmt, ap);
}

void
report_info(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	report_infov(fmt, ap);
	va_end(ap);
}

void
report_infov(const char *fmt, va_list ap)
{
	fprintf(stderr, "INFO: ");
        vfprintf(stderr, fmt, ap);
}
