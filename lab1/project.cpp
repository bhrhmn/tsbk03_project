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
#define WINDOW_SIZE 600


// Globals
// Data would normally be read from files

// fire
vec3 firePos = vec3(30, 0, 25);
vec3 fireColor = vec3(0.8, 0.5, 0.2);

//Models
Model *ground;
Model *skybox;
Model *sofa;
Model *table;
Model *cabin;

vec3 color1 = vec3(1, 0, 0);
vec3 color2 = vec3(0, 1, 0);



GLfloat projectionMatrix[] = {    2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,

                                            0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,

                                            0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),

                                            0.0f, 0.0f, -1.0f, 0.0f };


mat4 totalGround;
mat4 worldCamera;
mat4 zeroedge = { 1, 1, 1, 0,
                    1, 1, 1, 0, 
                    1, 1, 1, 0, 
                    1, 1, 1, 1};

GLfloat t = 0;

vec3 worldCameraP = { 0.0f, 5.0f, 25.0f};


vec3 worldCameraL = { 0.0f, 5.0f, 0.0f,};


vec3 worldCameraV = { 0.0f, 5.0f, 0.0f,};

// vertex array object
unsigned int vertexArrayObjID;
GLuint program;
GLuint shybox_shader;
GLuint object_shader;


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
vec3 upDir = {0, 1, 0};




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
unsigned int cabintex;


void init(void)
{
	dumpInfo();

	// GL inits
	glClearColor(0.8,0.8,0.8,0);

    glDisable(GL_CULL_FACE);

	// Load and compile shader
	
	
    program = loadShaders("program.vert", "program.frag");
    shybox_shader = loadShaders("skybox.vert", "skybox.frag");
	object_shader = loadShaders("object.vert", "object.frag");
	printError("init shader");
    

    //Textures

    glActiveTexture(GL_TEXTURE0);
    LoadTGATextureSimple("skybox-alt/atmosphere-cloud.tga", &myTex);
    glBindTexture(GL_TEXTURE_2D, myTex);

    glActiveTexture(GL_TEXTURE1);
    LoadTGATextureSimple("grass.tga", &myTex2);
    glBindTexture(GL_TEXTURE_2D, myTex2);


    glActiveTexture(GL_TEXTURE2);
    LoadTGATextureSimple("WoodCabinDif.tga", &cabintex);
    glBindTexture(GL_TEXTURE_2D, cabintex);
    
    totalGround = T(0,-10,0);
    ground = LoadDataToModel(vertices, vertex_normals, tex_coords, vertex_normals, indices, 4, 6);

    worldCamera = lookAtv(worldCameraP, worldCameraL, worldCameraV);

    skybox = LoadModel("skybox/skybox.obj");
	sofa = LoadModel("Koltuk.obj");
	table = LoadModel("Table.obj");
    cabin = LoadModel("WoodenCabinObj.obj");

    printError("textcoodarray");
    
    //Model done
    printError("init color");

    //uploadMat4toshader, vecutils
    glUseProgram(shybox_shader);
    glUniformMatrix4fv(glGetUniformLocation(shybox_shader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);



    //uploadMat4toshader, vecutils
    glUseProgram(object_shader);
    glUniformMatrix4fv(glGetUniformLocation(object_shader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
    
    glUseProgram(program);
    glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);

    
    //Start timer
    glutTimerFunc(20, &OnTimer, 0);


    // upload fire
	glUniform3fv(glGetUniformLocation(program, "firePos"), 1, firePos);
	glUniform3fv(glGetUniformLocation(program, "fireColor"), 1, fireColor);


    	
	printError("init arrays");
}

void moveCamera(){
    vec3 direction = normalize(worldCameraL - worldCameraP);
    vec3 side_dir = normalize(cross(vec3(0,1,0), direction));
    printf("dir %f \n", direction.y);
    if (glutKeyIsDown('a')) {
        worldCameraL += side_dir;
        worldCameraP += side_dir;
        worldCamera = lookAtv(worldCameraP, worldCameraL, worldCameraV);
        uploadMat4ToShader(program, "worldCamera", worldCamera);
    }
    if (glutKeyIsDown('d')) {
        worldCameraL -= side_dir;
        worldCameraP -= side_dir;
        worldCamera = lookAtv(worldCameraP, worldCameraL, worldCameraV);
    uploadMat4ToShader(program, "worldCamera", worldCamera);
    }
    if (glutKeyIsDown('w')) {
        
        worldCameraL += direction;
        worldCameraP += direction;
        worldCamera = lookAtv(worldCameraP, worldCameraL, worldCameraV);
    uploadMat4ToShader(program, "worldCamera", worldCamera);
    }
    if (glutKeyIsDown('s')) {
        worldCameraL -= direction;
        worldCameraP -= direction;
        worldCamera = lookAtv(worldCameraP, worldCameraL, worldCameraV);
        uploadMat4ToShader(program, "worldCamera", worldCamera);
    }

    if (glutKeyIsDown('q')) {
        worldCameraL += Ry(-1.1)*direction;
        worldCamera = lookAtv(worldCameraP, worldCameraL, worldCameraV);
        uploadMat4ToShader(program, "worldCamera", worldCamera);
    }
    if (glutKeyIsDown('e')) {
        //worldCameraL = T(worldCameraP)*Ry(0.1)*T(0,0,0)*worldCameraL;
        //get direction vector P-L and rotate it. 
        worldCameraL += Ry(1.1)*direction;
        worldCamera = lookAtv(worldCameraP, worldCameraL, worldCameraV);
        uploadMat4ToShader(program, "worldCamera", worldCamera);

    }
}


void DrawCabin(){
    mat4 cabinT = T(20,-10,0) * S(1);
    glActiveTexture(GL_TEXTURE2);
    glUniform1i(glGetUniformLocation(object_shader, "texUnit"), 2); // Texture unit 0
    uploadMat4ToShader(object_shader, "mdlMatrix", cabinT);
	uploadMat4ToShader(object_shader, "worldCamera", worldCamera);
	DrawModel(cabin, object_shader, "in_Position", "inNormal", "inTexCord");
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

    glEnable(GL_DEPTH_TEST);
    

	//Draw Models
	glUseProgram(object_shader);

    
	mat4 sofaT = T(25,-7,-5) * S(20);
    
	uploadMat4ToShader(object_shader, "mdlMatrix", sofaT);
	uploadMat4ToShader(object_shader, "worldCamera", worldCamera);
	DrawModel(sofa, object_shader, "in_Position", "inNormal", "inTexCord");

    //Draw table

	mat4 tableT = T(40,-10,10) * S(8);
	uploadMat4ToShader(object_shader, "mdlMatrix", tableT);
	DrawModel(table, object_shader, "in_Position", "inNormal", "inTexCord");

    DrawCabin();

	//Draw ground
    glUseProgram(program);
    glActiveTexture(GL_TEXTURE1);
    glUniform1i(glGetUniformLocation(program, "texUnit"), 1); // Texture unit 1
    uploadMat4ToShader(program, "worldCamera", worldCamera);
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
	glutInitWindowSize(WINDOW_SIZE, WINDOW_SIZE);
	glutCreateWindow ("GL3 white triangle example");
	glutDisplayFunc(display); 
	init ();
	glutMainLoop();
	return 0;
}
