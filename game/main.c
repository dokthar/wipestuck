#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <fcntl.h>
#include <errno.h>

#include <math.h>
#include "engine.h"

char *app_name;
unsigned int width = 1080;
unsigned int height = 800;

struct camera cam;
int flycam;
double xpre, ypre;
int xray = 0;

void camera_move(struct camera* c, vec3 off);
void camera_rotate(struct camera* c, vec3 axis, float angle);
static void camera_get_up(struct camera* c, vec3 up);
static void camera_get_dir(struct camera* c, vec3 dir);
static void camera_get_left(struct camera* c, vec3 left);

void
cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	double dx, dy;

	dx = xpre - xpos;
	dy = ypre - ypos;
	
	if (flycam) {
		vec3 up = {0, 1, 0};
		vec3 left;

		camera_rotate(&cam, up, 0.001 * dx);
		camera_get_left(&cam, left);
		normalize3(left);
		camera_rotate(&cam, left, -0.001 * dy);
	}
	xpre = xpos;
	ypre = ypos;
}

void
key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	vec3 dir, left;
	camera_get_dir(&cam, dir);
	scale3v(dir, 0.1);
	camera_get_left(&cam, left);
	scale3v(left, 0.1);

	switch (key) {
	case 'C':
		if (action == GLFW_PRESS && mods & GLFW_MOD_CONTROL)
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		break;
	case 'Z':
		if (action == GLFW_PRESS) {
			flycam = !flycam;
			if (flycam)
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			else
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			glfwGetCursorPos(window, &xpre, &ypre);
		}
		break;
	case 'X':
		if (action == GLFW_PRESS) {
			xray = !xray;
		}
		break;
	case 'A':
	case GLFW_KEY_LEFT:
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
			camera_move(&cam, left);
		break;
	case 'D':
	case GLFW_KEY_RIGHT:
		neg3v(left);
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
			camera_move(&cam, left);
		break;
	case 'W':
	case GLFW_KEY_UP:
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
			camera_move(&cam, dir);
		break;
	case 'S':
	case GLFW_KEY_DOWN:
		neg3v(dir);
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
			camera_move(&cam, dir);
		break;
		
	default:
		break;
	}
}

const char *vert =
	"#version 410 core\n"
	"in vec3 in_pos;\n"
	"in vec3 in_normal;\n"
	"in vec2 in_texcoord;\n"
	"out vec3 normal;\n"
	"out vec2 texcoord;\n"
	"uniform mat4 proj;\n"
	"uniform mat4 view;\n"
	"uniform mat4 model;\n"
	"void main(void)\n"
	"{\n"
//	"	gl_Position = vec4(in_pos.x, in_pos.y, in_pos.z, 1.0);\n"
//	"	gl_Position = projection * view * model * vec4(in_pos, 1.0);"
	"	gl_Position = proj * view * vec4(in_pos, 1.0);"
	"	texcoord = in_texcoord;\n"
	"	normal = in_normal;\n"
	"}\n";

const char *frag =
	"#version 410 core\n"
	"in vec3 normal;\n"
	"in vec2 texcoord;\n"
	"out vec4 out_color;\n"
	"uniform float time;\n"
	"void main(void)\n"
	"{\n"
	"	vec2 uv = (texcoord -0.5 )* 10;\n"
	"float a = time;\n"
	"uv *= mat2(cos(a), sin(a), -sin(a), cos(a));\n"
	"	vec3 col = vec3(fract(uv), sin(time*10)*.5+.5);\n"
	"	out_color = vec4(col,0);\n"
	"}\n";

const char *normal_vert =
	"#version 410 core\n"
	"in vec3 in_pos;\n"
	"in vec3 in_normal;\n"
	"out vec3 normal;\n"
	"uniform mat4 proj;\n"
	"uniform mat4 view;\n"
	"uniform mat4 model;\n"
	"void main(void)\n"
	"{\n"
	"	gl_Position = vec4(in_pos, 1.0);"
	"	normal = in_normal;\n"
	"}\n";

