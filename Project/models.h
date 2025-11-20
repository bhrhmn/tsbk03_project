#ifndef MODELS_H
#define MODELS_H

#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"

// Model pointers
extern Model *ground, *skybox, *sofa, *table, *cabin, *fireplace, *newCabin, *roof, *floor3;
extern Model *treeBillboard, *tree_log, *door, *squareModel;

// Transformation matrices
extern mat4 totalGround, cabinT, FireplaceT, tableT, sofaT, newCabinT, floorT, roofT;
extern mat4 fireT, fireT2, logT, wolfT, doorT;


// textures

extern unsigned int myTex;
extern unsigned int myTex2;
extern unsigned int cabintex;
extern unsigned int sofatex;
extern unsigned int fireplacetex;
extern unsigned int maskrosTex;
extern unsigned int treeTex;
extern unsigned int fireTex;
extern unsigned int fire2Tex;
extern unsigned int logTex;
extern unsigned int wolfTex;
extern unsigned int doorTex;
extern unsigned int newCabinTex;
extern unsigned int doorTex;
extern unsigned int roofTex;
extern const int FOREST_SIZE;
extern mat4 treeMat[12]; // FOREST_SIZE = 12

// Tree data (if these are used elsewhere)
extern vec3 tree_vertices[4];
extern vec3 tree_vertex_normals[4];
extern vec2 tree_tex_coords[4];
extern GLuint tree_indices[6];

// Model functions
void InstantiateModels();
void InstatiateTextures();

#endif