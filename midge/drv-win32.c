/* drv-win32.c : Windows driver */
/*-
 * Copyright (c) 2020 Jon Mayo
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are
 * those of the authors and should not be interpreted as representing official
 * policies, either expressed or implied, of the Midge project.
 */
#include "midge-drv.h"
#include <windows.h>

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#define ERR (-1)
#define OK (0)

struct current {
	struct current *previous;
	struct midge_notifier_manager paint_manager, reshape_manager, keyinput_manager;
};

static struct current *current;
static BOOL done = FALSE;

static void
report_info(const char *fmt, ...)
{
	va_list ap;
	fputs("INFO: ", stderr);
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	fputc('\n', stderr);
}

static void
report_error(const char *fmt, ...)
{
	va_list ap;
	fputs("ERROR: ", stderr);
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	fputc('\n', stderr);
}

static int
current_new(void)
{
	struct current *new = calloc(1, sizeof(*new));
	new->previous = current;
	current = new;

	// TODO: select the current context

	return OK;
}

int
midge_drv_init(void)
{
	return OK;
}

// TODO: add some parameters/configuration
int
midge_drv_window_new(void)
{
	// TODO: implement this

	return ERR;
}

int
midge_drv_window_delete(void)
{
	return ERR; // TODO: implement this
}

int
midge_drv_next(void)
{
	if (done)
		return -1;

	// TODO: process an event

	return ERR; // TODO: implement this
}

void
midge_drv_dispatch(void)
{
	// TODO: implement this
}

int
midge_drv_register_paint(struct midge_notifier_watcher *notify)
{
	if (!current)
		return -1;

	return midge_notifier_register(&current->paint_manager, notify);
}

int
midge_drv_register_keyinput(struct midge_notifier_watcher *notify)
{
	if (!current)
		return -1;

	return midge_notifier_register(&current->keyinput_manager, notify);
}
