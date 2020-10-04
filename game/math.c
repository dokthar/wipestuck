#include <stdio.h>
#include <string.h>
#include "math.h"

const vec3 VEC3_AXIS_X = {1,0,0};
const vec3 VEC3_AXIS_Y = {0,1,0};
const vec3 VEC3_AXIS_Z = {0,0,1};

#define SWAP(a, b) tmp = a; a = b; b = tmp;
#define VAL "%8.2f"
#define SEP ", "

/* vec2 */
void zero2v(vec2 v) {
	v[0] = 0;
	v[1] = 0;
}

void print2v(vec2 v) {
	printf("/" VAL " \\\n\\" VAL " /\n", v[0], v[1]);
}

void add2v(RESTRICT_VEC2(dest), RESTRICT_VEC2(u), RESTRICT_VEC2(v)) {
	dest[0] = u[0] + v[0];
	dest[1] = u[1] + v[1];
}

void incr2v(RESTRICT_VEC2(dest), RESTRICT_VEC2(incr)) {
	dest[0] += incr[0];
	dest[1] += incr[1];
}

void sub2v(RESTRICT_VEC2(dest), RESTRICT_VEC2(u), RESTRICT_VEC2(v)) {
	dest[0] = u[0] - v[0];
	dest[1] = u[1] - v[1];
}

void decr2v(RESTRICT_VEC2(dest), RESTRICT_VEC2(decr)) {
	dest[0] -= decr[0];
	dest[1] -= decr[1];
}

void mul2sv(RESTRICT_VEC2(dest), float s, RESTRICT_VEC2(v)) {
	dest[0] = s * v[0];
	dest[1] = s * v[1];
}

void scale2v(vec2 dest, float s) {
	dest[0] *= s;
	dest[1] *= s;
}

void neg2v(vec2 v) {
	v[0] = -v[0];
	v[1] = -v[1];
}

float norm2sq(vec2 v) {
	return v[0] * v[0] + v[1] * v[1];
}

float norm2(vec2 v) {
	return sqrt(norm2sq(v));
}

void normalize2(vec2 v) {
	float n = norm2(v);
	v[0] /= n;
	v[1] /= n;
}

float dot2(RESTRICT_VEC2(u), RESTRICT_VEC2(v)) {
	return u[0] * v[0] + u[1] * v[1];
}


/* vec3 */
void zero3v(vec3 v) {
	v[0] = 0;
	v[1] = 0;
	v[2] = 0;
}

void print3v(vec3 v) {
	printf("/" VAL " \\\n|" VAL " |\n\\" VAL " /\n", v[0], v[1], v[2]);
}

void add3v(RESTRICT_VEC3(dest), RESTRICT_VEC3(u), RESTRICT_VEC3(v)) {
	dest[0] = u[0] + v[0];
	dest[1] = u[1] + v[1];
	dest[2] = u[2] + v[2];
}

void incr3v(RESTRICT_VEC3(dest), RESTRICT_VEC3(incr)) {
	dest[0] += incr[0];
	dest[1] += incr[1];
	dest[2] += incr[2];
}

void sub3v(RESTRICT_VEC3(dest), RESTRICT_VEC3(u), RESTRICT_VEC3(v)) {
	dest[0] = u[0] - v[0];
	dest[1] = u[1] - v[1];
	dest[2] = u[2] - v[2];
}

void decr3v(RESTRICT_VEC3(dest), RESTRICT_VEC3(decr)) {
	dest[0] -= decr[0];
	dest[1] -= decr[1];
	dest[2] -= decr[2];
}

void mul3sv(RESTRICT_VEC3(dest), float s, RESTRICT_VEC3(v)) {
	dest[0] = s * v[0];
	dest[1] = s * v[1];
	dest[2] = s * v[2];
}

void scale3v(vec3 dest, float s) {
	dest[0] *= s;
	dest[1] *= s;
	dest[2] *= s;
}

void neg3v(vec3 v) {
	v[0] = -v[0];
	v[1] = -v[1];
	v[2] = -v[2];
}

float norm3sq(vec3 v) {
	return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
}

float norm3(vec3 v) {
	return sqrt(norm3sq(v));
}

void normalize3(vec3 v) {
	float n = norm3(v);
	v[0] /= n;
	v[1] /= n;
	v[2] /= n;
}

float dot3(RESTRICT_VEC3(u), RESTRICT_VEC3(v)) {
	return u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
}

void cross3(RESTRICT_VEC3(dest), RESTRICT_VEC3(u), RESTRICT_VEC3(v)) {
	dest[0] = u[1] * v[2] - u[2] * v[1];
	dest[1] = u[2] * v[0] - u[0] * v[2];
	dest[2] = u[0] * v[1] - u[1] * v[0];
}

