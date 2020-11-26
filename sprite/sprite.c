#include "sprite.h"
#include "shaderload.h"

#ifdef OPENGL_HEADER
#  include OPENGL_HEADER
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/glaux.h>
#endif

#include <midge/report.h>

#include <stddef.h>

#define OK (0)
#define ERR (-1)

enum {
	LAYOUT_POSITION = 0,
	LAYOUT_TEXCOORD = 1,
};

struct sprite_attr {
	GLfloat x, y;
	GLfloat u, v;
};

#define ATTR_OFS_POSITION ((void*)offsetof(struct sprite_attr, x))
#define ATTR_OFS_TEXCOORD ((void*)offsetof(struct sprite_attr, u))
#define ATTR_STRIDE (sizeof(struct sprite_attr))

struct sprite_attr quad[] = {
	/* unit sized quad with Y-flipped texture coordinates */
	{ -0.5f, 0.5f,	0.0f, 0.0f, },
	{ 0.5f, -0.5f,	1.0f, 1.0f, },
	{ -0.5f, -0.5f,	0.0f, 1.0f, },
	{ -0.5f, 0.5f,	0.0f, 0.0f, },
	{ 0.5f, -0.5f,	1.0f, 1.0f, },
	{ 0.5f,  0.5f,	1.0f, 0.0f, },
};

static const char frag_filename[] = "sprite.frag";
static const GLubyte *frag_default_source =
	"#version 330 core\n"
	"out vec4 FragColor;\n"
	"in vec2 fTex;\n"
	"uniform sampler2D uImage;\n"
	"void main()\n"
	"{\n"
	"  FragColor = texture(uImage, fTex);\n"
	// "  FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n" // red
	"}";

static const char vert_filename[] = "sprite.vert";
static const GLubyte *vert_default_source =
	"#version 330 core\n"
	"layout (location = 0) in vec2 aPos;\n"
	"layout (location = 1) in vec2 aTex;\n"
	"out vec2 fTex;\n"
	"void main()\n"
	"{\n"
	"gl_Position = vec4(aPos, 0.0, 1.0);\n"
	"fTex = aTex;\n"
	"}\n";

static GLuint sprite_program = 0;
static GLuint sprite_vao = 0;
static GLuint sprite_vbo = 0; /* a quad to draw all sprites */

int
sprite_init(void)
{
	sprite_program = shaderload_new_program_from_file(vert_filename, vert_default_source, frag_filename, frag_default_source);
	if (!sprite_program)
		return ERR;

	glGenVertexArrays(1, &sprite_vao);
	glBindVertexArray(sprite_vao);

	glGenBuffers(1, &sprite_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, sprite_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

	glEnableVertexAttribArray(LAYOUT_POSITION);
	glEnableVertexAttribArray(LAYOUT_TEXCOORD);

	glVertexAttribPointer(LAYOUT_POSITION, 2, GL_FLOAT, GL_FALSE, ATTR_STRIDE, ATTR_OFS_POSITION);
	glVertexAttribPointer(LAYOUT_TEXCOORD, 2, GL_FLOAT, GL_FALSE, ATTR_STRIDE, ATTR_OFS_TEXCOORD);

	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	report_info("ATTR: position=%+ld texcoord=%+ld stride=%ld\n",
			(int)ATTR_OFS_POSITION, (int)ATTR_OFS_TEXCOORD, (int)ATTR_STRIDE);

	return OK;
}

void
sprite_done(void)
{
	if (sprite_program) {
		glDeleteProgram(sprite_program);
		sprite_program = 0;
	}
}

void
sprite_draw(void)
{
	if (!sprite_program)
		return;
	if (!sprite_vao || !sprite_vbo)
		return;

	glUseProgram(sprite_program);

	//
	// TODO: implement the real thing ..
	//

	// test code:
	glBindVertexArray(sprite_vao);

	glDrawArrays(GL_TRIANGLES, 0, sizeof(quad) / sizeof(*quad));

	glBindVertexArray(0);

	glUseProgram(0);
}
