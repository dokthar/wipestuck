#ifndef CAMERA_H
#define CAMERA_H

struct camera {
	vec3 position;
	quaternion rotation;
	float ratio, fov, zNear, zFar;

	mat4 proj, view;
};
void camera_set(struct camera* c, float fov, float ratio);
void camera_get_view(struct camera *c, mat4 view);
void camera_get_proj(struct camera *c, mat4 proj);
void camera_get_up(struct camera* c, vec3 up);
void camera_get_dir(struct camera* c, vec3 dir);
void camera_get_left(struct camera* c, vec3 left);
void camera_move(struct camera* c, vec3 off);
void camera_rotate(struct camera* c, vec3 axis, float angle);

#endif
