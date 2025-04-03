// Lab 1-1.
// This is the same as the first simple example in the course book,
// but with a few error checks.
// Remember to copy your file to a new on appropriate places during the lab so you keep old results.
// Note that the files "lab1-1.frag", "lab1-1.vert" are required.

// Should work as is on Linux and Mac. MS Windows needs GLEW or glee.
// See separate Visual Studio version of my demos.
#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	// Linking hint for Lightweight IDE
	// uses framework Cocoa
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "math.h"
#define MAIN
#include "LittleOBJLoader.h"
#include "LoadTGA.h"
#include "VectorUtils4.h"
#include <math.h>


#define near 1.0
#define far 200.0
#define right 0.5
#define left -0.5
#define top 0.5
#define bottom -0.5
#define kGroundSize 100.0f

// Globals
// Data would normally be read from files

//MOdel 1
Model *wing1;
Model *wing2;
Model *wing3;
Model *wing4;

Model *roof;
Model *wall;
Model *balcony;

Model *ground;
Model *skybox;
Model *sofa;

vec3 color1 = vec3(1, 0, 0);
vec3 color2 = vec3(0, 1, 0);



GLfloat projectionMatrix[] = {    2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,

                                            0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,

                                            0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),

                                            0.0f, 0.0f, -1.0f, 0.0f };
GLfloat projectionMatrix2[] = {    2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,

                                            0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,

                                            0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),

                                            0.0f, 0.0f, -1.0f, 0.0f };

//Vector Utils
mat4 totalWing1;
mat4 totalWing2;
mat4 totalWing3;
mat4 totalWing4;

mat4 totalwall;
mat4 totalRoof;
mat4 totalBalcony;

mat4 totalGround;
mat4 worldCamera;
mat4 zeroedge = { 1, 1, 1, 0,
                    1, 1, 1, 0, 
                    1, 1, 1, 0, 
                    1, 1, 1, 1};

GLfloat t = 0;

vec3 worldCameraP = { 0.0f, 0.0f, 25.0f};


vec3 worldCameraL = { 0.0f, 0.0f, 0.0f,};


vec3 worldCameraV = { 0.0f, 1.0f, 0.0f,};

// vertex array object
unsigned int vertexArrayObjID;
GLuint program;
GLuint shybox_shader;

//Skybox
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

vec3 color[] =
{
  vec3(0.0f,1.0f,0.0f),
  vec3(0.0f,1.0f,0.0f),
  vec3(0.0f,1.0f,0.0f),
  vec3(0.0f,1.0f,0.0f)
};

//Skybox ground
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





void OnTimer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(20, &OnTimer, value);
}

mat4 changeTranslationWing(float offset){

    return S(0.75, 0.75, 0.75)*T(0,-1,6.5)*Rz(t+offset)*Ry(M_PI/2);
    //return Ry(M_PI/2);

}

unsigned int myTex; 
unsigned int myTex2; 


void init(void)
{
	dumpInfo();

	// GL inits
	glClearColor(0.8,0.8,0.8,0);

    glDisable(GL_CULL_FACE);

	// Load and compile shader
	
	
    program = loadShaders("program.vert", "program.frag");
    shybox_shader = loadShaders("skybox.vert", "skybox.frag");
	printError("init shader");
    

    //Textures

    glActiveTexture(GL_TEXTURE0);
    LoadTGATextureSimple("skybox-alt/atmosphere-cloud.tga", &myTex);
    glBindTexture(GL_TEXTURE_2D, myTex);

    glActiveTexture(GL_TEXTURE1);
    LoadTGATextureSimple("grass.tga", &myTex2);
    glBindTexture(GL_TEXTURE_2D, myTex2);


    
    totalGround = T(0,-10,0);
    ground = LoadDataToModel(vertices, vertex_normals, tex_coords, vertex_normals, indices, 4, 6);

    worldCamera = lookAtv(worldCameraP, worldCameraL, worldCameraV);

    skybox = LoadModel("skybox/skybox.obj");
	sofa = LoadModel("sofa.obj");
    printError("textcoodarray");
    


    //set wing rotations
    totalWing1 = changeTranslationWing(0.0);
    totalWing2 = changeTranslationWing(M_PI/2);
    totalWing3 = changeTranslationWing(M_PI);
    totalWing4 = changeTranslationWing(3*M_PI/2);
    //Model done
    printError("init color");

    //uploadMat4toshader, vecutils
    glUseProgram(shybox_shader);
    glUniformMatrix4fv(glGetUniformLocation(shybox_shader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix2);
    
    glUseProgram(program);
    glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);

    
    //Start timer
    glutTimerFunc(20, &OnTimer, 0);

    	
	printError("init arrays");
}

