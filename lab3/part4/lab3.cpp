#define MAIN

#include "GL_utilities.h"
#include "MicroGlut.h"
#include "LittleOBJLoader.h" 
#include "LoadTGA.h" 
#include "VectorUtils4.h" 
#include <math.h>
#include <stdio.h>


//WARN: DO NOT define before include section 
#define near 1.0
#define far 160.0
#define right 0.7
#define left -0.7
#define top 0.5
#define bottom -0.5
#define playerSpeed 0.7
#define WINDOW_SIZE 600
#define kGroundSize 100.0f


//function prototypes
void input();
void updateCamera();
void updateFocus(int, int);

vec3 cameraPos = {-4, 10, -40};
vec3 lookingDir = {0, 0, 1};
vec3 focusPoint = {-4, 10, -39};
vec3 upDir = {0, 1, 0};
vec2 lastMousePos = {WINDOW_SIZE / 2, WINDOW_SIZE /2};

mat4 camera; 

GLfloat projectionMatrix[] = {    
	2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,
  0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,
  0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),
  0.0f, 0.0f, -1.0f, 0.0f 
};


// Refrence to models, initialized in init()
Model *walls;
Model *roof;
Model *balcony;
Model *wing;
Model *martin;
Model *ground;
Model *skybox;
Model *teapot;

// Reference to shader program
GLuint program;

// Refrence to texture
GLuint wallTex;
GLuint martinTex;
GLuint grassTex;
GLuint skyTex;

// vertex array object
unsigned int modelsVertexArrayObjID;

GLfloat specularExponent[] = {100.0, 200.0, 60.0};


void init(void)
{
	glutPassiveMotionFunc(*updateFocus);
	updateCamera();

	// set to rerender in ~60FPS
	glutRepeatingTimer(16);
	
	dumpInfo();

	// GL inits
	glClearColor(0.9, 0.3, 0.4 ,0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab3.vert", "lab3.frag");
	
	//NOTE: always do this after loadShaders
	glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_TRUE, projectionMatrix);

	printError("init shader");

	// Load & Activate texture
	LoadTGATextureSimple("../conc.tga", &wallTex);
	LoadTGATextureSimple("../marin.tga", &martinTex);
	LoadTGATextureSimple("../grass.tga", &grassTex);
	LoadTGATextureSimple("../skybox/SkyBoxFull.tga", &skyTex);
	
	walls 	= LoadModel("../windmill/windmill-walls2.obj");
	roof  	= LoadModel("../windmill/windmill-roof.obj");
	balcony =	LoadModel("../windmill/windmill-balcony.obj");
	wing  	=	LoadModel("../windmill/blade.obj");
	martin  = LoadModel("../marin.obj");
	skybox  = LoadModel("../skybox/skyboxfull.obj");
	teapot 	= LoadModel("../models/various/teapot.obj");
	
	vec3 vertices[] =
	{
	 vec3(-kGroundSize,0.0f,-kGroundSize),
	 vec3(-kGroundSize,0.0f,kGroundSize),
	 vec3(kGroundSize,-0.0f,-kGroundSize),
	 vec3(kGroundSize,-0.0f,kGroundSize)
	};
	
	vec3 vertex_normals[] =
	{
	  vec3(0.0f,1.0f,0.0f),
	  vec3(0.0f,1.0f,0.0f),
	  vec3(0.0f,1.0f,0.0f),
	  vec3(0.0f,1.0f,0.0f)
	};
	
	vec2 tex_coords[] =
	{
	  vec2(0.0f,0.0f),
	  vec2(0.0f,20.0f),
	  vec2(20.0f,0.0f), 
	  vec2(20.0f,20.0f)
	};
	GLuint indices[] =
	{
	  0, 1, 2, 1, 3, 2
	};

	ground = LoadDataToModel(
		vertices,
		vertex_normals,
		tex_coords,
		nullptr,
		indices,
		sizeof(vertices),
		sizeof(indices)
	);

	vec3 lightSourcesColorsArr[] = 
		{ 
			vec3(1.0f, 0.0f, 0.0f), // Red light
      vec3(0.0f, 1.0f, 0.0f), // Green light
      vec3(0.0f, 0.0f, 1.0f), // Blue light
      vec3(1.0f, 1.0f, 1.0f)  // White light
		}; 

	GLint isDirectional[] = {0,0,1,1};
	GLfloat specularExponent[] = {100.0, 200.0, 60.0};

	vec3 lightSourcesDirectionsPositions[] = 
		{ 
			vec3(10.0f, 5.0f, 0.0f), // Red light, positional
      vec3(0.0f, 5.0f, 10.0f), // Green light, positional
      vec3(-1.0f, 0.0f, 0.0f), // Blue light along X
      vec3(0.0f, 0.0f, -1.0f)  // White light along Z
		};

	glUniform3fv(glGetUniformLocation(program, "lightSourcesDirPosArr"), 4, &lightSourcesDirectionsPositions[0].x);
	glUniform3fv(glGetUniformLocation(program, "lightSourcesColorArr"), 4, &lightSourcesColorsArr[0].x);
	glUniform1iv(glGetUniformLocation(program, "isDirectional"), 4, isDirectional);
	
	printError("init arrays");
}


