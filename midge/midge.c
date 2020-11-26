/* midge.c - the Midge framework: a small portable graphics library */
/*-
 * Copyright (c) 2019 Jon Mayo
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
#include "midge.h"
#include "midge-drv.h"
#include "glad.h"
#include <stdlib.h>
#include <stdbool.h>

static bool are_we_done;

int
midge_init(void)
{
	if (midge_drv_init())
		return -1;

	midge_app_configure();

	gladLoadGL();

	midge_app_start();

	return 0;
}

int
midge_next(void)
{
	if (are_we_done || midge_drv_next())
		return -1;
	return 0;
}

void
midge_dispatch(void)
{
	midge_drv_dispatch();
}

void
midge_done(void)
{
	are_we_done = true;
}

void
midge_exit(int status)
{
	midge_done();
	exit(status);
}

int
midge_window_new(void)
{
	// TODO: would be nice to configure the window's width, height, and other parameters
	return midge_drv_window_new();
}

int
midge_register_paint(struct midge_notifier_watcher *notify)
{
	return midge_drv_register_paint(notify);
}

int
midge_register_keyinput(struct midge_notifier_watcher *notify)
{
	return midge_drv_register_keyinput(notify);
}

void midge_app_configure(void) __attribute__((weak));
void
midge_app_configure(void)
{
	if (midge_window_new())
		midge_exit(1);
}
