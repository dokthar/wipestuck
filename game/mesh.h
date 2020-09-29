#ifndef MESH_H
#define MESH_H

#define MESH_ATTRIB_POSITION 0
#define MESH_ATTRIB_NORMAL   1
#define MESH_ATTRIB_TEXCOORD 2
#define MESH_MAX_VBO 4

struct mesh {
	GLuint vao;
	GLuint vbo[MESH_MAX_VBO];
	int vbo_count;
	int idx_positions;
	int idx_normals;
	int idx_texcoords;
	int idx_indices;
	size_t vertex_count;
	size_t index_count;
	GLenum primitive;
};

void mesh_load(struct mesh *m, size_t count, float *positions, float *normals, float *texcoords);
void mesh_bind(struct mesh *m, GLint position, GLint normal, GLint texture);
void mesh_free(struct mesh* m);
void mesh_load_box(struct mesh *m, float x, float y, float z);
void mesh_load_quad(struct mesh *m, float x, float y);

#endif
