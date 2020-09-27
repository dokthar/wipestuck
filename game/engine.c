#include <stdio.h>
#include <string.h>

#include "engine.h"

GLFWwindow *window;

void
render(struct camera *c, struct shader *s, struct mesh *m)
{
	GLint loc;

	glUseProgram(s->prog);
	glBindVertexArray(m->vao);
/*
	loc = glGetUniformLocation(geometry->shader, "projection");
	glUniformMatrix4fv(loc, 1, GL_FALSE, (float*)camera->projection);

	loc = glGetUniformLocation(geometry->shader, "view");
	glUniformMatrix4fv(loc, 1, GL_FALSE, (float*)camera->view);

	loc = glGetUniformLocation(geometry->shader, "model");
	glUniformMatrix4fv(loc, 1, GL_FALSE, (float*)model);

	loc = glGetUniformLocation(geometry->shader, "inverseNormal");
	glUniformMatrix3fv(loc, 1, GL_FALSE, (float*)inverseNormal);
*/
	/* glPolygonMode(GL_FRONT_AND_BACK, geometry->mode); */
}


static void
framebuffer_callback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, width = w, height = h);
}

extern double xpre, ypre;

static void
init_glfw(void)
{
	if (!glfwInit())
		die("GLFW init failed\n");

	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);
	glfwWindowHint(GLFW_STENCIL_BITS, 8);

	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_FALSE);

	window = glfwCreateWindow(width, height, app_name, NULL, NULL);
	if (!window)
		die("create window failed\n");

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	glfwGetFramebufferSize(window, &width, &height);
	glfwSetFramebufferSizeCallback(window, framebuffer_callback);

	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwGetCursorPos(window, &xpre, &ypre);

	glfwSetKeyCallback(window, key_callback);
}

void
init_engine(void)
{
	int ret;

	init_glfw();

	ret = glewInit();
	if (ret != GLEW_OK)
		die("GLEW init failed: %s\n", glewGetErrorString(ret));

}

void
fini_engine(void)
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

int
engine_shutdown(void)
{
	glfwWindowShouldClose(window);
}

void
engine_poll(void)
{
	glfwPollEvents();
}

void
engine_swap(GLbitfield clear)
{
	glfwSwapBuffers(window);
	if (clear)
		glClear(clear);
}

void
mesh_load(struct mesh *m, size_t count, float *positions, float *normals, float *texcoords)
{
	GLuint vbo_count = 0;

	glGenVertexArrays(1, &m->vao);
	glBindVertexArray(m->vao);
	
	m->idx_positions = (positions) ? vbo_count++ : 0;
	m->idx_normals   = (normals)   ? vbo_count++ : 0;
	m->idx_texcoords = (texcoords) ? vbo_count++ : 0;

	glGenBuffers(vbo_count, m->vbo);

	m->vbo_count = vbo_count;
	m->vertex_count = count;

	if (positions) {
		glBindBuffer(GL_ARRAY_BUFFER, m->vbo[m->idx_positions]);
		glBufferData(GL_ARRAY_BUFFER, m->vertex_count * 3 * sizeof(float), positions, GL_STATIC_DRAW);
	}

	if (normals) {
		glBindBuffer(GL_ARRAY_BUFFER, m->vbo[m->idx_normals]);
		glBufferData(GL_ARRAY_BUFFER, m->vertex_count * 3 * sizeof(float), normals, GL_STATIC_DRAW);
	}

	if (texcoords) {
		glBindBuffer(GL_ARRAY_BUFFER, m->vbo[m->idx_texcoords]);
		glBufferData(GL_ARRAY_BUFFER, m->vertex_count * 2 * sizeof(float), texcoords, GL_STATIC_DRAW);
	}
/*
	if (positions) {
		glBindBuffer(GL_ARRAY_BUFFER, m->vbo[m->idx_positions]);
		glVertexAttribPointer(MESH_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, 0, 0);
//		glEnableVertexAttribArray(MESH_ATTRIB_POSITION);
		glEnableVertexArrayAttrib(m->vao, MESH_ATTRIB_POSITION);
	}

	if (normals) {
		glBindBuffer(GL_ARRAY_BUFFER, m->vbo[m->idx_normals]);
		glEnableVertexAttribArray(MESH_ATTRIB_NORMAL);
		glVertexAttribPointer(MESH_ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	if (texcoords) {
		glBindBuffer(GL_ARRAY_BUFFER, m->vbo[m->idx_texcoords]);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
//		glEnableVertexAttribArray(MESH_ATTRIB_TEXCOORD);
		glEnableVertexArrayAttrib(m->vao, 1);
	}
*/
	glBindVertexArray(0);
}

