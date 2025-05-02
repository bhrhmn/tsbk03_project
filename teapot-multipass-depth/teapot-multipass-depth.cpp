//uses framework Cocoa
//uses framework OpenGL
#define MAIN
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "LoadTGA.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"

#define	width	128
#define	height	128

	int lastw, lasth;

GLfloat vertices[] = { -1.0f,-1.0f,0.0f,
						-1.0f,1.0f,0.0f,
						1.0f,-1.0f,0.0f,
						1.0f,1.0f,0.0f,
						};
GLfloat texCoords[] = {	0.0f, 0.0f,
						0.0f, 1.0f,
						1.0f, 0.0f,
						1.0f, 1.0f,
						};
GLuint indices[] = {0,1,2,1,3,2};

mat4 modelView, projectionMatrix;

// Reference to shader program
GLuint program;
// Reference to texture
GLuint tex;
// teapot
Model *teapot, *quad;

void initTextures()
{
// create tex
	glGenTextures(1, &tex); // texture id
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL); // NULL = Empty texture. Undocumented feature?
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
// (set texture parameters here)
}

void init(void)
{
	dumpInfo();

	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("tex.vert", "tex.frag");
	printError("init shader");
	
	// Upload geometry to the GPU:
	teapot = LoadModel("teapot.obj");
	
	quad = LoadDataToModel(
			(vec3 *)vertices,
			NULL,
			(vec2 *) texCoords,
			NULL,
			indices,
			4,
			6);

	// End of upload of geometry

	initTextures();

	glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0

	printError("init arrays");
}

float a = 0.0;

void display(void)
{
	printError("pre display");
	a += 0.02;

	// clear the screen
	glViewport(0, 0, width, height);
	glClearColor(1,1,0.5,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	modelView = lookAt(0, 0, 2.5, 0, 0, 0, 0, 1, 0) * ArbRotate(vec3(1,1,1), a);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelView"), 1, GL_TRUE, modelView.m);
	
	projectionMatrix = frustum(-1,1, -1,1, 1, 30);
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);

	DrawModel(teapot, program, "in_Position", "in_Normal", "in_Texcoord");
	
	printError("display");
	
	glFlush();
	
	// Kopiera resultatet till texturen
	glBindTexture(GL_TEXTURE_2D, tex);
//	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, width, height);
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 0,0, width, height,0);

	glViewport(0, 0, lastw, lasth);
	
// Rendera till fönstret med den genererade texturen
	
	glClearColor(0.3, 0.3, 0.7,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DrawModel(teapot, program, "in_Position", "in_Normal", "in_Texcoord");
	
	printError("display 2");

	glutSwapBuffers();
}

void reshape(GLsizei w, GLsizei h)
{
	lastw = w;
	lasth = h;
	
	glViewport(0, 0, w, h);
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2); // Might not be needed in Linux
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow ("Render depth to texture example (glCopyTexSubImage2D)");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutRepeatingTimer(20);
	init ();
	glutMainLoop();
	exit(0);
}
