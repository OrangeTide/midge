#ifndef UNICODE
# define UNICODE
#endif

#define WIN32_MEAN_AND_LEAN

#include "report.h"

#include <stdio.h>

#include <windows.h>
#include <tchar.h>

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
        char scratch[1024];
        TCHAR wbuf[1024];
        int nResult;

        vsnprintf(scratch, sizeof(scratch), fmt, ap);

        nResult = MultiByteToWideChar(CP_UTF8, 0, scratch, strlen(scratch) + 1, wbuf, sizeof(wbuf) / sizeof(*wbuf));

        if (nResult > 0) {
                MessageBox(NULL, wbuf, TEXT("Error"), MB_ICONWARNING | MB_OK);
	} else {
                MessageBox(NULL, TEXT("Unknown Error"), TEXT("Error"), MB_ICONEXCLAMATION | MB_OK);
	}
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
	// TODO: detect AllocConsole() versus pure GUI (logfile?)
	vprintf(fmt, ap);
}
