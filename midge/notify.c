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