void
mesh_bind(struct mesh *m, GLint position, GLint normal, GLint texture)
{
	glBindVertexArray(m->vao);

	if (position >= 0) {
		glBindBuffer(GL_ARRAY_BUFFER, m->vbo[m->idx_positions]);
		glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexArrayAttrib(m->vao, position);
	}

	if (normal >= 0) {
		glBindBuffer(GL_ARRAY_BUFFER, m->vbo[m->idx_normals]);
		glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexArrayAttrib(m->vao, normal);
	}

	if (texture >= 0) {
		glBindBuffer(GL_ARRAY_BUFFER, m->vbo[m->idx_texcoords]);
		glVertexAttribPointer(texture, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexArrayAttrib(m->vao, texture);
	}

	glBindVertexArray(0);
}

void
mesh_free(struct mesh* m)
{
	if (m->vbo_count)
		glDeleteBuffers(m->vbo_count, m->vbo);
	m->vbo_count = 0;

	if (m->vao && glIsVertexArray(m->vao) == GL_TRUE)
		glDeleteVertexArrays(1, &m->vao);
}

char logbuf[4096];
GLsizei logsize;

static GLint
shader_compile(GLsizei count, const GLchar **string, const GLint *length, GLenum type, GLuint *out)
{
	GLuint shader = glCreateShader(type);
	GLint ret;

	glShaderSource(shader, count, string, length);
        glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &ret);
	if (ret != GL_TRUE) {
		glGetShaderInfoLog(shader, sizeof(logbuf), &logsize, logbuf);	
		glDeleteShader(shader);
		fprintf(stderr, "--- ERROR ---\n%s", logbuf);
		return ret;
	}

	*out = shader;
	return GL_TRUE;
}

GLint
shader_load(struct shader *s, const char *vertex, const char *fragment, const char *geom_src)
{
	GLuint prog;
	GLuint vert;
	GLuint frag;
	GLuint geom;
	GLint vert_len = strlen(vertex);
	GLint frag_len = strlen(fragment);
	GLint geom_len;
	GLint ret;

	if (!vertex || !fragment)
		return GL_FALSE;

	ret = shader_compile(1, &vertex, &vert_len, GL_VERTEX_SHADER, &vert);
	if (ret != GL_TRUE)
		return ret;
	ret = shader_compile(1, &fragment, &frag_len, GL_FRAGMENT_SHADER, &frag);
	if (ret != GL_TRUE) {
		glDeleteShader(vert);
		return ret;
	}
	if (geom_src) {
		geom_len = strlen(geom_src); 
		ret = shader_compile(1, &geom_src, &geom_len, GL_GEOMETRY_SHADER, &geom);
		if (ret != GL_TRUE) {
			glDeleteShader(vert);
			glDeleteShader(frag);
			return ret;
		}
	}

	prog = glCreateProgram();
	glAttachShader(prog, vert);
	glAttachShader(prog, frag);
	if (geom_src)
		glAttachShader(prog, geom);
	glLinkProgram(prog);

	glGetProgramiv(prog, GL_LINK_STATUS, &ret);
	if (ret != GL_TRUE) {
		glGetProgramInfoLog(prog, sizeof(logbuf), &logsize, logbuf);
		fprintf(stderr, "--- ERROR ---\n%s", logbuf);
		glDetachShader(prog, vert);
		glDetachShader(prog, frag);
		glDeleteShader(vert);
		glDeleteShader(frag);
		glDeleteProgram(prog);
		return ret;
	}

	s->prog = prog;
	s->vert = vert;
	s->frag = frag;

	return GL_TRUE;
}

void
shader_free(struct shader *s)
{
	glDetachShader(s->prog, s->vert);
	glDetachShader(s->prog, s->frag);
	glDeleteShader(s->vert);
	glDeleteShader(s->frag);
	glDeleteProgram(s->prog);
	s->vert = 0;
	s->frag = 0;
	s->prog = 0;
}