/* vec4 */
void zero4v(vec4 v) {
	v[0] = 0;
	v[1] = 0;
	v[2] = 0;
	v[3] = 0;
}

void print4v(vec4 v) {
	printf("/" VAL " \\\n|" VAL " |\n|" VAL " |\n\\" VAL " /\n", v[0], v[1], v[2], v[3]);
}

void add4v(RESTRICT_VEC4(dest), RESTRICT_VEC4(u), RESTRICT_VEC4(v)) {
	dest[0] = u[0] + v[0];
	dest[1] = u[1] + v[1];
	dest[2] = u[2] + v[2];
	dest[3] = u[3] + v[3];
}

void incr4v(RESTRICT_VEC4(dest), RESTRICT_VEC4(incr)) {
	dest[0] += incr[0];
	dest[1] += incr[1];
	dest[2] += incr[2];
	dest[3] += incr[3];
}

void sub4v(RESTRICT_VEC4(dest), RESTRICT_VEC4(u), RESTRICT_VEC4(v)) {
	dest[0] = u[0] - v[0];
	dest[1] = u[1] - v[1];
	dest[2] = u[2] - v[2];
	dest[3] = u[3] - v[3];
}

void decr4v(RESTRICT_VEC4(dest), RESTRICT_VEC4(decr)) {
	dest[0] -= decr[0];
	dest[1] -= decr[1];
	dest[2] -= decr[2];
	dest[3] -= decr[3];
}

void mul4sv(RESTRICT_VEC4(dest), float s, RESTRICT_VEC4(v)) {
	dest[0] = s * v[0];
	dest[1] = s * v[1];
	dest[2] = s * v[2];
	dest[3] = s * v[3];
}

void scale4v(vec4 dest, float s) {
	dest[0] *= s;
	dest[1] *= s;
	dest[2] *= s;
	dest[3] *= s;
}

void neg4v(vec4 v) {
	v[0] = -v[0];
	v[1] = -v[1];
	v[2] = -v[2];
	v[3] = -v[3];
}

float norm4sq(vec4 v) {
	return v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3];
}

float norm4(vec4 v) {
	return sqrt(norm4sq(v));
}

void normalize4(vec4 v) {
	float n = norm4(v);
	v[0] /= n;
	v[1] /= n;
	v[2] /= n;
	v[3] /= n;
}

float dot4(RESTRICT_VEC4(u), RESTRICT_VEC4(v)) {
	return u[0] * v[0] + u[1] * v[1] + u[2] * v[2] + u[3] * v[3];
}

/* mat2 */
void load_id2(mat2 dest) {
	memset(dest, 0, sizeof(mat2));
	dest[0][0] = 1;
	dest[1][1] = 1;
}

void print2m(mat2 m) {
	printf(
		"/"  VAL SEP VAL " \\\n"
		"\\" VAL SEP VAL " /\n",
		m[0][0], m[1][0],
		m[0][1], m[1][1]
		);
}

void mul2sm(RESTRICT_MAT2(dest), float s, RESTRICT_MAT2(m)) {
	dest[0][0] = m[0][0] * s;
	dest[0][1] = m[0][1] * s;
	dest[1][0] = m[1][0] * s;
	dest[1][1] = m[1][1] * s;
}

void mul2mv(RESTRICT_VEC2(dest), RESTRICT_MAT2(m), RESTRICT_VEC2(v)) {
	dest[0] = m[0][0] * v[0] + m[1][0] * v[1];
	dest[1] = m[0][1] * v[0] + m[1][1] * v[1];
}

void mul2mm(RESTRICT_MAT2(dest), RESTRICT_MAT2(a), RESTRICT_MAT2(b)) {
	dest[0][0] = a[0][0] * b[0][0] + a[1][0] * b[0][1];
	dest[0][1] = a[0][1] * b[0][0] + a[1][1] * b[0][1];
	dest[1][0] = a[0][0] * b[1][0] + a[1][0] * b[1][1];
	dest[1][1] = a[0][1] * b[1][0] + a[1][1] * b[1][1];
}

void neg2m(mat2 m) {
	m[0][0] = -m[0][0];
	m[0][1] = -m[0][1];
	m[1][0] = -m[1][0];
	m[1][1] = -m[1][1];
}

void transpose2m(mat2 m) {
	float tmp;
	SWAP(m[0][1], m[1][0]);
}

float det2(mat2 m) {
	return m[0][0] * m[1][1] - m[0][1] * m[1][0];
}

