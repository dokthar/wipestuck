#ifndef CAMERA_H
#define CAMERA_H

struct camera {
	vec3 position;
	quaternion rotation;
	float ratio, fov, zNear, zFar;

	mat4 proj, view;
};
void camera_set(struct camera* c, float fov, float ratio);
void camera_bind_view(struct camera *c, GLuint prog, GLint loc);
void camera_bind_proj(struct camera *c, GLuint prog, GLint loc, float ratio);
void camera_get_up(struct camera* c, vec3 up);
void camera_get_dir(struct camera* c, vec3 dir);
void camera_get_left(struct camera* c, vec3 left);
void camera_set_position(struct camera* c, vec3 pos);
void camera_set_rotation(struct camera *c, quaternion q);
void camera_move(struct camera* c, vec3 off);
void camera_rotate(struct camera* c, vec3 axis, float angle);

#endif
