#ifndef SHADERLOAD_H_
#define SHADERLOAD_H_

#ifdef OPENGL_HEADER
#  include OPENGL_HEADER
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/glaux.h>
#endif

GLuint shaderload_new_program(const char *vertex_source, const char *fragment_source);
GLuint shaderload_new_program_from_file(const char *vert_filename, const char *vert_default, const char *frag_filename, const char *frag_default);
#endif
