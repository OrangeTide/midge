#include "midge-drv.h"
#include <assert.h>
#include <stdlib.h>

int
midge_notify_call(struct midge_notifier_manager *event, void *event_data)
{
	struct midge_notifier_watcher *cur;

	/* take items off the list and call them */
	while ((cur = event->first)) {
		/* detach from primary list. */
		if (cur->prev)
			*cur->prev = cur->next;

		/* place onto temp list in so that we don't lose it. */
		cur->prev = &event->temp;
		cur->next = event->temp;
		event->temp = cur;

		/* do callback */
		if (cur->fn)
			cur->fn(event_data, cur);
	}

	/* take everything off the temp list and place it back onto the original list */
	while ((cur = event->temp)) {

		/* detach from temp list */
		if (cur->prev)
			*cur->prev = cur->next;

		/* place onto primary list */
		cur->prev = &event->first;
		cur->next = event->first;
		event->first= cur;
	}
	return 0;
}

int
midge_notifier_register(struct midge_notifier_manager *event, struct midge_notifier_watcher *notify)
{
	if (!notify || !event)
		return -1;

	/* remove from old list */
	if (notify->prev) {
		*notify->prev = notify->next;
		notify->next = NULL;
	}
	assert(notify->next == NULL);

	/* insert to the head of the new list */
	notify->prev = &event->first;
	notify->next = event->first;
	event->first = notify;

	return 0;
}

struct midge_notifier_watcher *
midge_notifier_new(midge_notifier_fn *fn)
{
	struct midge_notifier_watcher *notify;

	notify = calloc(1, sizeof(*notify));
	if (!notify)
		return NULL;

	notify->next = NULL;
	notify->prev = NULL;
	notify->fn = fn;

	return notify;
}
