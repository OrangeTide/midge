/* midge-drv.h - application interface for midge */
/* no copyright on interfaces or header files */
#ifndef MIDGE_H_
#define MIDGE_H_

struct midge_notifier_watcher;

struct midge_event_keyinput {
	unsigned char pressed;
	short keysym;
	char keybuf[16];
	unsigned char keybuf_len;
};

struct midge_event_paint {
	enum midge_paint_type { MIDGE_PAINT_READY, MIDGE_PAINT_REQUIRED } paint_type;
};

struct midge_event_reshape {
	short width, height;
};

struct midge_event {
	union {
		struct midge_event_keyinput keyinput;
		struct midge_event_paint paint;
		struct midge_event_reshape reshape;
	} data;
};

typedef void midge_notifier_fn(struct midge_event *event_data, struct midge_notifier_watcher *notify);

/* used by midge-main.h or by custom application */
int midge_init(void);
int midge_next(void);
void midge_dispatch(void);
void midge_done(void);
void midge_exit(int status);
int midge_window_new(void);

/* event notifier API */
struct midge_notifier_watcher *midge_notifier_new(midge_notifier_fn *fn);
int midge_register_paint(struct midge_notifier_watcher *notify);
int midge_register_keyinput(struct midge_notifier_watcher *notify);

/* linking application must define these */
void midge_app_start(void);
void midge_app_configure(void); /* optional - weak symbol for fallback */

#endif