const char *normal_geom =
	"#version 410 core\n"
	"layout (triangles) in;\n"
	"layout (line_strip, max_vertices = 8) out;\n"
	"in vec3 normal[];\n"
	"out vec3 color;\n"
	"const float MAGNITUDE = 0.2;\n"
	"uniform mat4 proj;\n"
	"uniform mat4 view;\n"
	"void GenerateLine(int index) {\n"
	"	gl_Position = proj * view *gl_in[index].gl_Position;\n"
	"	color = normal[index];\n"
	"	EmitVertex();\n"
	"	gl_Position = proj * view*(gl_in[index].gl_Position + vec4(normal[index], 0.0) * MAGNITUDE);\n"
	"	color = normal[index];\n"
	"	EmitVertex();\n"
	"	EndPrimitive();\n"
	"}\n"
	"void main() {\n"
	"	GenerateLine(0); // first vertex normal\n"
	"	GenerateLine(1); // second vertex normal\n"
	"	GenerateLine(2); // third vertex normal\n"
	"}\n";

const char *normal_frag =
	"#version 410\n"
	"in vec3 color;\n"
	"out vec4 out_color;\n"
	"void main() {\n"
	"	out_color = vec4(abs(color), 1);\n"
	"}\n";

static void quad_load(struct mesh *m);
static void render_quad(struct shader *s, struct mesh *m);
static void box_load(struct mesh *m);
static void render_box(struct camera *c, struct shader *s, struct mesh *m);

static void camera_get_left(struct camera* c, vec3 left) {
	mat3 rot;
	quaternion_to_rot3(rot, c->rotation);
	row3m3(left, rot, 0);
}

static void camera_get_up(struct camera* c, vec3 up) {
	mat3 rot;
	quaternion_to_rot3(rot, c->rotation);
	row3m3(up, rot, 1);
}

static void camera_get_dir(struct camera* c, vec3 dir) {
	mat3 rot;
	quaternion_to_rot3(rot, c->rotation);
	row3m3(dir, rot, 2);
}

void
camera_update_view(struct camera *c)
{
	mat3 rot;
	float x, y, z;
	vec3 f;
	vec3 u;
	vec3 s;
	quaternion_to_rot3(rot, c->rotation);
	row3m3(s, rot, 0);
	row3m3(u, rot, 1);
	row3m3(f, rot, 2);
	normalize3(f);
	normalize3(u);
	cross3(s, f, u);
	normalize3(s);
/*
M = s0  s1  s2 0
    u0  u1  u2 0
   -f0 -f1 -f2 0
    0    0   0 1
*/
	/* changement de repère worldview to eyeview */
	x = -dot3(s, c->position);
	y = -dot3(u, c->position);
	z =  dot3(f, c->position);
	{
		mat4 view = { /* tableau de vecteurs colonne */
			{  s[0],  u[0], -f[0], 0 },
			{  s[1],  u[1], -f[1], 0 },
			{  s[2],  u[2], -f[2], 0 },
			{     x,     y,     z, 1 }
		};

		memcpy(c->view, view, sizeof(view));
	}
}

// https://github.com/g-truc/glm/blob/master/manual.md#section5_2
// https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/gluLookAt.xml
// https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/gluPerspective.xml

void camera_update_projection(struct camera* c) {
	float tanHalfFov = tan(c->fov / 2.0);

	memset(c->proj, 0, sizeof(c->proj));
//	c->proj[0][0] = 1.0 / (c->ratio * tanHalfFov);
	c->proj[0][0] = 1.0 / (tanHalfFov);
	c->proj[1][1] = 1.0 / (tanHalfFov);
	c->proj[2][2] = -(c->zFar + c->zNear) / (c->zFar - c->zNear);
	c->proj[2][3] = -1.0;
	c->proj[3][2] = -(2.0 * c->zFar * c->zNear) / (c->zFar - c->zNear);
}

void
camera_set(struct camera* c, float fov, float ratio)
{
	zero3v(c->position);
	quaternion_load_id(c->rotation);
	load_id4(c->view);
	load_id4(c->proj);

	c->fov = fov;
	c->ratio = ratio;
	c->zNear = 0.1;
	c->zFar = 2000;

	camera_update_view(c);
	camera_update_projection(c);
}

void
camera_move(struct camera* c, vec3 off)
{	
	incr3v(c->position, off);
	camera_update_view(c);
}

void
camera_rotate(struct camera* c, vec3 axis, float angle) {
	quaternion q, old;

	quaternion_set_axis_angle(q, axis, angle);
	memcpy(old, c->rotation, sizeof(quaternion));
	quaternion_mul(c->rotation, q, old);
	camera_update_view(c);
}

