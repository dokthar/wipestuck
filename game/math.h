#ifndef MATH_H
#define MATH_H

#define M_PI 3.14159265358979

/* If c99 is available, use the restrict keyword
 * to allow the compiler to do vectorial optimizations
 */
#if __STDC_VERSION__ >= 199901L
#define RESTRICT_VEC2(var) float var[restrict 2]
#define RESTRICT_VEC3(var) float var[restrict 3]
#define RESTRICT_VEC4(var) float var[restrict 4]
#define RESTRICT_MAT2(var) float var[restrict 2][2]
#define RESTRICT_MAT3(var) float var[restrict 3][3]
#define RESTRICT_MAT4(var) float var[restrict 4][4]
#else
#define RESTRICT_VEC2(var) vec2 var
#define RESTRICT_VEC3(var) vec3 var
#define RESTRICT_VEC4(var) vec4 var
#define RESTRICT_MAT2(var) mat2 var
#define RESTRICT_MAT3(var) mat3 var
#define RESTRICT_MAT4(var) mat4 var
#endif

typedef float vec2[2];
typedef float vec3[3];
typedef float vec4[4];
typedef vec2 mat2[2];
typedef vec3 mat3[3];
typedef vec4 mat4[4];

extern const vec3 VEC3_AXIS_X;
extern const vec3 VEC3_AXIS_Y;
extern const vec3 VEC3_AXIS_Z;

/* vec2 */
void zero2v(vec2 v);
void print2v(vec2 v);
void add2v(RESTRICT_VEC2(dest), RESTRICT_VEC2(u), RESTRICT_VEC2(v));
void incr2v(RESTRICT_VEC2(dest), RESTRICT_VEC2(incr));
void sub2v(RESTRICT_VEC2(dest), RESTRICT_VEC2(u), RESTRICT_VEC2(v));
void decr2v(RESTRICT_VEC2(dest), RESTRICT_VEC2(decr));
void mul2sv(RESTRICT_VEC2(dest), float s, RESTRICT_VEC2(v));
void scale2v(vec2 dest, float s);
void neg2v(vec2 v);
float norm2sq(vec2 v);
float norm2(vec2 v);
void normalize2(vec2 v);
float dot2(RESTRICT_VEC2(u), RESTRICT_VEC2(v));

/* vec3 */
void zero3v(vec3 v);
void print3v(vec3 v);
void add3v(RESTRICT_VEC3(dest), RESTRICT_VEC3(u), RESTRICT_VEC3(v));
void incr3v(RESTRICT_VEC3(dest), RESTRICT_VEC3(incr));
void sub3v(RESTRICT_VEC3(dest), RESTRICT_VEC3(u), RESTRICT_VEC3(v));
void decr3v(RESTRICT_VEC3(dest), RESTRICT_VEC3(decr));
void mul3sv(RESTRICT_VEC3(dest), float s, RESTRICT_VEC3(v));
void scale3v(vec3 dest, float s);
void neg3v(vec3 v);
float norm3sq(vec3 v);
float norm3(vec3 v);
void normalize3(vec3 v);
float dot3(RESTRICT_VEC3(u), RESTRICT_VEC3(v));
void cross3(RESTRICT_VEC3(dest), RESTRICT_VEC3(u), RESTRICT_VEC3(v));

/* vec4 */
void zero4v(vec4 v);
void print4v(vec4 v);
void add4v(RESTRICT_VEC4(dest), RESTRICT_VEC4(u), RESTRICT_VEC4(v));
void incr4v(RESTRICT_VEC4(dest), RESTRICT_VEC4(incr));
void sub4v(RESTRICT_VEC4(dest), RESTRICT_VEC4(u), RESTRICT_VEC4(v));
void decr4v(RESTRICT_VEC4(dest), RESTRICT_VEC4(decr));
void mul4sv(RESTRICT_VEC4(dest), float s, RESTRICT_VEC4(v));
void scale4v(vec4 dest, float s);
void neg4v(vec4 v);
float norm4sq(vec4 v);
float norm4(vec4 v);
void normalize4(vec4 v);
float dot4(RESTRICT_VEC4(u), RESTRICT_VEC4(v));