void moveCamera(){
    if (glutKeyIsDown('a')) {
        worldCameraL = T(-0.5,0,0)*worldCameraL;
        worldCameraP = T(-0.5,0,0)*worldCameraP;
        worldCamera = lookAtv(worldCameraP, worldCameraL, worldCameraV);
        uploadMat4ToShader(program, "worldCamera", worldCamera);
    }
    if (glutKeyIsDown('d')) {
        worldCameraL = T(0.5,0,0)*worldCameraL;
        worldCameraP = T(0.5,0,0)*worldCameraP;
        worldCamera = lookAtv(worldCameraP, worldCameraL, worldCameraV);
    uploadMat4ToShader(program, "worldCamera", worldCamera);
    }
    if (glutKeyIsDown('w')) {
        worldCameraP = T(0,0,-0.5)*worldCameraP;
        worldCameraL = T(0,0,-0.5)*worldCameraL;
        worldCamera = lookAtv(worldCameraP, worldCameraL, worldCameraV);
    uploadMat4ToShader(program, "worldCamera", worldCamera);
    }
    if (glutKeyIsDown('s')) {
        worldCameraP = T(0,0,0.5)*worldCameraP;
        worldCameraL = T(0,0,0.5)*worldCameraL;
        worldCamera = lookAtv(worldCameraP, worldCameraL, worldCameraV);
        uploadMat4ToShader(program, "worldCamera", worldCamera);
    }

    if (glutKeyIsDown('q')) {
        vec3 direction = worldCameraL - worldCameraP;
        worldCameraL = Ry(-0.1)*direction;
        worldCamera = lookAtv(worldCameraP, worldCameraL, worldCameraV);
        uploadMat4ToShader(program, "worldCamera", worldCamera);
    }
    if (glutKeyIsDown('e')) {
        //worldCameraL = T(worldCameraP)*Ry(0.1)*T(0,0,0)*worldCameraL;
        //get direction vector P-L and rotate it. 
        vec3 direction = worldCameraL - worldCameraP;
        worldCameraL = Ry(0.1)*direction;
        worldCamera = lookAtv(worldCameraP, worldCameraL, worldCameraV);
        uploadMat4ToShader(program, "worldCamera", worldCamera);

    }
}


void display(void)
{
	printError("pre display");

    glUseProgram(shybox_shader);
	// clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    t = (GLfloat)glutGet(GLUT_ELAPSED_TIME)/1000;

    //World camera
    vec3 newWorldCamera = worldCameraP;
    worldCamera = lookAtv(newWorldCamera, worldCameraL, worldCameraV);

    //Draw skybox
    glDisable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shybox_shader, "texUnit"), 0); // Texture unit 0
    
    
    mat4 worldCameraCopy = worldCamera;
    worldCameraCopy.m[3] = 0;
    worldCameraCopy.m[7] = 0;
    worldCameraCopy.m[11] = 0;
    uploadMat4ToShader(shybox_shader, "worldCamera", worldCameraCopy);

    uploadMat4ToShader(shybox_shader, "mdlMatrix", IdentityMatrix());
    DrawModel(skybox, shybox_shader, "in_Position", "inNormal", "inTexCord");

    //Draw Models
    glUseProgram(program);
    glEnable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE1);
    
    uploadMat4ToShader(program, "worldCamera", worldCamera);

	mat4 sofaT = T(0,-10,0);
	uploadMat4ToShader(program, "mdlMatrix", sofaT);
	DrawModel(sofa, program, "in_Position", "inNormal", "inTexCord");

    
    glUniform1i(glGetUniformLocation(program, "texUnit"), 1); // Texture unit 0

    //Draw ground
    uploadMat4ToShader(program, "mdlMatrix", totalGround);
    DrawModel(ground, program, "in_Position", "inNormal", "inTexCord");

    moveCamera();   
	printError("display");
	glutSwapBuffers();
}





int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitContextVersion(3, 2);

	glutCreateWindow ("GL3 white triangle example");
	glutDisplayFunc(display); 
	init ();
	glutMainLoop();
	return 0;
}
