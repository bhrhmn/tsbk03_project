#include "VectorUtils4.h"

#define TREE_SIZE 50.0f
#define start TREE_SIZE/-2
#define ground_offset 10.0f

vec3 treeMiddle = vec3(start+(TREE_SIZE/2), 0.0f-ground_offset, 0.0f);

mat4 treeT = T(start, 0.0f, 0.0f);

vec3 tree_vertices[] = {
    vec3(start,             0.0f-ground_offset,      0.0f),
    vec3(start+TREE_SIZE,   0.0f-ground_offset,      0.0f),
    vec3(start,             TREE_SIZE-ground_offset, 0.0f),
    vec3(start+TREE_SIZE,   TREE_SIZE-ground_offset, 0.0f),
};

vec3 tree_vertex_normals[] = {
    vec3(0.0f, 0.0f, 1.0f),
    vec3(0.0f, 0.0f, 1.0f),
    vec3(0.0f, 0.0f, 1.0f),
    vec3(0.0f, 0.0f, 1.0f)
};

vec2 tree_tex_coords[] = {
    vec2(0.0f, 0.0f),
    vec2(1.0f, 0.0f), 
    vec2(0.0f, 1.0f),
    vec2(1.0f, 1.0f)
};

GLuint tree_indices[] = { 1, 0, 2, 1, 2, 3 };