int invert2m(RESTRICT_MAT2(dest), RESTRICT_MAT2(src)) {
	float det = det2(src);

	if (det != 0) {
		float oneOverDet = 1.0 / det;

		dest[0][0] = oneOverDet * src[1][1];
		dest[0][1] = -oneOverDet * src[0][1];

		dest[1][0] = -oneOverDet * src[1][0];
		dest[1][1] = oneOverDet * src[0][0];

		return 1;
	}

	return 0;
}

/* mat3 */
void load_id3(mat3 dest) {
	memset(dest, 0, sizeof(mat3));
	dest[0][0] = 1;
	dest[1][1] = 1;
	dest[2][2] = 1;
}

void print3m(mat3 m) {
	printf(
		"/"  VAL SEP VAL SEP VAL " \\\n"
		"|"  VAL SEP VAL SEP VAL " |\n"
		"\\" VAL SEP VAL SEP VAL " /\n",
		m[0][0], m[1][0], m[2][0],
		m[0][1], m[1][1], m[2][1],
		m[0][2], m[1][2], m[2][2]
		);
}

void mul3sm(RESTRICT_MAT3(dest), float s, RESTRICT_MAT3(m)) {
	dest[0][0] = m[0][0] * s;
	dest[0][1] = m[0][1] * s;
	dest[0][2] = m[0][2] * s;
	dest[1][0] = m[1][0] * s;
	dest[1][1] = m[1][1] * s;
	dest[1][2] = m[1][2] * s;
	dest[2][0] = m[2][0] * s;
	dest[2][1] = m[2][1] * s;
	dest[2][2] = m[2][2] * s;
}

void mul3mv(RESTRICT_VEC3(dest), RESTRICT_MAT3(m), RESTRICT_VEC3(v)) {
	dest[0] = m[0][0] * v[0] + m[1][0] * v[1] + m[2][0] * v[2];
	dest[1] = m[0][1] * v[0] + m[1][1] * v[1] + m[2][1] * v[2];
	dest[2] = m[0][2] * v[0] + m[1][2] * v[1] + m[2][2] * v[2];
}

void mul3mm(RESTRICT_MAT3(dest), RESTRICT_MAT3(a), RESTRICT_MAT3(b)) {
	dest[0][0] = a[0][0] * b[0][0] + a[1][0] * b[0][1] + a[2][0] * b[0][2];
	dest[0][1] = a[0][1] * b[0][0] + a[1][1] * b[0][1] + a[2][1] * b[0][2];
	dest[0][2] = a[0][2] * b[0][0] + a[1][2] * b[0][1] + a[2][2] * b[0][2];
	dest[1][0] = a[0][0] * b[1][0] + a[1][0] * b[1][1] + a[2][0] * b[1][2];
	dest[1][1] = a[0][1] * b[1][0] + a[1][1] * b[1][1] + a[2][1] * b[1][2];
	dest[1][2] = a[0][2] * b[1][0] + a[1][2] * b[1][1] + a[2][2] * b[1][2];
	dest[2][0] = a[0][0] * b[2][0] + a[1][0] * b[2][1] + a[2][0] * b[2][2];
	dest[2][1] = a[0][1] * b[2][0] + a[1][1] * b[2][1] + a[2][1] * b[2][2];
	dest[2][2] = a[0][2] * b[2][0] + a[1][2] * b[2][1] + a[2][2] * b[2][2];
}

void neg3m(mat3 m) {
	m[0][0] = -m[0][0];
	m[0][1] = -m[0][1];
	m[0][2] = -m[0][2];
	m[1][0] = -m[1][0];
	m[1][1] = -m[1][1];
	m[1][2] = -m[1][2];
	m[2][0] = -m[2][0];
	m[2][1] = -m[2][1];
	m[2][2] = -m[2][2];
}

void transpose3m(mat3 m) {
	float tmp;
	SWAP(m[0][1], m[1][0]);
	SWAP(m[0][2], m[2][0]);
	SWAP(m[1][2], m[2][1]);
}

float det3(mat3 m) {
	return m[0][0] * m[1][1] * m[2][2] + m[0][1] * m[1][2] * m[2][0] + m[1][0] * m[2][1] * m[0][2]
		- m[1][1] * m[0][2] * m[2][0] - m[0][0] * m[1][2] * m[2][1] - m[0][1] * m[1][0] * m[2][2];
}

