#ifndef SCENE_H
#define SCENE_H

// System includes
#ifdef __APPLE__
    #include <OpenGL/gl3.h>
#endif
#include "GL_utilities.h"
#include "LittleOBJLoader.h"
#include <assimp/Importer.hpp>
#include "LoadTGA.h"  // Add this for LoadTGATextureSimple
#include "lodepng.h"  // Add this for lodepng::decode


// Projection parameters
#define near 1.0
#define far 500.0
#define right 0.5
#define left (-0.5)
#define top 0.5
#define bottom (-0.5)
#define GROUND_SIZE 1000.0f
#define WINDOW_HEIGHT 1080.0f
#define WINDOW_WIDTH 1920.0f


#define TEX_UNIT 0
#define MOON_TEX_UNIT 1
#define BLOOM_TEX_UNIT 16

inline mat4 projectionMatrix = perspective(45.0f, WINDOW_WIDTH/WINDOW_HEIGHT, near, far);

inline mat4 modelViewMatrix;
inline mat4 shadowProjectionMatrixFire;


// Vertex data
inline vec3 vertices[] = {
    vec3(-GROUND_SIZE, 0.0f, -GROUND_SIZE),
    vec3(-GROUND_SIZE, 0.0f, GROUND_SIZE),
    vec3(GROUND_SIZE, -0.0f, -GROUND_SIZE),
    vec3(GROUND_SIZE, -0.0f, GROUND_SIZE)
};
inline vec3 vertex_normals[] = {
    vec3(0.0f, 1.0f, 0.0f),
    vec3(0.0f, 1.0f, 0.0f),
    vec3(0.0f, 1.0f, 0.0f),
    vec3(0.0f, 1.0f, 0.0f)
};
inline vec2 tex_coords[] = {
    vec2(0.0f, 0.0f),
    vec2(0.0f, 20.0f),
    vec2(20.0f, 0.0f),
    vec2(20.0f, 20.0f)
};
inline mat4 zeroedge = {
    1, 1, 1, 0,
    1, 1, 1, 0,
    1, 1, 1, 0,
    1, 1, 1, 1
};
inline GLuint indices[] = { 0, 1, 2, 1, 3, 2 };

// Camera variables
extern vec3 worldCameraP;
extern vec3 worldCameraL;
extern vec3 worldCameraV;
extern mat4 worldCamera;

// Textures
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

extern void drawObjects(GLuint shader);

// Transformation matrices
extern mat4 totalGround;
extern mat4 cabinT;
extern mat4 FireplaceT;
extern mat4 tableT;
extern mat4 sofaT;
extern mat4 fireT;
extern mat4 fireT2;
extern mat4 logT;
extern mat4 wolfT;
extern mat4 doorT;

// Fire
extern vec3 firePos;
extern vec3 fireColor;
extern vec3 fire_start_pos;
extern vec3 moonPos;
extern vec3 moonColor;

// Shaders
extern GLuint shybox_shader;
extern GLuint object_shader;
extern GLuint shadow_shader;
extern GLuint tree_shader;
extern GLuint overflow_shader;
extern GLuint lowpass_shader;
extern GLuint bloom_shader;

// FBOs
extern FBOstruct *fireFbo, *moonFbo, *bloomFbo, *overFlowFbo, *tempFbo;

// Time
extern GLfloat t;

// Mouse control
extern bool mouseCaptured;

// Function declarations
void InstantiateModels();
void InstantiateTextures();
void OnTimer(int value);
void init();
void moveCamera();
void DrawCabin();
void DrawFireplace();
void DrawTable();
void DrawSofa();
void DrawSkyBox();
void DrawGround();
void DrawTree();
void DrawFire();
void DrawWolf();
void DrawDoor();
void DrawLog();
void display();

#endif // SCENE_H