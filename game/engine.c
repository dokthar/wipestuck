#include <stdio.h>
#include <string.h>

#include "engine.h"

GLFWwindow *window;

void
engine_render(struct camera *c, struct shader *s, struct mesh *m)
{
	GLint position;
	GLint normal;
	GLint texcoord;
	glUseProgram(s->prog);

	position = glGetAttribLocation(s->prog, "in_pos");
	normal = glGetAttribLocation(s->prog, "in_normal");
	texcoord = glGetAttribLocation(s->prog, "in_texcoord");
	mesh_bind(m, position, normal, texcoord);

	glBindVertexArray(m->vao);
	glEnable(GL_CULL_FACE);  glCullFace(GL_BACK);
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST); glDepthFunc(GL_LESS);

	if (m->index_count > 0)
		glDrawElements(m->primitive, m->index_count, GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(m->primitive, 0, m->vertex_count);

	glBindVertexArray(0);
	glUseProgram(0);
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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
	m->index_count = 0;

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

	glBindVertexArray(0);
}

void
mesh_index(struct mesh *m, size_t index_count, unsigned int *indices)
{

	if (!indices)
		return;

	m->idx_indices = m->vbo_count++;

	glBindVertexArray(m->vao);

	glGenBuffers(1, &m->vbo[m->idx_indices]);

	m->index_count = index_count;

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->vbo[m->idx_indices]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->index_count * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void
mesh_bind(struct mesh *m, GLint position, GLint normal, GLint texture)
{
	glBindVertexArray(m->vao);

	if (position >= 0) {
		glBindBuffer(GL_ARRAY_BUFFER, m->vbo[m->idx_positions]);
		glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 0, 0);
//		glEnableVertexArrayAttrib(m->vao, position);
		glEnableVertexAttribArray(position);
	}

	if (normal >= 0) {
		glBindBuffer(GL_ARRAY_BUFFER, m->vbo[m->idx_normals]);
		glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(normal);
	}

	if (texture >= 0) {
		glBindBuffer(GL_ARRAY_BUFFER, m->vbo[m->idx_texcoords]);
		glVertexAttribPointer(texture, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(texture);
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

void
mesh_load_quad(struct mesh *m, float x, float y)
{
	float positions[] = {
		-x, -y,  0,
		-x,  y,  0,
		 x, -y,  0,
		 x,  y,  0,
	};
	float texcoords[] = {
		0.0, 0.0,
		0.0, 1.0,
		1.0, 0.0,
		1.0, 1.0,
	};

	mesh_load(m, 4, positions, NULL, texcoords);
	m->primitive = GL_TRIANGLE_STRIP;
}

void
mesh_load_box(struct mesh *m, float a, float b, float c)
{
	/*
  a,b,-c +---+  -a,b,-c
        /   /|
a,b,c  +---+ -a,b,c
       |\, | |
       |  \| / -a,-b,-c
a,-b,c +---+'-a,-b,c
	 */
	float positions[] = {
		/* front */
		 a,  b,  c,   -a,  b,  c,   -a, -b,  c,   -a, -b,  c,    a, -b,  c,    a,  b,  c,
		/* back */
		-a,  b, -c,    a,  b, -c,    a, -b, -c,    a, -b, -c,   -a, -b, -c,   -a,  b, -c,
		/* left */
		 a,  b, -c,    a,  b,  c,    a, -b,  c,    a, -b,  c,    a, -b, -c,    a,  b, -c,
		/* right */
		-a,  b,  c,   -a,  b, -c,   -a, -b, -c,   -a, -b, -c,   -a, -b,  c,   -a,  b,  c,
		/* top */
		 a,  b, -c,   -a,  b, -c,   -a,  b,  c,   -a,  b,  c,    a,  b,  c,    a,  b, -c,
		/* bot */
		 a, -b,  c,   -a, -b,  c,   -a, -b, -c,   -a, -b, -c,    a, -b, -c,    a, -b,  c,
	};
	float normals[] = {
		 0,  0,  1,    0,  0,  1,    0,  0,  1,    0,  0,  1,    0,  0,  1,    0,  0,  1,
		 0,  0, -1,    0,  0, -1,    0,  0, -1,    0,  0, -1,    0,  0, -1,    0,  0, -1,
		 1,  0,  0,    1,  0,  0,    1,  0,  0,    1,  0,  0,    1,  0,  0,    1,  0,  0,
		-1,  0,  0,   -1,  0,  0,   -1,  0,  0,   -1,  0,  0,   -1,  0,  0,   -1,  0,  0,
		 0,  1,  0,    0,  1,  0,    0,  1,  0,    0,  1,  0,    0,  1,  0,    0,  1,  0,
		 0, -1,  0,    0, -1,  0,    0, -1,  0,    0, -1,  0,    0, -1,  0,    0, -1,  0,
	};
	float texcoords[] = {
		0, 1,    1, 1,    1, 0,    1, 0,    0, 0,    0, 1,
		0, 1,    1, 1,    1, 0,    1, 0,    0, 0,    0, 1,
		0, 1,    1, 1,    1, 0,    1, 0,    0, 0,    0, 1,
		0, 1,    1, 1,    1, 0,    1, 0,    0, 0,    0, 1,
		0, 1,    1, 1,    1, 0,    1, 0,    0, 0,    0, 1,
		0, 1,    1, 1,    1, 0,    1, 0,    0, 0,    0, 1,
	};
	mesh_load(m, 36, positions, normals, texcoords);
	m->primitive = GL_TRIANGLES;
}

#define M_PI 3.14159265358979
#include <math.h>

static void
gen_cylinder_positions(float *positions, vec3 center, vec3 up, float extent,
		       float radius, unsigned int sides)
{
	float a, x, y, z;
	unsigned int s;

	for (s = 0; s < sides; s++) {
		a = 2 * M_PI * (s / (float)sides);
		x = radius * sin(a);
		y = extent;
		z = radius * cos(a);
		positions[s * 6 + 0] = center[0] + x;
		positions[s * 6 + 1] = center[1] + y;
		positions[s * 6 + 2] = center[2] + z;
		x = radius * sin(a);
		y = 0;
		z = radius * cos(a);
		positions[s * 6 + 3] = center[0] + x;
		positions[s * 6 + 4] = center[1] + y;
		positions[s * 6 + 5] = center[2] + z;
	}
}

void
mesh_load_cylinder(struct mesh *m, float h, float r, unsigned int sides)
{
	float positions[(sides + 1) * 3 * 2];
	float normals[(sides + 1) * 3 * 2];
	float texcoords[(sides + 1) * 2 * 2];
	float a, x, y, z;
	unsigned int s;
	vec3 p, n;

#if 1
	gen_cylinder_positions(positions, (vec3){0,-h * 0.5,0}, (vec3){0,1,0}, h, r, sides);
#else
	for (s = 0; s < sides; s++) {
		a = 2 * M_PI * (s / (float)sides);
		x = r * sin(a);
		y = h * 0.5;
		z = r * cos(a);
		positions[s * 6 + 0] = x;
		positions[s * 6 + 1] = y;
		positions[s * 6 + 2] = z;
		x = r * sin(a);
		y = -h * 0.5;
		z = r * cos(a);
		positions[s * 6 + 3] = x;
		positions[s * 6 + 4] = y;
		positions[s * 6 + 5] = z;
	}
#endif
	/* require two vertices for different uv with same position */
	/* last vertex is at the same position as the first one */
	positions[sides * 6 + 0] = positions[0];
	positions[sides * 6 + 1] = positions[1];
	positions[sides * 6 + 2] = positions[2];
	positions[sides * 6 + 3] = positions[3];
	positions[sides * 6 + 4] = positions[4];
	positions[sides * 6 + 5] = positions[5];

	for (s = 0; s < sides; s++) {
		a = 2 * M_PI * (s / (float)sides);
		x = r * sin(a);
		z = r * cos(a);

		normals[s * 6 + 0] = x;
		normals[s * 6 + 1] = 0;
		normals[s * 6 + 2] = z;
		normals[s * 6 + 3] = x;
		normals[s * 6 + 4] = 0;
		normals[s * 6 + 5] = z;
	}
	normals[sides * 6 + 0] = normals[0];
	normals[sides * 6 + 1] = normals[1];
	normals[sides * 6 + 2] = normals[2];
	normals[sides * 6 + 3] = normals[3];
	normals[sides * 6 + 4] = normals[4];
	normals[sides * 6 + 5] = normals[5];

	for (s = 0; s < sides; s++) {
		a = s / (float)sides;
		texcoords[s * 4 + 0] = a;
		texcoords[s * 4 + 1] = 0;
		texcoords[s * 4 + 2] = a;
		texcoords[s * 4 + 3] = 1;
	}
	texcoords[sides * 4 + 0] = 1; /* sides / sides */
	texcoords[sides * 4 + 1] = texcoords[1];
	texcoords[sides * 4 + 2] = 1; /* sides / sides */
	texcoords[sides * 4 + 3] = texcoords[3];

	mesh_load(m, (sides + 1) * 2, positions, normals, texcoords);
	m->primitive = GL_TRIANGLE_STRIP;
}

void
mesh_load_torus(struct mesh *m, float circle_radius, float radial_radius,
		unsigned int circle_sides, unsigned int radial_sides)
{
	unsigned int vert_count = (circle_sides + 1) * (radial_sides + 1);
	float positions[vert_count * 3];
	float normals[vert_count * 3];
	float texcoords[vert_count * 2];
	unsigned int indices[radial_sides * (circle_sides + 1 + 2) * 2];
	float ra, ca, x, y, z;
	unsigned int r, c;
	vec3 cc, cp, cn;
	unsigned int off;
	
	for (r = 0; r < radial_sides; r++) {
		ra = 2 * M_PI * (r / (float) radial_sides);
		/* circle center */
		cc[0] = radial_radius * cos(ra);
		cc[1] = 0;
		cc[2] = radial_radius * sin(ra);
		off = r * (circle_sides + 1);
		for (c = 0; c < circle_sides; c++) {
			ca = 2 * M_PI * (c / (float) circle_sides);
			x = cos(ra) * circle_radius * cos(ca);
			y = circle_radius * sin(ca);
			z = sin(ra) * circle_radius * cos(ca);

			positions[(off + c) * 3 + 0] = cc[0] + x;
			positions[(off + c) * 3 + 1] = cc[1] + y;
			positions[(off + c) * 3 + 2] = cc[2] + z;

			cp[0] = x;
			cp[1] = y;
			cp[2] = z;
			sub3v(cn, cp, (vec3){0,0,0});
			normalize3(cn);
			normals[(off + c) * 3 + 0] = cn[0];
			normals[(off + c) * 3 + 1] = cn[1];
			normals[(off + c) * 3 + 2] = cn[2];

			texcoords[(off + c) * 2 + 0] = (r / (float) radial_sides);
			texcoords[(off + c) * 2 + 1] = (c / (float) circle_sides);
		}
		{
			positions[(off + c) * 3 + 0] = positions[off * 3 + 0];
			positions[(off + c) * 3 + 1] = positions[off * 3 + 1];
			positions[(off + c) * 3 + 2] = positions[off * 3 + 2];
			normals[(off + c) * 3 + 0] = normals[off * 3 + 0];
			normals[(off + c) * 3 + 1] = normals[off * 3 + 1];
			normals[(off + c) * 3 + 2] = normals[off * 3 + 2];
			texcoords[(off + c) * 2 + 0] = (r / (float) radial_sides);
			texcoords[(off + c) * 2 + 1] = 1;
		}
	}
	{
		off = r * (circle_sides + 1);
		for (c = 0; c <= circle_sides; c++) {
			positions[(off + c) * 3 + 0] = positions[c * 3 + 0];
			positions[(off + c) * 3 + 1] = positions[c * 3 + 1];
			positions[(off + c) * 3 + 2] = positions[c * 3 + 2];
			normals[(off + c) * 3 + 0] = normals[c * 3 + 0];
			normals[(off + c) * 3 + 1] = normals[c * 3 + 1];
			normals[(off + c) * 3 + 2] = normals[c * 3 + 2];
			texcoords[(off + c) * 2 + 0] = 1;
			texcoords[(off + c) * 2 + 1] = 1;
		}
	}

	for (r = 0; r < radial_sides; r++) {
		off = r * (circle_sides + 1 + 2) * 2;
		for (c = 0; c <= circle_sides; c++) {
			indices[off + c * 2 + 0] = (r + 1) * (circle_sides + 1) + c;
			indices[off + c * 2 + 1] = (r + 0) * (circle_sides + 1) + c;
		}
		{
			indices[off + c * 2 + 0] = (r + 1) * (circle_sides + 1) - 1;
			indices[off + c * 2 + 1] = (r + 1) * (circle_sides + 1) - 1;
			indices[off + c * 2 + 2] = (r + 2) * (circle_sides + 1);
			indices[off + c * 2 + 3] = (r + 2) * (circle_sides + 1);
		}
	}
	printf("%d indices\n", ARRAY_LEN(indices));
	mesh_load(m, vert_count, positions, normals, texcoords);
	mesh_index(m, ARRAY_LEN(indices), indices);
	m->primitive = GL_TRIANGLE_STRIP;
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
