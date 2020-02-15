/* demo2.c - second demo of the Midge framework
 *
 * Written in 2019 by Jon Mayo <jon@rm-f.net>
 *
 * To the extent possible under law, the author(s) have dedicated all copyright
 * and related and neighboring rights to this software to the public domain
 * worldwide.  This software is distributed without any warranty.
 *
 * You should have received a copy of the CC0 Public Domain Dedication along with
 * this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
 */
#include <midge/midge.h>
#include <midge/midge-main.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>

static struct midge_notifier_watcher *paint_notifier, *keyinput_notifier;
static GLuint fontsheet = 0;

void app_paint(struct midge_event *event_data, struct midge_notifier_watcher *notify);
void app_keyinput(struct midge_event *event_data, struct midge_notifier_watcher *notify);
void app_reshape(struct midge_event *event_data, struct midge_notifier_watcher *notify);
void app_focus(struct midge_event *event_data, struct midge_notifier_watcher *notify);

void
midge_app_start(void)
{
	paint_notifier = midge_notifier_new(app_paint);
	midge_register_paint(paint_notifier);

	keyinput_notifier = midge_notifier_new(app_keyinput);
	midge_register_keyinput(keyinput_notifier);

	const GLubyte *ver = glGetString(GL_VERSION);
	printf("Version: %s\n", ver);
	if (!ver) {
		fprintf(stderr, "Missing GL context!\n");
		exit(1);
	}

	glClearColor(0.8, 0.2, 0.8, 1.0);

	glGenTextures(1, &fontsheet);
	glBindTexture(GL_TEXTURE_2D, fontsheet);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	/* load a 2x2 pixel texture */
	unsigned char data[] = {
		255, 0, 0, 255, 0, 255, 0, 255,
		0, 0, 255, 255, 255, 255, 255, 255,
	};
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );
}

void
app_stop(void)
{
	// TODO: unregister paint_notifier()
}

void
app_paint(struct midge_event *event_data, struct midge_notifier_watcher *notify)
{
	double x = 0.0, y = 0.0, w = 0.5, h = 0.5;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, fontsheet);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(x, y, 0.0f);
	glTexCoord2f(0.0, 1.0); glVertex3f(x, y + h, 0.0f);
	glTexCoord2f(1.0, 1.0); glVertex3f(x + w, y + h, 0.0f);
	glTexCoord2f(1.0, 0.0); glVertex3f(x + w, y, 0.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);

#if 0
	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		fprintf(stderr, "err=%d\n", err);
	}
#endif

}

static void
hexprint(FILE *f, const void *data, size_t len)
{
	const char *p, *end = (const char*)data + len - 1;
	for (p = data; p <= end; p++) {
		fprintf(f, " %02hhX", *p);
	}
}

void
app_keyinput(struct midge_event *event_data, struct midge_notifier_watcher *notify)
{
	char *buf = event_data->data.keyinput.keybuf;
	int len = event_data->data.keyinput.keybuf_len;
	int pressed = event_data->data.keyinput.pressed;

	if (pressed) {
		fprintf(stderr, "Pressed (len=%d)", len);
		hexprint(stderr, buf, len);
		fprintf(stderr, "\n");

	} else if (!pressed) {
		// TODO: there is probably no string in the release
		fprintf(stderr, "Released (len=%d)", len);
		hexprint(stderr, buf, len);
		fprintf(stderr, "\n");

		if (len && buf[0] == 27) {
			midge_done();
		}
	}
}

void
app_reshape(struct midge_event *event_data, struct midge_notifier_watcher *notify)
{
	// fprintf(stderr, "reshape %dx%d\n", width, height);

	// float ratio = w * 1.0 / h;
	// glMatrixMode(GL_PROJECTION);
	// glLoadIdentity();
	// glViewport(0, 0, w, h);
	// gluPerspective(45.0, ratio, 0.1, 1000.0);
	// glMatrixMode(GL_MODELVIEW);
}

void
app_focus(struct midge_event *event_data, struct midge_notifier_watcher *notify)
{
}