int invert3m(RESTRICT_MAT3(dest), RESTRICT_MAT3(src)) {
	float det = det3(src);

	if (det != 0) {
		float oneOverDet = 1.0 / det;

		dest[0][0] = oneOverDet * (src[1][1] * src[2][2] - src[2][1] * src[1][2]);
		dest[0][1] = oneOverDet * (src[2][1] * src[0][2] - src[0][1] * src[2][2]);
		dest[0][2] = oneOverDet * (src[0][1] * src[1][2] - src[1][1] * src[0][2]);

		dest[1][0] = oneOverDet * (src[2][0] * src[1][2] - src[1][0] * src[2][2]);
		dest[1][1] = oneOverDet * (src[0][0] * src[2][2] - src[2][0] * src[0][2]);
		dest[1][2] = oneOverDet * (src[1][0] * src[0][2] - src[0][0] * src[1][2]);

		dest[2][0] = oneOverDet * (src[1][0] * src[2][1] - src[1][1] * src[2][0]);
		dest[2][1] = oneOverDet * (src[2][0] * src[0][1] - src[0][0] * src[2][1]);
		dest[2][2] = oneOverDet * (src[0][0] * src[1][1] - src[1][0] * src[0][1]);

		return 1;
	}

	return 0;
}

/* mat4 */
void load_id4(mat4 dest) {
	memset(dest, 0, sizeof(mat4));
	dest[0][0] = 1;
	dest[1][1] = 1;
	dest[2][2] = 1;
	dest[3][3] = 1;
}

void print4m(mat4 m) {
	printf(
		"/"  VAL SEP VAL SEP VAL SEP VAL " \\\n"
		"|"  VAL SEP VAL SEP VAL SEP VAL " |\n"
		"|"  VAL SEP VAL SEP VAL SEP VAL " |\n"
		"\\" VAL SEP VAL SEP VAL SEP VAL " /\n",
		m[0][0], m[1][0], m[2][0], m[3][0],
		m[0][1], m[1][1], m[2][1], m[3][1],
		m[0][2], m[1][2], m[2][2], m[3][2],
		m[0][3], m[1][3], m[2][3], m[3][3]
		);
}

void mul4sm(RESTRICT_MAT4(dest), float s, RESTRICT_MAT4(m)) {
	dest[0][0] = m[0][0] * s;
	dest[0][1] = m[0][1] * s;
	dest[0][2] = m[0][2] * s;
	dest[0][3] = m[0][3] * s;
	dest[1][0] = m[1][0] * s;
	dest[1][1] = m[1][1] * s;
	dest[1][2] = m[1][2] * s;
	dest[1][3] = m[1][3] * s;
	dest[2][0] = m[2][0] * s;
	dest[2][1] = m[2][1] * s;
	dest[2][2] = m[2][2] * s;
	dest[2][3] = m[2][3] * s;
	dest[3][0] = m[3][0] * s;
	dest[3][1] = m[3][1] * s;
	dest[3][2] = m[3][2] * s;
	dest[3][3] = m[3][3] * s;
}

void mul4mv(RESTRICT_VEC4(res), RESTRICT_MAT4(m), RESTRICT_VEC4(v)) {
	
	
	res[0] = m[0][0] * v[0] + m[1][0] * v[1] + m[2][0] * v[2] + m[3][0] * v[3];
	res[1] = m[0][1] * v[0] + m[1][1] * v[1] + m[2][1] * v[2] + m[3][1] * v[3];
	res[2] = m[0][2] * v[0] + m[1][2] * v[1] + m[2][2] * v[2] + m[3][2] * v[3];
	res[3] = m[0][3] * v[0] + m[1][3] * v[1] + m[2][3] * v[2] + m[3][3] * v[3];
}

void mul4mm(RESTRICT_MAT4(res), RESTRICT_MAT4(a), RESTRICT_MAT4(b)) {
	res[0][0] = a[0][0] * b[0][0] + a[1][0] * b[0][1] + a[2][0] * b[0][2] + a[3][0] * b[0][3];
	res[0][1] = a[0][1] * b[0][0] + a[1][1] * b[0][1] + a[2][1] * b[0][2] + a[3][1] * b[0][3];
	res[0][2] = a[0][2] * b[0][0] + a[1][2] * b[0][1] + a[2][2] * b[0][2] + a[3][2] * b[0][3];
	res[0][3] = a[0][3] * b[0][0] + a[1][3] * b[0][1] + a[2][3] * b[0][2] + a[3][3] * b[0][3];
	res[1][0] = a[0][0] * b[1][0] + a[1][0] * b[1][1] + a[2][0] * b[1][2] + a[3][0] * b[1][3];
	res[1][1] = a[0][1] * b[1][0] + a[1][1] * b[1][1] + a[2][1] * b[1][2] + a[3][1] * b[1][3];
	res[1][2] = a[0][2] * b[1][0] + a[1][2] * b[1][1] + a[2][2] * b[1][2] + a[3][2] * b[1][3];
	res[1][3] = a[0][3] * b[1][0] + a[1][3] * b[1][1] + a[2][3] * b[1][2] + a[3][3] * b[1][3];
	res[2][0] = a[0][0] * b[2][0] + a[1][0] * b[2][1] + a[2][0] * b[2][2] + a[3][0] * b[2][3];
	res[2][1] = a[0][1] * b[2][0] + a[1][1] * b[2][1] + a[2][1] * b[2][2] + a[3][1] * b[2][3];
	res[2][2] = a[0][2] * b[2][0] + a[1][2] * b[2][1] + a[2][2] * b[2][2] + a[3][2] * b[2][3];
	res[2][3] = a[0][3] * b[2][0] + a[1][3] * b[2][1] + a[2][3] * b[2][2] + a[3][3] * b[2][3];
	res[3][0] = a[0][0] * b[3][0] + a[1][0] * b[3][1] + a[2][0] * b[3][2] + a[3][0] * b[3][3];
	res[3][1] = a[0][1] * b[3][0] + a[1][1] * b[3][1] + a[2][1] * b[3][2] + a[3][1] * b[3][3];
	res[3][2] = a[0][2] * b[3][0] + a[1][2] * b[3][1] + a[2][2] * b[3][2] + a[3][2] * b[3][3];
	res[3][3] = a[0][3] * b[3][0] + a[1][3] * b[3][1] + a[2][3] * b[3][2] + a[3][3] * b[3][3];
}

