#include "shaderload.h"
#include "loadfile.h"

#include <midge/midge.h>
#include <midge/report.h>

#include <string.h>
#include <stdlib.h>

static void
report_gl_error(const char *reason)
{
	GLenum code = glGetError();
	const GLubyte *str;

	if (code == GL_NO_ERROR)
		return;

#if 1 // enable this once we start loading libglu
	str = gluErrorString(code);
#else
	str = "(no glu)";
#endif
	if (!reason)
		report_error("GL error 0x%04x:%s", (unsigned)code, str);
	else
		report_error("%s:GL error 0x%04x:%s", reason, (unsigned)code, str);
}

static void
report_shader_error(GLuint shader, const char *reason)
{
	GLint info_len = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_len);
	if (info_len > 255)
		info_len = 255;
	char info[info_len + 1];
	glGetShaderInfoLog(shader, info_len, NULL, info);
	report_error("%s:%s", reason, info);
}

static GLuint
load_shader_from_string(GLenum type, const unsigned char *s)
{
	GLuint shader;
	GLint compile_status;
	const GLchar *source[] = { (const GLchar*)s };

	shader = glCreateShader(type);
	if (!shader)
		return 0;

	glShaderSource(shader, 1, source, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
	if (!compile_status) {
		report_shader_error(shader, "shader compile failed");
		glDeleteShader(shader);
		return 0;
	}

	return shader;
}

GLuint
shaderload_new_program(const char *vertex_source, const char *fragment_source)
{
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint program;
	GLint link_status;

	vertex_shader = load_shader_from_string(GL_VERTEX_SHADER, vertex_source);
	fragment_shader = load_shader_from_string(GL_FRAGMENT_SHADER, fragment_source);
	if (!vertex_shader || !fragment_shader)
		goto err;
	program = glCreateProgram();
	if (!program) {
		report_gl_error("glCreateProgram()");
		goto err_free_shaders;
	}
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
// #if USE_GLES2
//	glBindAttribLocation(program, 0, "vPosition");
//	glBindAttribLocation(program, 1, "vColor");
// #endif
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &link_status);
	if (!link_status) {
		report_shader_error(program, "shader linking failed");
		goto err_free_program;
	}
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	return program;
err_free_program:
	glDeleteProgram(program);
err_free_shaders:
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
err:
	return 0;
}

GLuint
shaderload_new_program_from_file(const char *vert_filename, const char *vert_default, const char *frag_filename, const char *frag_default)
{
	GLubyte *frag_source;
	GLubyte *vert_source;

	frag_source = loadfile(frag_filename, NULL);
	if (!frag_source)
		frag_source = strdup(frag_default);
	if (!frag_source)
		return 0;

	vert_source = loadfile(vert_filename, NULL);
	if (!vert_source)
		vert_source = strdup(vert_default);
	if (!vert_source) {
		free(frag_source);
		return 0;
	}

	GLuint program = shaderload_new_program(vert_source, frag_source);

	free(frag_source);
	free(vert_source);

	return program;
}
