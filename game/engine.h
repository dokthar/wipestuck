#ifndef ENGINE_H
#define ENGINE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

extern char *app_name;
extern unsigned int width;
extern unsigned int height;
extern void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
extern void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

void init_engine(void);
void fini_engine(void);
int  engine_shutdown(void);
void engine_poll(void);
void engine_swap(GLbitfield clear);

#include "util.h"
#include "math.h"

#include "mesh.h"

struct camera {
	vec3 position;
	quaternion rotation;
	float ratio, fov, zNear, zFar;

	mat4 proj, view;
};
void camera(struct camera *);

struct shader {
	GLuint prog;
	GLuint vert;
	GLuint frag;
	GLuint geom;
};
GLint shader_load(struct shader *s, const char *vertex, const char *fragment, const char *geom);
void shader_free(struct shader *s);


#endif