void neg4m(mat4 m) {
	m[0][0] = -m[0][0];
	m[0][1] = -m[0][1];
	m[0][2] = -m[0][2];
	m[0][3] = -m[0][3];
	m[1][0] = -m[1][0];
	m[1][1] = -m[1][1];
	m[1][2] = -m[1][2];
	m[1][3] = -m[1][3];
	m[2][0] = -m[2][0];
	m[2][1] = -m[2][1];
	m[2][2] = -m[2][2];
	m[2][3] = -m[2][3];
	m[3][0] = -m[3][0];
	m[3][1] = -m[3][1];
	m[3][2] = -m[3][2];
	m[3][3] = -m[3][3];
}

void transpose4m(mat4 m) {
	float tmp;
	SWAP(m[0][1], m[1][0]);
	SWAP(m[0][2], m[2][0]);
	SWAP(m[0][3], m[3][0]);
	SWAP(m[1][2], m[2][1]);
	SWAP(m[1][3], m[3][1]);
	SWAP(m[2][3], m[3][2]);
}

/* Conversions */
void row3m3(RESTRICT_VEC3(dest), RESTRICT_MAT3(m), unsigned int row) {
	dest[0] = m[0][row];
	dest[1] = m[1][row];
	dest[2] = m[2][row];
}

void row3m4(RESTRICT_VEC3(dest), RESTRICT_MAT4(m), unsigned int row) {
	dest[0] = m[0][row];
	dest[1] = m[1][row];
	dest[2] = m[2][row];
}

void row4m4(RESTRICT_VEC4(dest), RESTRICT_MAT4(m), unsigned int row) {
	dest[0] = m[0][row];
	dest[1] = m[1][row];
	dest[2] = m[2][row];
	dest[3] = m[3][row];
}

void vec4to3(RESTRICT_VEC3(dest), RESTRICT_VEC4(src)) {
	memcpy(dest, src, sizeof(vec3));
}

void mat4to3(RESTRICT_MAT3(dest), RESTRICT_MAT4(src)) {
	memcpy(dest[0], src[0], sizeof(vec3));
	memcpy(dest[1], src[1], sizeof(vec3));
	memcpy(dest[2], src[2], sizeof(vec3));
}

void mat3to4(RESTRICT_MAT4(dest), RESTRICT_MAT3(src)) {
	memcpy(dest[0], src[0], sizeof(vec3));
	memcpy(dest[1], src[1], sizeof(vec3));
	memcpy(dest[2], src[2], sizeof(vec3));
}

/* Rotations */
void load_rot3(RESTRICT_MAT3(dest), RESTRICT_VEC3(axis), float angle) {
	vec3 a;
	float s = sin(angle), c = cos(angle);
	float xv, xs, yv, ys, zv, zs, ver = 1 - c;

	memcpy(a, axis, sizeof(vec3));
	normalize3(a);

	if (angle == 0) {
		load_id3(dest);
		return;
	}

	xv = ver * a[0];
	xs = a[0] * s;
	yv = ver * a[1];
	ys = a[1] * s;
	zv = ver * a[2];
	zs = a[2] * s;

	dest[0][0] = a[0] * xv + c;
	dest[0][1] = a[0] * yv + zs;
	dest[0][2] = a[0] * zv - ys;

	dest[1][0] = a[1] * xv - zs;
	dest[1][1] = a[1] * yv + c;
	dest[1][2] = a[1] * zv + xs;

	dest[2][0] = a[2] * xv + ys;
	dest[2][1] = a[2] * yv - xs;
	dest[2][2] = a[2] * zv + c;
}

