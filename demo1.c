#include "midge.h"
#include "midge-main.h"
#include <GL/gl.h>

static void my_paint(struct midge_event *event_data, struct midge_notifier_watcher *notify);

static struct midge_notifier_watcher *paint_notifier;

void
midge_app_start(void)
{
	paint_notifier = midge_notifier_new(my_paint);
	midge_register_paint(paint_notifier);
	glClearColor(0.5, 0.5, 1.0, 1.0);
}

static void
my_paint(struct midge_event *event_data, struct midge_notifier_watcher *notify)
{
	glClear(GL_COLOR_BUFFER_BIT);
}
