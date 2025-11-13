#ifndef TREE_H
#define TREE_H

#include "scene.h"

#define TREE_SIZE 50.0f
#define start TREE_SIZE/-2
#define ground_offset 10.0f

// DECLARATIONS only (use extern)
extern vec3 treeMiddle;
extern mat4 treeT;
extern vec3 tree_vertices[4];
extern vec3 tree_vertex_normals[4];
extern vec2 tree_tex_coords[4];
extern GLuint tree_indices[6];

#endif // TREE_H