void load_rot4(RESTRICT_MAT4(dest), RESTRICT_VEC3(axis), float angle) {
	vec3 a;
	float s = sin(angle), c = cos(angle);
	float xv, xs, yv, ys, zv, zs, ver = 1 - c;

	memcpy(a, axis, sizeof(vec3));
	normalize3(a);

	if (angle == 0) {
		load_id4(dest);
		return;
	}

	xv = ver * a[0];
	xs = a[0] * s;
	yv = ver * a[1];
	ys = a[1] * s;
	zv = ver * a[2];
	zs = a[2] * s;

	dest[0][0] = a[0] * xv + c;
	dest[0][1] = a[0] * yv + zs;
	dest[0][2] = a[0] * zv - ys;
	dest[0][3] = 0.0;

	dest[1][0] = a[1] * xv - zs;
	dest[1][1] = a[1] * yv + c;
	dest[1][2] = a[1] * zv + xs;
	dest[1][3] = 0.0;

	dest[2][0] = a[2] * xv + ys;
	dest[2][1] = a[2] * yv - xs;
	dest[2][2] = a[2] * zv + c;
	dest[2][3] = 0.0;

	dest[3][0] = 0.0;
	dest[3][1] = 0.0;
	dest[3][2] = 0.0;
	dest[3][3] = 1.0;
}

void compute_rotation(RESTRICT_VEC3(u), RESTRICT_VEC3(v), RESTRICT_VEC3(axis), float* angle) {
	float c, d = norm3(u) * norm3(v);
	if (d == 0.0) {
		*angle = 0.0;
		axis[0] = 0;
		axis[1] = 0;
		axis[2] = 0;
	} else {
		c = dot3(u, v) / d;
		cross3(axis, u, v);
		*angle = (c > 1.0) ? 0.0 : ((c < -1.0) ? M_PI : acos(c));
	}
}

void quaternion_load_id(quaternion q) {
	q[0] = 1;
	q[1] = 0;
	q[2] = 0;
	q[3] = 0;
}
void quaternion_from_rot3(quaternion q, mat3 m);
void quaternion_look_at(quaternion q, vec3 dir, vec3 up) {
	vec3 s, u, f;
	mat3 m;

	memcpy(f, dir, sizeof(f));
	normalize3(f);
	cross3(s, up, f);
	normalize3(s);
	cross3(u, f, s);
	normalize3(u);
#if 0
/*
	vec3 f, s, u;

	zero3v(f);
	decr3v(f, forward);
	normalize3(f); // normalize(-forward)
	cross3(s, up, f);
	normalize3(s);
	cross3(u, f, s);
*/
	
	vars.vect3.set(direction).normalizeLocal();
	vars.vect1.set(up).crossLocal(direction).normalizeLocal();
	vars.vect2.set(direction).crossLocal(vars.vect1).normalizeLocal();

	q[1] = axis[0];
	q[2] = axis[1];
	q[3] = axis[2];
	q[0] = sqrt((v1.Length ^ 2) * (v2.Length ^ 2)) + dot3(up, dir);
#endif
	m[0][0] = s[0];
	m[0][1] = u[0];
	m[0][2] = f[0];
	m[1][0] = s[1];
	m[1][1] = u[1];
	m[1][2] = f[1];
	m[2][0] = s[2];
	m[2][1] = u[2];
	m[2][2] = f[2];

	quaternion_from_rot3(q, m);
}

void quaternion_set_axis_angle(quaternion q, vec3 axis, float angle) {
	float s = sin(angle * 0.5);
	float w = cos(angle * 0.5);

	if (axis[0] == 0 && axis[1] == 0 && axis[2] == 0) {
		quaternion_load_id(q);
	} else {
//		mul3sv(&q[1], sin(angle / 2.0) / norm3(axis), axis);
		q[0] = w;
		q[1] = s * axis[0];
		q[2] = s * axis[1];
		q[3] = s * axis[2];
	}
}

void quaternion_get_axis(quaternion q, vec3 axis) {
	memcpy(axis, &q[1], sizeof(vec3));
	normalize3(axis);
}

float quaternion_get_angle(quaternion q) {
	return 2 * acos(q[0]);
}

void quaternion_set_angle(quaternion q, float angle) {
	q[0] = cos(angle / 2.0);
	if (norm3(&q[1]) > 0.0) {
		scale3v(&q[1], sin(angle / 2.0) / norm3(&q[1]));
	}
}

