#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>

#include "engine.h"

#ifndef NO_AUDIO
#define STB_VORBIS_HEADER_ONLY
#include "miniaudio/stb_vorbis.c"    /* Enables Vorbis decoding. */
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio/miniaudio.h"
/* stb_vorbis implementation must come after the implementation of miniaudio. */
#undef STB_VORBIS_HEADER_ONLY
#include "miniaudio/stb_vorbis.c"

void init_audio(void);
void fini_audio(void);
#else
void init_audio(void) {}
void fini_audio(void) {}
#endif

char *app_name;
unsigned int width = 1080;
unsigned int height = 800;

struct camera cam;
int flycam;
double xpre, ypre;
int xray = 0;
float position = 0;
float speed = 1.5;
int show = 0;

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
	} else {
		position += speed * (dx / (float)width);
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
	case 'Q':
	case 'C':
		if (action == GLFW_PRESS && mods & GLFW_MOD_CONTROL)
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		break;
	case 'F':
		if (action == GLFW_PRESS)
			show = !show;
		break;
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, GLFW_TRUE);
		break;
	case 'Z':
		if (action == GLFW_PRESS) {
			flycam = !flycam;
			if (flycam) {
				quaternion qid;
				quaternion_load_id(qid);
				camera_set_rotation(&cam, qid);
			}
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
			if (flycam) camera_move(&cam, left);
		break;
	case 'D':
	case GLFW_KEY_RIGHT:
		neg3v(left);
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
			if (flycam) camera_move(&cam, left);
		break;
	case 'W':
	case GLFW_KEY_UP:
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
			if (flycam) camera_move(&cam, dir);
		break;
	case 'S':
	case GLFW_KEY_DOWN:
		neg3v(dir);
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
			if (flycam) camera_move(&cam, dir);
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
	"uniform float time;\n"
	"void main(void)\n"
	"{\n"
//	"	gl_Position = vec4(in_pos.x, in_pos.y, in_pos.z, 1.0);\n"
	"	gl_Position = proj * view * model * (vec4(in_pos, 1.0));"
//	"	gl_Position = proj * view * vec4(in_pos, 1.0);"
	"	texcoord = in_texcoord;\n"
	"	normal = in_normal;\n"
	"}\n";

const char *frag =
	"#version 410 core\n"
	"in vec3 normal;\n"
	"in vec2 texcoord;\n"
	"out vec4 out_color;\n"
	"uniform float time;\n"
	"uniform vec3 look;\n"
	"void main(void)\n"
	"#define M_PI 3.14159\n"
	"{\n"
	"	vec2 uv = texcoord + vec2(0*time/ M_PI, 0);\n"
//	"	vec2 uv = mix(texcoord * 2, 2-texcoord * 2, step(0.5, texcoord));\n"
	"\n"
	"uv *= 20;\n"
	"float a = time/20;\n"
	"uv *= mat2(cos(a), sin(a), -sin(a), cos(a));\n"
//	"	vec3 col = vec3(step(0.5, fract(uv.rrr)));\n"
	"	vec3 col = vec3(0);\n"
	"	float bpm = 127 / 60.0;\n"
	"	float wash = mix(0.5, 0.75, fract(bpm*time));\n"
	"\n"
	"	uv /= vec2(1, 10);\n"
	"	uv.x += fract(uv.y*5);\n"
	"	uv = fract(uv) - 0.5;\n"
	"	col.r = 1 - step(mix(0.1, 0.4, 0.5+0.5*sin(time/10)), length(uv));\n"
//	"	col.r = max(col.r, 1.5*dot(-normal, look));\n"
	"	if (step(0.1, dot(normal, look))>0)\n"
	"	col.r = 1 - col.r;\n"
	"	out_color = vec4(col.rrr,0);\n"
	"}\n";


const char *solid_frag =
	"#version 410 core\n"
	"in vec3 normal;\n"
	"in vec2 texcoord;\n"
	"out vec4 out_color;\n"
	"uniform float time;\n"
	"uniform vec3 color;\n"
	"#define M_PI 3.14159\n"
	"void main(void)\n"
	"{\n"
	"	out_color = vec4(color,0);\n"
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
	"	gl_Position = proj * view * model * vec4(in_pos, 1.0);"
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
	"uniform mat4 model;\n"
	"void GenerateLine(int index) {\n"
	"	vec4 norm = proj * view * model * vec4(normal[index], 0.0) * MAGNITUDE;\n"
	"	gl_Position = gl_in[index].gl_Position;\n"
	"	color = normalize(normal[index]);\n"
	"	EmitVertex();\n"
	"	gl_Position = gl_in[index].gl_Position + norm;\n"
	"	color = normalize(normal[index]);\n"
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

static void render_box(struct camera *c, struct shader *s, struct mesh *m, mat4 model, vec3 color);
void camera_look_at(struct camera* c, vec3 look_at, vec3 up);

void rotation_between_vec3(quaternion q, vec3 start, vec3 dest, vec3 up);

vec3 cam_pos;
vec3 cam_look;
vec3 cam_norm;
vec3 player_pos;
quaternion player_dir;
float radius = 50;
float cr = 0.5;

struct bomb {
	vec3 pos;
	vec3 dir;
	mat4 model;
	int state;
	float t;
};
struct bomb bombs[10];

void
spawn_bomb(struct bomb *bomb, float cr, float rr)
{
	float a = 2 * M_PI * (rand() / (float)RAND_MAX);
	float b = 2 * M_PI * (rand() / (float)RAND_MAX);
	float x, y, z;
	quaternion q1, q2, q3;
	
	x = cos(b) * cr * cos(b);
	y = cr * sin(b);
	z = sin(b) * cr * cos(b);
	bomb->dir[0] = x;
	bomb->dir[1] = y;
	bomb->dir[2] = z;
	normalize3(bomb->dir);

	x += rr * cos(a);
	y += 0;
	z += rr * sin(a);

	bomb->pos[0] = x;
	bomb->pos[1] = y;
	bomb->pos[2] = z;

	load_id4(bomb->model);
	quaternion_set_axis_angle(q1, (vec3){0,1,0}, a);
	quaternion_set_axis_angle(q2, (vec3){1,0,0}, b);
	quaternion_mul(q3, q2, q1);
	quaternion_to_mat4(bomb->model, q3);
	bomb->model[3][0] = x;
	bomb->model[3][1] = y;
	bomb->model[3][2] = z;
}

void
game_update(void)
{
	float time, aheadtime;
	float lookahead = 0.1;
	float surfdist = 0.85;
	vec3 cc, cp, cn;
	float dist = surfdist - lookahead;
	vec3 dir;

	time = 0.5*glfwGetTime();

	aheadtime = time + lookahead;
	cc[0] = radius * cos(aheadtime);
	cc[1] = 0;
	cc[2] = radius * sin(aheadtime);
	cp[0] = cos(aheadtime) * cos(position) * cr * dist;
	cp[1] = sin(position) * cr * dist;
	cp[2] = sin(aheadtime) * cos(position) * cr * dist;
	sub3v(cam_look, cc, cp);

	cc[0] = radius * cos(time);
	cc[1] = 0;
	cc[2] = radius * sin(time);
	cp[0] = cos(time) * cos(position) * cr * surfdist;
	cp[1] = sin(position) * cr * surfdist;
	cp[2] = sin(time) * cos(position) * cr * surfdist;
	sub3v(cam_pos, cc, cp);

	add3v(cam_norm, cp, (vec3){0,0,0});
	normalize3(cam_norm);

	aheadtime = time + lookahead * 0.025;
	surfdist = 1-lookahead;
	cc[0] = radius * cos(aheadtime);
	cc[1] = 0;
	cc[2] = radius * sin(aheadtime);
	cp[0] = cos(aheadtime) * cos(position) * cr * surfdist;
	cp[1] = sin(position) * cr * surfdist;
	cp[2] = sin(aheadtime) * cos(position) * cr * surfdist;
	sub3v(player_pos, cc, cp);
	sub3v(dir, cam_look, player_pos);
	quaternion_look_at(player_dir, dir, cam_norm);

	if (!flycam) {
		quaternion q;

		sub3v(dir, cam_look, cam.position);
		quaternion_look_at(q, dir, cam_norm);
		camera_set_position(&cam, cam_pos);
		camera_set_rotation(&cam, q);
	}
}

static void
rate_limit(int rate)
{
	static double tlast;
	double tnext, tcurr;
	struct timespec ts;
	double period = 1 / (double) rate;

	tnext = tlast + period;
	tcurr = glfwGetTime() + 0.0001;
	if (tcurr < tnext) {
		ts.tv_sec = 0;
		ts.tv_nsec = 1000000000 * (tnext - tcurr);
		nanosleep(&ts, NULL);
	}
	tlast = glfwGetTime();
}

int
main(int argc, char **argv)
{
	struct shader shad;
	struct shader normal_shad;
	struct shader solid_shad;
	struct mesh torus;
	struct mesh box1, box2;
	mat4 rot_id;
	GLint ret;
	unsigned int i;
	static unsigned int bombs_seed = 0xf00;
	srand(bombs_seed);

	/* Game init */
	app_name = argv[0];

	init_engine();
	init_audio();

	ret = shader_load(&shad, vert, frag, NULL);
	ret = shader_load(&solid_shad, vert, solid_frag, NULL);
	ret = shader_load(&normal_shad, normal_vert, normal_frag, normal_geom);

	mesh_load_torus(&torus, cr, -radius, 30, 2000);
	mesh_load_box(&box1, 0.1, 0.2, 0.1);
	mesh_load_box(&box2, 0.0025, 0.0025, 0.015);

	camera_set(&cam, 1.05, (float)width / (float)height);
	camera_set_position(&cam, (vec3){ 0, 0, 0});
	camera_set_rotation(&cam, (quaternion){0, 0, 0, 1});

	for (i = 0; i < ARRAY_LEN(bombs); i++) {
		spawn_bomb(&bombs[i], cr, radius);
		bombs[i].state = 1;
	}
	load_id4(rot_id);


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	while (!engine_shutdown()) {
		rate_limit(100);
		engine_swap(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		engine_poll();

		game_update();

		render_box(&cam, &shad, &torus, rot_id, NULL);
		if (xray)
			render_box(&cam, &normal_shad, &torus, rot_id, NULL);

		for (i = 0; i < ARRAY_LEN(bombs); i++) {
			if (bombs[i].state != 0 && show) {
				render_box(&cam, &solid_shad, &box1, bombs[i].model, (vec3){1,0,0});
			}
		}
		mat4 player_model;
		quaternion_to_mat4(player_model, player_dir);
		player_model[3][0] = player_pos[0];
		player_model[3][1] = player_pos[1];
		player_model[3][2] = player_pos[2];

		if (show)
			render_box(&cam, &solid_shad, &box2, player_model, (vec3){0,0,1});
//		render_box(&cam, &shad, &box1, cam_look);
//		render_box(&cam, &shad, &box1, cam_pos);
	}

	shader_free(&shad);
	shader_free(&normal_shad);
	fini_audio();
	fini_engine();

	return 0;
}

static void
camera_bind(struct shader *s, struct camera *c)
{
	GLint proj, view;

	proj = glGetUniformLocation(s->prog, "proj");
	view = glGetUniformLocation(s->prog, "view");

	camera_bind_proj(c, s->prog, proj, (float)width / (float)height);
	camera_bind_view(c, s->prog, view);
}

static void
render_box(struct camera *c, struct shader *s, struct mesh *m, mat4 model, vec3 color)
{
	GLint model_loc;
	GLint time = glGetUniformLocation(s->prog, "time");
	GLint look = glGetUniformLocation(s->prog, "look");
	GLint col = glGetUniformLocation(s->prog, "color");

	glUseProgram(s->prog);

	if (time >= 0)
		glUniform1f(time, glfwGetTime());

	if (look >= 0) {
		vec3 left, dir;
		camera_get_left(&cam, left);
		cross3(dir, left, cam_norm);
		glUniform3f(look, dir[0], dir[1], dir[2]);
	}

	if (col >= 0 && color) {
		glUniform3f(col, color[0], color[1], color[2]);
	}

	camera_bind(s, c);

	model_loc = glGetUniformLocation(s->prog, "model");
	if (model_loc >= 0 && model)
		glUniformMatrix4fv(model_loc, 1, GL_FALSE, (float *)model);

	if (xray)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	engine_render(c, s, m);
}

#ifndef NO_AUDIO
/* Miniaudio device callback */
void
audio_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
	ma_bool32 isLooping = MA_TRUE;
	ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;

	if (pDecoder == NULL) {
		return;
	}

	/*
	  A decoder is a data source which means you can seemlessly plug it into the ma_data_source API. We can therefore take advantage
	  of the "loop" parameter of ma_data_source_read_pcm_frames() to handle looping for us.
	*/
	ma_data_source_read_pcm_frames(pDecoder, pOutput, frameCount, NULL, isLooping);

	(void)pInput;
}

ma_result audio_ma_result;
ma_decoder audio_ma_decoder;
ma_device audio_ma_device;

void
fini_audio(void)
{
	if (audio_ma_result == MA_SUCCESS) {
		ma_device_uninit(&audio_ma_device);
		ma_decoder_uninit(&audio_ma_decoder);
	}
}

void
init_audio(void)
{
	ma_device_config deviceConfig;

	/* Audio playback init */
	audio_ma_result = ma_decoder_init_file("./audio/Stuck.ogg", NULL, &audio_ma_decoder);
	if (audio_ma_result != MA_SUCCESS) {
		printf("Failed to open audio file\n");
	} else {
		deviceConfig = ma_device_config_init(ma_device_type_playback);
		deviceConfig.playback.format   = audio_ma_decoder.outputFormat;
		deviceConfig.playback.channels = audio_ma_decoder.outputChannels;
		deviceConfig.sampleRate        = audio_ma_decoder.outputSampleRate;
		deviceConfig.dataCallback      = audio_callback;
		deviceConfig.pUserData         = &audio_ma_decoder;

		if (ma_device_init(NULL, &deviceConfig, &audio_ma_device) != MA_SUCCESS) {
			printf("Failed to open playback device.\n");
			ma_decoder_uninit(&audio_ma_decoder);
			return;
		}

		if (ma_device_start(&audio_ma_device) != MA_SUCCESS) {
			printf("Failed to start playback device.\n");
			fini_audio();
			return;
		}
	}
}
#endif
