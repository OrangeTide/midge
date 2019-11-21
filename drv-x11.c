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
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <X11/keysym.h>

struct current {
	struct current *previous;
	Window win;
	GLXContext ctx;
	Bool has_event;
	XEvent event;
	struct midge_notifier_manager paint_manager, reshape_manager, keyinput_manager;
	XComposeStatus compose_status;
};

static Display *dpy;
static struct current *current;
static int double_buffer = 1;
static const char *title = "Window Title"; // TODO: configure window title
static Atom wm_delete_window;
static Bool done = False;

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
current_new(Window win, GLXContext ctx)
{
	struct current *new = calloc(1, sizeof(*new));
	new->previous = current;
	new->win = win;
	new->ctx = ctx;
	current = new;

	/* make sure current has the correct context loaded */
	glXMakeCurrent(dpy, win, ctx);

	return 0;
}

static Window
window_new(Window parent, Window root, XVisualInfo *vi, int width, int height)
{
	Window win;
	XSetWindowAttributes swa;
	long mask;

	swa.background_pixel = 0;
	swa.border_pixel = 0;
	swa.colormap = XCreateColormap(dpy, root, vi->visual, AllocNone);
	swa.event_mask = StructureNotifyMask | ExposureMask | KeyPressMask | KeyReleaseMask;
	mask = CWBackPixel | CWBorderPixel | CWColormap | CWEventMask;

	win = XCreateWindow(dpy, parent, 0, 0, width, height, 0, vi->depth, InputOutput, vi->visual, mask, &swa);
	report_info("window %dx%d created (win=%#lx)", width, height, win);

	return win;
}

static GLXContext
context_new(XVisualInfo *vi)
{
	return glXCreateContext(dpy, vi, NULL, True);
}

int
midge_drv_init(void)
{
	dpy = XOpenDisplay(NULL);
	if (!dpy)
		return -1;

	wm_delete_window = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
	return 0;
}

/* used to block until the window is mapped */
static Bool wait_for_MapNotify(Display *_dpy, XEvent *xev, XPointer arg) {
	(void)_dpy; // unused
	return (xev->type == MapNotify) && (xev->xmap.window == (Window)arg);
}

// TODO: add some parameters/configuration
int
midge_drv_window_new(void)
{
	int screen;
	Window root;
	XVisualInfo *vi;
	static int attr_single_rgba[] = {
		GLX_RGBA,
		GLX_RED_SIZE, 1,
		GLX_GREEN_SIZE, 1,
		GLX_BLUE_SIZE, 1,
		GLX_DEPTH_SIZE, 8,
		None
	};
	static int attr_double_rgba[] = {
		GLX_RGBA,
		GLX_RED_SIZE, 1,
		GLX_GREEN_SIZE, 1,
		GLX_BLUE_SIZE, 1,
		GLX_DEPTH_SIZE, 8,
		GLX_DOUBLEBUFFER,
		None
	};

	Window win;
	GLXContext ctx;

	if (!dpy)
		return -1;
	screen = DefaultScreen(dpy);
	root = RootWindow(dpy, screen);

	/* detect appropriate visual for GL */
	// try double buffer first ...
	if (double_buffer) {
		vi = glXChooseVisual(dpy, screen, attr_double_rgba);
	}
	// fallback to single buffer ...
	if (!vi || !double_buffer) {
		vi = glXChooseVisual(dpy, screen, attr_single_rgba);
		double_buffer = 0;
	}
	// no matching visual
	if (!vi) {
		report_error("Could not create RGBA visual");
		return -1;
	}

	/* create window */
	win = window_new(root, root, vi, 800, 600);
	if (!win) {
		report_error("Could not create Window");
		return -1;
	}

	/* set up window parameters */
	char *prot_names[] = { "WM_DELETE_WINDOW", };
	unsigned num_prot_names = sizeof(prot_names) / sizeof(*prot_names);
	Atom prots[num_prot_names];
	XInternAtoms(dpy, prot_names, num_prot_names, True, prots);
	XSetWMProtocols(dpy, win, prots, num_prot_names);
	// Pixmap icon = None; // TODO: load icon
	// XSetStandardProperties(dpy, win, title, title, icon, argv, argc, NULL);
	XStoreName(dpy, win, title);

	/* set up GL context */
	ctx = context_new(vi);
	if (!ctx) {
		report_error("Could not create GL context");
		XDestroyWindow(dpy, win);
		return -1;
	}

	current_new(win, ctx);

	XMapWindow(dpy, win);

	/* block until window is mapped */
	XEvent xev;
	XIfEvent(dpy, &xev, wait_for_MapNotify, (XPointer)win);

	return 0;
}

int
midge_drv_window_delete(void)
{
	// TODO: implement this
	return -1;
}

int
midge_drv_next(void)
{
	if (!dpy)
		return -1;
	if (done)
		return -1;
	// TODO: ... this blocks, maybe it shouldn't
	XNextEvent(dpy, &current->event);
	current->has_event = True;
	report_info("Event %d: win=%ld", current->event.type, current->event.xany.window);

	return 0;
}

static void
do_paint(void)
{
	struct midge_event paint_event = { .data.paint.paint_type = MIDGE_PAINT_READY };

	glXMakeCurrent(dpy, current->win, current->ctx); // TODO: cache which GL context+win is currently selected
	midge_notify_call(&current->paint_manager, &paint_event);
	glXSwapBuffers(dpy, current->win); // TODO: swapbuffers only if double buffered
	glXMakeCurrent(dpy, None, NULL); // TODO: cache which GL context+win is selected
}

static void
do_reshape(int width, int height)
{
	struct midge_event reshape_event = {
		.data.reshape.width = width,
		.data.reshape.height = height,
	};
	midge_notify_call(&current->reshape_manager, &reshape_event);
}

static void
do_xkey(XEvent *event, int pressed)
{
	KeySym sym;
	struct midge_event keyinput_event = {
		.data.keyinput.pressed = pressed,
	};

	int n = XLookupString(&event->xkey, keyinput_event.data.keyinput.keybuf,
		      sizeof(keyinput_event.data.keyinput.keybuf),
		      &sym, &current->compose_status);
	keyinput_event.data.keyinput.keybuf_len = n;
	keyinput_event.data.keyinput.keysym = sym;
	midge_notify_call(&current->keyinput_manager, &keyinput_event);
}

void
midge_drv_dispatch(void)
{
	if (!current->has_event)
		return; /* no event */
	current->has_event = False; /* consume the event */

	// TODO: look at event's window and find matching entry
	switch (current->event.type) {
	case Expose:
		do_paint();
		break;
	case ConfigureNotify:
		// TODO: rate limit to 1 reshape per paint
		do_reshape(current->event.xconfigure.width, current->event.xconfigure.height);
		break;
	case KeyPress:
		do_xkey(&current->event, 1);
		break;
	case KeyRelease:
		do_xkey(&current->event, 0);
		break;
	case ClientMessage:
		if ((Atom)current->event.xclient.data.l[0] == wm_delete_window)
			done = True;
		break;
	}
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
