#ifndef SCENE_H
#define SCENE_H

// System includes
#ifdef __APPLE__
    #include <OpenGL/gl3.h>
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "math.h"
#define MAIN
#include "LittleOBJLoader.h"
#include "LoadTGA.h"
#include "VectorUtils4.h"
#include "lodepng.h"

// Projection parameters
#define near 1.0
#define far 500.0
#define right 0.5
#define left -0.5
#define top 0.5
#define bottom -0.5
#define GROUND_SIZE 1000.0f
#define WINDOW_SIZE 800.0f

// We assign one texture unit in which to store the transformation.
#define TEX_UNIT 0
#define MOON_TEX_UNIT 1



const GLfloat projectionMatrix[] = {2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,
                                    0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,
                                    0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),
                                    0.0f, 0.0f, -1.0f, 0.0f };




// Vertex data for ground
vec3 vertices[] = {
    vec3(-GROUND_SIZE, 0.0f, -GROUND_SIZE),
    vec3(-GROUND_SIZE, 0.0f, GROUND_SIZE),
    vec3(GROUND_SIZE, -0.0f, -GROUND_SIZE),
    vec3(GROUND_SIZE, -0.0f, GROUND_SIZE)
};

vec3 vertex_normals[] = {
    vec3(0.0f, 1.0f, 0.0f),
    vec3(0.0f, 1.0f, 0.0f),
    vec3(0.0f, 1.0f, 0.0f),
    vec3(0.0f, 1.0f, 0.0f)
};

vec2 tex_coords[] = {
    vec2(0.0f, 0.0f),
    vec2(0.0f, 20.0f),
    vec2(20.0f, 0.0f), 
    vec2(20.0f, 20.0f)
};

mat4 zeroedge = { 
    1, 1, 1, 0,
    1, 1, 1, 0, 
    1, 1, 1, 0, 
    1, 1, 1, 1
};
GLuint indices[] = { 0, 1, 2, 1, 3, 2 };
                                    
// Models
extern Model *ground;
extern Model *skybox;
extern Model *sofa;
extern Model *table;
extern Model *cabin;
extern Model *fireplace;

// Textures
extern unsigned int myTex;
extern unsigned int myTex2;
extern unsigned int cabintex;

// Transformation matrices
extern mat4 totalGround;
extern mat4 worldCamera;
extern mat4 zeroedge;
extern mat4 cabinT;
extern mat4 FireplaceT;
extern mat4 tableT;
extern mat4 sofaT;

// Camera
extern vec3 worldCameraP;
extern vec3 worldCameraL;
extern vec3 worldCameraV;

// Shaders
extern GLuint ground_shader;
extern GLuint shybox_shader;
extern GLuint object_shader;

// Fire
extern vec3 firePos;
extern vec3 fireColor;

// Time
extern GLfloat t;

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
void display();

#endif // SCENE_H