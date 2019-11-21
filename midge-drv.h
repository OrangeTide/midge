#ifndef MIDGE_DRV_H_
#define MIDGE_DRV_H_
#include "midge.h"

struct midge_notifier_manager {
	struct midge_notifier_watcher *first, *temp;
};

struct midge_notifier_watcher {
	struct midge_notifier_watcher *next, **prev;
	midge_notifier_fn *fn;
};

int midge_drv_init(void);
int midge_drv_next(void);
void midge_drv_dispatch(void);
int midge_drv_window_new(void);
int midge_drv_register_paint(struct midge_notifier_watcher *notify);
int midge_drv_register_keyinput(struct midge_notifier_watcher *notify);

/* interal API */
struct midge_notifier_manager;
int midge_notifier_register(struct midge_notifier_manager *event, struct midge_notifier_watcher *notify);
int midge_notify_call(struct midge_notifier_manager *event, void *event_data);

#endif