void input()
{
	vec3 dir = normalize(focusPoint - cameraPos);
	vec3 side_dir = normalize(cross(upDir, dir));
	
	if (glutKeyIsDown('w')) {
		cameraPos += dir * playerSpeed;
	}
	if (glutKeyIsDown('a')) {
		cameraPos += side_dir * playerSpeed;
	};
	if (glutKeyIsDown('s')) {
		cameraPos -= dir * playerSpeed;
	} 
	if (glutKeyIsDown('d')) {
		cameraPos -= side_dir * playerSpeed;
	};
	//LEFT_SHIFT
	if (glutKeyIsDown(GLUT_KEY_LEFT_SHIFT)) {
		cameraPos -= upDir * playerSpeed;
	};
	//SPACEBAR
	if (glutKeyIsDown(' ')) {
		cameraPos += upDir * playerSpeed;
	};
	focusPoint = cameraPos + lookingDir;
}


void updateFocus(int x, int y)
{
	vec2 mouseDelta = (vec2){
		lastMousePos.x - x, 
		lastMousePos.y - y
	};
	
	GLfloat theta_x = 0.0f;
	GLfloat theta_y = 0.0f;
	GLfloat sensitivity = 0.0005f;
	int threshhold = 1;

	if (abs(mouseDelta.x) > threshhold) {
		theta_x = mouseDelta.x * sensitivity;
	}
	
	if (abs(mouseDelta.y) > threshhold) {
		theta_y = mouseDelta.y * sensitivity;
	}
			

	vec3 side_dir = cross(lookingDir, upDir);
	lookingDir = normalize(ArbRotate(upDir, theta_x) * ArbRotate(side_dir, theta_y) * lookingDir);
	
	focusPoint = cameraPos + lookingDir;

	glutWarpPointer(WINDOW_SIZE / 2, WINDOW_SIZE / 2);
	lastMousePos = (vec2){WINDOW_SIZE / 2, WINDOW_SIZE / 2};
}


void updateCamera()
{
	camera = lookAtv(
		cameraPos,
		focusPoint,
		upDir
	);
	glUniformMatrix4fv(glGetUniformLocation(program, "worldToView"), 1, GL_TRUE, camera.m);
}