void quaternion_mul(quaternion dest, quaternion a, quaternion b) {
	dest[0] = a[0] * b[0] - a[1] * b[1] - a[2] * b[2] - a[3] * b[3];
	dest[1] = a[0] * b[1] + a[1] * b[0] + a[2] * b[3] - a[3] * b[2];
	dest[2] = a[0] * b[2] - a[1] * b[3] + a[2] * b[0] + a[3] * b[1];
	dest[3] = a[0] * b[3] + a[1] * b[2] - a[2] * b[1] + a[3] * b[0];
}

void quaternion_conj(quaternion q) {
	neg3v(&q[1]);
}

void quaternion_inv(quaternion q) {
	float nSq = norm4sq(q);
	q[0] /= nSq;
	scale3v(&q[1], -1.0 / nSq);
}

void quaternion_compose(vec3 dest, quaternion q, vec3 v) {
	quaternion tmp, vec, qInv;
	memcpy(qInv, q, sizeof(quaternion));
	quaternion_inv(qInv);
	vec[0] = 0;
	memcpy(&vec[1], v, sizeof(vec3));
	quaternion_mul(tmp, q, vec);
	quaternion_mul(vec, tmp, qInv);
	memcpy(dest, &vec[1], sizeof(vec3));
}

void quaternion_to_mat3(mat3 dest, quaternion q) {
	float mag = norm3(&q[1]);
	float angle = atan2(mag, q[0]);
	if (angle > M_PI / 2.0) {
		angle -= M_PI;
		mag = -mag;
	}
	if (mag) {
		load_rot3(dest, &q[1], 2.0 * angle);
	} else {
		vec3 axis = {1, 0, 0};
		load_rot3(dest, axis, 2.0 * angle);
	}
}

void quaternion_to_mat4(mat4 dest, quaternion q) {
	float mag = norm3(&q[1]);
	float angle = atan2(mag, q[0]);
	if (angle > M_PI / 2.0) {
		angle -= M_PI;
		mag = -mag;
	}
	if (mag) {
		load_rot4(dest, &q[1], 2.0 * angle);
	} else {
		vec3 axis = {1, 0, 0};
		load_rot4(dest, axis, 2.0 * angle);
	}
}

void quaternion_from_mat3(quaternion dest, mat3 src) {
	vec3 axis;
	axis[0] = src[1][2] - src[2][1];
	axis[1] = src[2][0] - src[0][2];
	axis[2] = src[0][1] - src[1][0];
	quaternion_set_axis_angle(dest, axis, atan2(norm3(axis) / 2.0, (src[0][0] + src[1][1] + src[2][2] - 1.0) / 2.0));
}

void quaternion_from_mat4(quaternion dest, mat4 src) {
	vec3 axis;
	axis[0] = src[1][2] - src[2][1];
	axis[1] = src[2][0] - src[0][2];
	axis[2] = src[0][1] - src[1][0];
	quaternion_set_axis_angle(dest, axis, atan2(norm3(axis) / 2.0, (src[0][0] + src[1][1] + src[2][2] - 1.0) / 2.0));
}

void lookat_from_axes(quaternion q, vec3 forward, vec3 up, vec3 side) {
	vec3 s, u, f;
	mat3 rot;

	//zero3v(f);
	//decr3v(f, forward);
	sub3v(f, (vec3){0}, forward);
	normalize3(f); // normalize(-forward)
	cross3(s, f, up);
	normalize3(s);
	cross3(u, f, s);
/*
m00 = x.x
m01 = y.x
m02 = z.x
m10 = x.y
m11 = y.y
m12 = z.y
m20 = x.z
m21 = y.z
m22 = z.z

m00 = s.x
m01 = u.x
m02 =-f.x
m10 = s.y
m11 = u.y
m12 =-f.y
m20 = s.z
m21 = u.z
m22 =-f.z
*/
	rot[0][0] = s[0];
	rot[0][1] = u[0];
	rot[0][2] =-f[0];
	rot[1][0] = s[1];
	rot[1][1] = u[1];
	rot[1][2] =-f[1];
	rot[2][0] = s[2];
	rot[2][1] = u[2];
	rot[2][2] =-f[2];
	/*
	dest[3][0] =-dot3(s, eye);
	dest[3][1] =-dot3(u, eye);
	dest[3][2] = dot3(f, eye);
	*/
	quaternion_from_rot3(q, rot);
}
/*
float quaternion_norm(quaternion q) {
	return w * w + x * x + y * y + z * z;
}
*/
void quaternion_to_rot3(mat3 dest, quaternion src) {
	float norm = norm4sq(src);
	float s = (norm > 0) ? 2.0 / norm : 0;
	float x = src[1];
	float y = src[2];
	float z = src[3];
	float w = src[0];
	// compute xs/ys/zs first to save 6 multiplications, since xs/ys/zs
	// will be used 2-4 times each.
	float xs = x * s;
	float ys = y * s;
	float zs = z * s;
	float xx = x * xs;
	float xy = x * ys;
	float xz = x * zs;
	float xw = w * xs;
	float yy = y * ys;
	float yz = y * zs;
	float yw = w * ys;
	float zz = z * zs;
	float zw = w * zs;

	// using s=2/norm (instead of 1/norm) saves 9 multiplications by 2 here
	dest[0][0] = 1 - (yy + zz);
	dest[0][1] = (xy - zw);
	dest[0][2] = (xz + yw);
	dest[1][0] = (xy + zw);
	dest[1][1] = 1 - (xx + zz);
	dest[1][2] = (yz - xw);
	dest[2][0] = (xz - yw);
	dest[2][1] = (yz + xw);
	dest[2][2] = 1 - (xx + yy);
}