/* mat2 */
void load_id2(mat2 dest);
void print2m(mat2 m);
void mul2sm(RESTRICT_MAT2(dest), float s, RESTRICT_MAT2(m));
void mul2mv(RESTRICT_VEC2(dest), RESTRICT_MAT2(m), RESTRICT_VEC2(v));
void mul2mm(RESTRICT_MAT2(dest), RESTRICT_MAT2(a), RESTRICT_MAT2(b));
void neg2m(mat2 m);
void transpose2m(mat2 m);
float det2(mat2 m);
int invert2m(RESTRICT_MAT2(dest), RESTRICT_MAT2(src));

/* mat3 */
void load_id3(mat3 dest);
void print3m(mat3 m);
void mul3sm(RESTRICT_MAT3(dest), float s, RESTRICT_MAT3(m));
void mul3mv(RESTRICT_VEC3(dest), RESTRICT_MAT3(m), RESTRICT_VEC3(v));
void mul3mm(RESTRICT_MAT3(dest), RESTRICT_MAT3(a), RESTRICT_MAT3(b));
void neg3m(mat3 m);
void transpose3m(mat3 m);
float det3(mat3 m);
int invert3m(RESTRICT_MAT3(dest), RESTRICT_MAT3(src));


/* mat4 */
void load_id4(mat4 dest);
void print4m(mat4 m);
void mul4sm(RESTRICT_MAT4(dest), float s, RESTRICT_MAT4(m));
void mul4mv(RESTRICT_VEC4(res), RESTRICT_MAT4(m), RESTRICT_VEC4(v));
void mul4mm(RESTRICT_MAT4(res), RESTRICT_MAT4(a), RESTRICT_MAT4(b));
void neg4m(mat4 m);
void transpose4m(mat4 m);

/* Conversions */
void row3m3(RESTRICT_VEC3(dest), RESTRICT_MAT3(m), unsigned int row);
void row3m4(RESTRICT_VEC3(dest), RESTRICT_MAT4(m), unsigned int row);
void row4m4(RESTRICT_VEC4(dest), RESTRICT_MAT4(m), unsigned int row);
void vec4to3(RESTRICT_VEC3(dest), RESTRICT_VEC4(src));
void mat4to3(RESTRICT_MAT3(dest), RESTRICT_MAT4(src));
void mat3to4(RESTRICT_MAT4(dest), RESTRICT_MAT3(src));

/* Rotations */
void load_rot3(RESTRICT_MAT3(dest), RESTRICT_VEC3(axis), float angle);
void load_rot4(RESTRICT_MAT4(dest), RESTRICT_VEC3(axis), float angle);
void compute_rotation(RESTRICT_VEC3(u), RESTRICT_VEC3(v), RESTRICT_VEC3(axis), float* angle);

typedef vec4 quaternion;

void quaternion_load_id(quaternion q);
void quaternion_set_axis_angle(quaternion q, vec3 axis, float angle);
void quaternion_get_axis(quaternion q, vec3 axis);
float quaternion_get_angle(quaternion q);
void quaternion_set_angle(quaternion q, float angle);

void quaternion_mul(quaternion dest, quaternion a, quaternion b);
float quaternion_norm_sq(quaternion q);
void quaternion_conj(quaternion q);
void quaternion_inv(quaternion q);
void quaternion_compose(vec3 dest, quaternion q, vec3 v);
void quaternion_to_mat3(mat3 dest, quaternion q);
void quaternion_to_mat4(mat4 dest, quaternion q);
void quaternion_from_mat3(quaternion dest, mat3 src);
void quaternion_from_mat4(quaternion dest, mat4 src);
void quaternion_decompose_swing_twist(quaternion src, vec3 direction, quaternion swing, quaternion twist);
void quaternion_print(quaternion q);

#endif
