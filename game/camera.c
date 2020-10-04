#include <string.h>
#include "engine.h"

static void camera_update_view(struct camera *c);
static void camera_update_proj(struct camera *c);

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
	camera_update_proj(c);
}
void
camera_bind_view(struct camera *c, GLuint prog, GLint loc)
{
	glUniformMatrix4fv(loc, 1, GL_FALSE, (float *)c->view);
}

void
camera_bind_proj(struct camera *c, GLuint prog, GLint loc, float ratio)
{
	mat4 p;

	memcpy(p, c->proj, sizeof(p));
	p[0][0] /= ratio;
	glUniformMatrix4fv(loc, 1, GL_FALSE, (float *)p);
}

void
camera_get_left(struct camera *c, vec3 left)
{
	mat3 rot;
	quaternion_to_rot3(rot, c->rotation);
	row3m3(left, rot, 0);
}

void
camera_get_up(struct camera *c, vec3 up)
{
	mat3 rot;
	quaternion_to_rot3(rot, c->rotation);
	row3m3(up, rot, 1);
}

void
camera_get_dir(struct camera *c, vec3 dir)
{
	mat3 rot;
	quaternion_to_rot3(rot, c->rotation);
	row3m3(dir, rot, 2);
}

void
camera_set_position(struct camera *c, vec3 pos)
{
	memcpy(c->position, pos, sizeof(c->position));
	camera_update_view(c);
}

void
camera_move(struct camera *c, vec3 off)
{
	incr3v(c->position, off);
	camera_update_view(c);
}

void
camera_apply(struct camera *c, quaternion q)
{
	quaternion old;

	memcpy(old, c->rotation, sizeof(old));
	quaternion_mul(c->rotation, q, old);
	camera_update_view(c);
}

void
camera_rotate(struct camera *c, vec3 axis, float angle)
{
	quaternion q, old;

	quaternion_set_axis_angle(q, axis, angle);
	camera_apply(c, q);
}

void
camera_set_rotation(struct camera *c, quaternion q)
{
	memcpy(c->rotation, q, sizeof(c->rotation));
	camera_update_view(c);
}

/**
 * Calcule le quartenion permettant de tourner la camera de sa position
 * courante vers l'objet a observer. Si la camera tourne le dos a l'objet 
 * on utilise up (axe vertical de la camera comme axe de rotation)
 */
void
rotation_between_vec3(quaternion q, vec3 start, vec3 dest, vec3 up) {
	vec3 u, s, d;
	float cosa;

	memcpy(s, start, sizeof(s));
	memcpy(d, dest, sizeof(d));
	normalize3(s);
	normalize3(d);

	cosa = dot3(s, d);
	if (fabsf(cosa) > (1 - 0.00001)) {
		memcpy(u, up, sizeof(u));
	} else {
		cross3(u, s, d);
	}

	quaternion_set_axis_angle(q, u, acos(cosa));
}

#if 1
void
camera_look_at(struct camera *c, vec3 look_at, vec3 up)
{
	vec3 start, dest;
	quaternion rot, old;

	camera_get_dir(c, start);
	sub3v(dest, look_at, c->position);

	rotation_between_vec3(rot, start, dest, up);

	memcpy(old, c->rotation, sizeof(old));
	quaternion_mul(c->rotation, rot, old);
	camera_update_view(c);
}
#else
void
camera_look_at(struct camera *c, vec3 look_at, vec3 up)
{
	vec3 s, u, f;
	sub3v(f, look_at, c->position);
	normalize3(f);

	float a = dot3((vec3){0,0,1}, f);
	cross3(u, (vec3){0,0,1}, f);
	normalize3(u);

	quaternion_set_axis_angle(c->rotation, u, acos(a));

	camera_update_view(c);
#if 0	
	cross3(s, up, f);
	normalize3(s);
	cross3(u, f, s);
	normalize3(u);
#endif
}
#endif
static void
camera_update_view(struct camera *c)
{
	mat3 rot;
	float x, y, z;
	vec3 s, u, f;

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

static void
camera_update_proj(struct camera *c)
{
	float tanHalfFov = tan(c->fov / 2.0);

	memset(c->proj, 0, sizeof(c->proj));
//	c->proj[0][0] = 1.0 / (c->ratio * tanHalfFov);
	c->proj[0][0] = 1.0 / (tanHalfFov);
	c->proj[1][1] = 1.0 / (tanHalfFov);
	c->proj[2][2] = -(c->zFar + c->zNear) / (c->zFar - c->zNear);
	c->proj[2][3] = -1.0;
	c->proj[3][2] = -(2.0 * c->zFar * c->zNear) / (c->zFar - c->zNear);
}