int
main(int argc, char **argv)
{
	struct shader shad;
	struct shader normal_shad;
	struct mesh box;
	GLint ret;

	app_name = argv[0];

	init_engine();
	ret = shader_load(&shad, vert, frag, NULL);
	ret = shader_load(&normal_shad, normal_vert, normal_frag, normal_geom);
	box_load(&box);
	camera_set(&cam, 1.04, (float)width / (float)height);
	camera_move(&cam, (vec3){ 0, 0, -5});

	while (!engine_shutdown()) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		engine_poll();
		render_box(&cam, &shad, &box);
		render_box(&cam, &normal_shad, &box);
		engine_swap(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	shader_free(&shad);
	fini_engine();

	return 0;
}

static void
camera_bind(struct shader *s, struct camera *c)
{
	mat4 p;
	GLint proj;
	GLint view;
	GLint ratio;

	proj = glGetUniformLocation(s->prog, "proj");
	view = glGetUniformLocation(s->prog, "view");

	memcpy(p, c->proj, sizeof(p));
	p[0][0] /= (float)width / (float)height;
	
	glProgramUniformMatrix4fv(s->prog, proj, 1, GL_FALSE, (float *)p);
	glProgramUniformMatrix4fv(s->prog, view, 1, GL_FALSE, (float *)c->view);
}

static void
render_box(struct camera *c, struct shader *s, struct mesh *m)
{
	GLint position;
	GLint normal;
	GLint texcoord;
	static int once = 1;

	if (1) {
		once = 0;
		position = glGetAttribLocation(s->prog, "in_pos");
		normal = glGetAttribLocation(s->prog, "in_normal");
		texcoord = glGetAttribLocation(s->prog, "in_texcoord");
		mesh_bind(m, position, normal, texcoord);
	}
	GLint time = glGetUniformLocation(s->prog, "time");
	if (time >= 0)
		glProgramUniform1f(s->prog, time, glfwGetTime());

	glUseProgram(s->prog);
	glBindVertexArray(m->vao);
	glEnable(GL_CULL_FACE);  glCullFace(GL_BACK);
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST); glDepthFunc(GL_LESS);

	if (xray)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	camera_bind(s, c);
        glDrawArrays(m->primitive, 0, m->vertex_count);

	glBindVertexArray(0);
	glUseProgram(0);
}

static void
box_load(struct mesh *m)
{
	/* 
  a,b,-c +---+  -a,b,-c
        /   /|
a,b,c  +---+ -a,b,c
       |\, | |
       |  \| / -a,-b,-c
a,-b,c +---+'-a,-b,c
	 */
	float a = 0.5, b = 0.5, c = 0.5;
	float normals[] = {
		 0,  0,  1,    0,  0,  1,    0,  0,  1,    0,  0,  1,    0,  0,  1,    0,  0,  1,
		 0,  0, -1,    0,  0, -1,    0,  0, -1,    0,  0, -1,    0,  0, -1,    0,  0, -1,
		 1,  0,  0,    1,  0,  0,    1,  0,  0,    1,  0,  0,    1,  0,  0,    1,  0,  0,
		-1,  0,  0,   -1,  0,  0,   -1,  0,  0,   -1,  0,  0,   -1,  0,  0,   -1,  0,  0,
		 0,  1,  0,    0,  1,  0,    0,  1,  0,    0,  1,  0,    0,  1,  0,    0,  1,  0,
		 0, -1,  0,    0, -1,  0,    0, -1,  0,    0, -1,  0,    0, -1,  0,    0, -1,  0,
	};
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

static void
quad_load(struct mesh *m)
{
	static float quad_pos[] = {
		-1.0, -1.0,  0,
		-1.0,  1.0,  0,
		 1.0, -1.0,  0,
		 1.0,  1.0,  0,
	};
	static float quad_txc[] = {
		0.0, 0.0,
		0.0, 1.0,
		1.0, 0.0,
		1.0, 1.0,
	};

	mesh_load(m, 4, quad_pos, NULL, quad_txc);
	m->primitive = GL_TRIANGLE_STRIP;
}

static void
render_quad(struct shader *s, struct mesh *m)
{
	GLint position;
	GLint texcoord;
	static int once = 1;

	if (once) {
		once = 0;
		position = glGetAttribLocation(s->prog, "in_pos");
		texcoord = glGetAttribLocation(s->prog, "in_texcoord");
		mesh_bind(m, position, -1, texcoord);
	}

	glUseProgram(s->prog);
	glBindVertexArray(m->vao);

        glDrawArrays(m->primitive, 0, m->vertex_count);

	glBindVertexArray(0);
	glUseProgram(0);
}