void quaternion_from_rot3(quaternion q, mat3 m)
{
	float m00 = m[0][0];
	float m01 = m[0][1];
	float m02 = m[0][2];
	float m10 = m[1][0];
	float m11 = m[1][1];
	float m12 = m[1][2];
	float m20 = m[2][0];
	float m21 = m[2][1];
	float m22 = m[2][2];
        float lengthSquared = m00 * m00 + m10 * m10 + m20 * m20;
	float s, t;
	float x, y, z, w;

	if (lengthSquared != 1 && lengthSquared != 0) {
		lengthSquared = 1.0 / sqrt(lengthSquared);
		m00 *= lengthSquared;
		m10 *= lengthSquared;
		m20 *= lengthSquared;
	}
	lengthSquared = m01 * m01 + m11 * m11 + m21 * m21;
	if (lengthSquared != 1 && lengthSquared != 0) {
		lengthSquared = 1.0 / sqrt(lengthSquared);
		m01 *= lengthSquared;
		m11 *= lengthSquared;
		m21 *= lengthSquared;
	}
	lengthSquared = m02 * m02 + m12 * m12 + m22 * m22;
	if (lengthSquared != 1 && lengthSquared != 0) {
		lengthSquared = 1.0 / sqrt(lengthSquared);
		m02 *= lengthSquared;
		m12 *= lengthSquared;
		m22 *= lengthSquared;
	}

        t = m00 + m11 + m22;

        if (t >= 0) { // |w| >= .5
		s = sqrt(t + 1); // |s|>=1 ...
		w = 0.5 * s;
		s = 0.5 / s;                 // so this division isn't bad
		x = (m21 - m12) * s;
		y = (m02 - m20) * s;
		z = (m10 - m01) * s;
	} else if ((m00 > m11) && (m00 > m22)) {
		s = sqrt(1.0 + m00 - m11 - m22); // |s|>=1
		x = 0.5 * s; // |x| >= .5
		s = 0.5 / s;
		y = (m10 + m01) * s;
		z = (m02 + m20) * s;
		w = (m21 - m12) * s;
	} else if (m11 > m22) {
		s = sqrt(1.0 + m11 - m00 - m22); // |s|>=1
		y = 0.5 * s; // |y| >= .5
		s = 0.5 / s;
		x = (m10 + m01) * s;
		z = (m21 + m12) * s;
		w = (m02 - m20) * s;
	} else {
		s = sqrt(1.0 + m22 - m00 - m11); // |s|>=1
		z = 0.5 * s; // |z| >= .5
		s = 0.5 / s;
		x = (m02 + m20) * s;
		y = (m21 + m12) * s;
		w = (m10 - m01) * s;
	}

	q[0] = w;
	q[1] = x;
	q[2] = y;
	q[3] = z;
}

void quaternion_decompose_swing_twist(quaternion src, vec3 direction, quaternion swing, quaternion twist) {
	quaternion tmp;
	vec3 d;
	float norm;

	memcpy(d, direction, sizeof(vec3));
	normalize3(d);
	mul3sv(&twist[1], dot3(&src[1], d), d);
	twist[0] = src[0];
	if ((norm = norm4(twist)) != 0.0) {
		scale4v(twist, 1.0 / norm);
		tmp[0] = twist[0];
		tmp[1] = -twist[1];
		tmp[2] = -twist[2];
		tmp[3] = -twist[3];
		quaternion_mul(swing, src, tmp);
	} else {
		quaternion_load_id(twist);
		memcpy(swing, src, sizeof(quaternion));
	}
}

void quaternion_print(quaternion q) {
	vec3 tmp;
	memcpy(tmp, &q[1], sizeof(vec3));
	normalize3(tmp);
	printf("Angle: %f, Axis: (%f, %f, %f)\n", 2 * acos(q[0]), tmp[0], tmp[1], tmp[2]);
}
