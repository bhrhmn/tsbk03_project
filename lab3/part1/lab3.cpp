// Lab 1-6.

#define MAIN

#include "GL_utilities.h"
#include "MicroGlut.h"
#include "LittleOBJLoader.h" 
#include "LoadTGA.h" 
#include "VectorUtils4.h" 
#include <math.h>

//WARN: DO NOT define before include section 
#define near 1.0
#define far 60.0
#define right 0.5
#define left -0.5
#define top 0.5
#define bottom -0.5

#define cameraPos vec3(-4, 10, -40)
#define focusPoint vec3(0, 5, 0)

GLfloat projectionMatrix[] = {    
	2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,
  0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,
  0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),
  0.0f, 0.0f, -1.0f, 0.0f 
};


// Refrence to walls, initialized in init()
Model *walls;
Model *roof;
Model *balcony;
Model *wing;

// Reference to shader program
GLuint program;

// Refrence to texture
GLuint tex;

// vertex array object
unsigned int modelsVertexArrayObjID;


void init(void)
{
	
	//mat4 lookAtv(vec3 p, vec3 l, vec3 v);
	mat4 camera = lookAtv(
		cameraPos,
		focusPoint,
		vec3(0, 1, 0)
	);

	// set to rerender in ~60FPS
	glutRepeatingTimer(16);
	
	dumpInfo();

	// GL inits
	glClearColor(0.9, 0.3, 0.4 ,0);
	//glDisable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	//glDisable(GL_CULL_FACE);
	
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab3.vert", "lab3.frag");
	
	//NOTE: always do this after loadShaders
	glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_TRUE, projectionMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "worldToView"), 1, GL_TRUE, camera.m);
	
	printError("init shader");

	walls 	= LoadModel("../windmill/windmill-walls2.obj");
	roof  	= LoadModel("../windmill/windmill-roof.obj");
	balcony =	LoadModel("../windmill/windmill-balcony.obj");
	wing  	=	LoadModel("../windmill/blade.obj");

	// Load & Activate texture
	LoadTGATextureSimple("../conc.tga", &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	
	printError("init arrays");
}


void display(void)
{
	printError("pre display");
	
	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	GLfloat theta = t/600;

	// Model to world matrices
	mat4 trans = T(0, 0, 0);
	mat4 rot = Ry(theta);
	mat4 wallsMtW = rot * trans; 
	
	mat4 trans2 = T(0, 0.35, 0);
	mat4 roofMtW = rot * trans2;

	mat4 trans3 = T(0.2, 0, 0);
	mat4 balconyMtW = rot * trans3; 

	mat4 trans4 = T(4.9, 9.25, 0);
	mat4 rot4 	= Rx(theta) * Rx(M_PI / 2);
	mat4 rot4_1 = Rx(theta) * Rx(M_PI);
	mat4 rot4_2 = Rx(theta) * Rx(-M_PI / 2);
	mat4 rot4_3 = Rx(theta) * Rx(2 * M_PI);
	mat4 wingMtW1 = rot * trans4 * rot4;
	mat4 wingMtW2 = rot * trans4 * rot4_1;
	mat4 wingMtW3 = rot * trans4 * rot4_2;
	mat4 wingMtW4 = rot * trans4 * rot4_3;
	
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(modelsVertexArrayObjID);    // Select VAO

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

	printError("display");
	glutSwapBuffers();
}


int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitWindowSize(600, 600);
	glutCreateWindow ("GL3 white triangle example");
	glutDisplayFunc(display); 
	init ();
	glutMainLoop();
	return 0;
}