void display(void)
{
	printError("pre display");

	glutHideCursor();
	input();
	updateCamera();

	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	GLfloat theta = t/600;

	// Model to world matrices
	
	// SKYBOX
	mat4 skyMat = mat3tomat4(mat4tomat3(camera)) * S(10);

	// GROUND 
	mat4 groundMtW = T(0,0,0);

	// WALLS
	mat4 trans = T(0, 0, 0);
	mat4 rot = Ry(theta);
	mat4 wallsMtW = rot * trans; 
	
	// ROOF
	mat4 trans2 = T(0, 0.30, 0);
	mat4 roofMtW = rot * trans2;
	
	// BALCONY
	mat4 trans3 = T(0.2, 0, 0);
	mat4 balconyMtW = rot * trans3; 

	// WINGS
	mat4 trans4 = T(4.9, 9.25, 0);
	mat4 rot4 	= Rx(theta) * Rx(M_PI / 2);
	mat4 rot4_1 = Rx(theta) * Rx(M_PI);
	mat4 rot4_2 = Rx(theta) * Rx(-M_PI / 2);
	mat4 rot4_3 = Rx(theta) * Rx(2 * M_PI);
	mat4 wingMtW1 = rot * trans4 * rot4;
	mat4 wingMtW2 = rot * trans4 * rot4_1;
	mat4 wingMtW3 = rot * trans4 * rot4_2;
	mat4 wingMtW4 = rot * trans4 * rot4_3;
		
	// TEAPOT
	mat4 teapotMtW = T(20, 0, 20);

	// MARTIN
	mat4 scaleMartin = S(2.2);
	mat4 matTrans = T(-3,4.7,-2.4);
	mat4 matMtW = rot * matTrans * Ry(-M_PI / 2) * scaleMartin;
	
	mat4 matMtW2 = T(27.1, 5.5, 20) * S(2.05 + cos(theta) * 2) * Ry(M_PI / 2) * Rx(M_PI/6) * Ry(theta * 2) * T(0,cos(theta)/2 + 0.5,0);

	mat4 bigMartinMtw = T(40, 5, 40) * S(20);
	
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(modelsVertexArrayObjID);    // Select VAO
	
	// DRAW SKYBOX
	// NOTE: important to draw this first
	glUniform1i(glGetUniformLocation(program, "isSky"), 1);
	glUniformMatrix4fv(glGetUniformLocation(program, "worldToView"), 1, GL_TRUE, IdentityMatrix().m);
	
	glBindTexture(GL_TEXTURE_2D, skyTex);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	
	glUniformMatrix4fv(glGetUniformLocation(program, "ModelToWorld"), 1, GL_TRUE, skyMat.m);
	DrawModel(skybox, program, "in_Position", "in_Normal", "inTexCoord");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
	// NOTE: must be "turned off" for skybox
	glUniformMatrix4fv(glGetUniformLocation(program, "worldToView"), 1, GL_TRUE, camera.m);
	glUniform1i(glGetUniformLocation(program, "isSky"), 0);
	
	// DRAW GROUND
	glUniform1f(glGetUniformLocation(program, "specularExponent"), specularExponent[0]);
	glBindTexture(GL_TEXTURE_2D, grassTex);
	glUniformMatrix4fv(glGetUniformLocation(program, "ModelToWorld"), 1, GL_TRUE, groundMtW.m);
	DrawModel(ground, program, "in_Position", "in_Normal", "inTexCoord");

	// DRAW WINDMILL 
	glUniform1f(glGetUniformLocation(program, "specularExponent"), specularExponent[1]);
	glBindTexture(GL_TEXTURE_2D, wallTex);
	glUniformMatrix4fv(glGetUniformLocation(program, "ModelToWorld"), 1, GL_TRUE, wallsMtW.m);
	DrawModel(walls, program, "in_Position", "in_Normal", "inTexCoord");
	
	glUniformMatrix4fv(glGetUniformLocation(program, "ModelToWorld"), 1, GL_TRUE, roofMtW.m);
	DrawModel(roof, program, "in_Position", "in_Normal", "inTexCoord");
	
	glUniformMatrix4fv(glGetUniformLocation(program, "ModelToWorld"), 1, GL_TRUE, balconyMtW.m);
	DrawModel(balcony, program, "in_Position", "in_Normal", "inTexCoord");

	glUniformMatrix4fv(glGetUniformLocation(program, "ModelToWorld"), 1, GL_TRUE, wingMtW1.m);
	DrawModel(wing, program, "in_Position", "in_Normal", "inTexCoord");

	glUniformMatrix4fv(glGetUniformLocation(program, "ModelToWorld"), 1, GL_TRUE, wingMtW2.m);
	DrawModel(wing, program, "in_Position", "in_Normal", "inTexCoord");

	glUniformMatrix4fv(glGetUniformLocation(program, "ModelToWorld"), 1, GL_TRUE, wingMtW3.m);
	DrawModel(wing, program, "in_Position", "in_Normal", "inTexCoord");

	glUniformMatrix4fv(glGetUniformLocation(program, "ModelToWorld"), 1, GL_TRUE, wingMtW4.m);
	DrawModel(wing, program, "in_Position", "in_Normal", "inTexCoord");
		
	// DRAW TEAPOT
	glUniform1f(glGetUniformLocation(program, "specularExponent"), specularExponent[2]);
	glUniformMatrix4fv(glGetUniformLocation(program, "ModelToWorld"), 1, GL_TRUE, teapotMtW.m);
	DrawModel(teapot, program, "in_Position", "in_Normal", "inTexCoord");

	// DRAW MARTIN
	glUniform1i(glGetUniformLocation(program, "isSky"), 2);
	glBindTexture(GL_TEXTURE_2D, martinTex);
	glUniformMatrix4fv(glGetUniformLocation(program, "ModelToWorld"), 1, GL_TRUE, matMtW.m);
	DrawModel(martin, program, "in_Position", "in_Normal", "inTexCoord");
	
	glUniformMatrix4fv(glGetUniformLocation(program, "ModelToWorld"), 1, GL_TRUE, matMtW2.m);
	DrawModel(martin, program, "in_Position", "in_Normal", "inTexCoord");

	glUniformMatrix4fv(glGetUniformLocation(program, "ModelToWorld"), 1, GL_TRUE, bigMartinMtw.m);
	DrawModel(martin, program, "in_Position", "in_Normal", "inTexCoord");
	
	printError("display");
	glutSwapBuffers();
}


int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitWindowSize(WINDOW_SIZE, WINDOW_SIZE);
	glutWarpPointer(WINDOW_SIZE / 2, WINDOW_SIZE / 2);
	glutCreateWindow ("GL3 white triangle example");
	glutDisplayFunc(display); 
	init ();
	glutMainLoop();
	return 0;
}
