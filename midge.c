#include "midge.h"
#include "midge-drv.h"
#include <stdlib.h>
#include <stdbool.h>

static bool are_we_done;

int
midge_init(void)
{
	if (midge_drv_init())
		return -1;

	midge_app_configure();
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
