
#define MAIN
#include "scene.h"
#include "camera.h"
#include "models.h"
#include "rendering.h"
#include "lighting.h"
#include "shadows.h"
//#include "sounds.h"

FBOstruct *fireFbo, *moonFbo, *bloomFbo, *overFlowFbo, *tempFbo;

GLuint shybox_shader;
GLuint object_shader;
GLuint shadow_shader;
GLuint tree_shader;
GLuint overflow_shader;
GLuint lowpass_shader;
GLuint bloom_shader;

GLfloat t = 0;


void OnTimer(int value) {
    glutPostRedisplay();
    glutTimerFunc(20, &OnTimer, value);
}

void init() {
    dumpInfo();

    // GL inits
    glClearColor(0.8,0.8,0.8,0);

    // Load and compile shader
    shybox_shader = loadShaders("Shaders/skybox.vert", "Shaders/skybox.frag");
    shadow_shader = loadShaders("Shaders/shadow.vert", "Shaders/shadow.frag");
    object_shader = loadShaders("Shaders/object.vert", "Shaders/object.frag");
    tree_shader = loadShaders("Shaders/tree.vert", "Shaders/tree.frag");
    overflow_shader = loadShaders("Shaders/overflow.vert", "Shaders/overflow.frag");
    lowpass_shader = loadShaders("Shaders/lowpassfilter.vert", "Shaders/lowpassfilter.frag");
    bloom_shader = loadShaders("Shaders/overflow.vert", "Shaders/bloom.frag");
    printError("init shader");
    
    // Textures
    InstantiateTextures();
    printError("Init Textures");

    shadowProjectionMatrix = perspective(45, WINDOW_SIZE/WINDOW_SIZE, 10, 100);
    mat4 scaleBiasMatrix = T(0.5, 0.5, 0.0) * S(0.5, 0.5, 1.0);
    
    // Models
    InstantiateModels();
    printError("Init Models");
    
    // Upload Projection Matrix Once to each shader (and scaleBiasMatrix)
    glUseProgram(shybox_shader);
    glUniformMatrix4fv(glGetUniformLocation(shybox_shader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);

    glUseProgram(shadow_shader);
    uploadMat4ToShader(shadow_shader, "projectionMatrix", shadowProjectionMatrix);
    uploadMat4ToShader(shadow_shader, "scaleBiasMatrix", scaleBiasMatrix);

    glUseProgram(tree_shader);
    glUniformMatrix4fv(glGetUniformLocation(tree_shader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);

    glUseProgram(object_shader);
	glUniformMatrix4fv(glGetUniformLocation(object_shader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
	uploadMat4ToShader(object_shader, "scaleBiasMatrix", scaleBiasMatrix);
    glUniform3fv(glGetUniformLocation(object_shader, "firePos"), 1, &firePos.x);
    glUniform3fv(glGetUniformLocation(object_shader, "fireColor"), 1, &fireColor.x);
    glUniform3fv(glGetUniformLocation(object_shader, "moonPos"), 1, &moonPos.x);
    glUniform3fv(glGetUniformLocation(object_shader, "moonColor"), 1, &moonColor.x);
    
    // Start timer
    glutTimerFunc(20, &OnTimer, 0);
    // initFireplaceSound();

    // init fbos
    glActiveTexture(GL_TEXTURE13);
    fireFbo = initFBO2(WINDOW_SIZE, WINDOW_SIZE, 0, 1);
    glActiveTexture(GL_TEXTURE14);
    moonFbo = initFBO2(WINDOW_SIZE, WINDOW_SIZE, 0, 1);
    glActiveTexture(GL_TEXTURE16);
    bloomFbo = initFBO2(WINDOW_SIZE, WINDOW_SIZE, 0, 1);
    glActiveTexture(GL_TEXTURE17);
    overFlowFbo = initFBO2(WINDOW_SIZE, WINDOW_SIZE, 0, 1);
    glActiveTexture(GL_TEXTURE18);
    tempFbo = initFBO2(WINDOW_SIZE, WINDOW_SIZE, 0, 1);

    printError("init arrays");
}


void display()
{
	printError("pre display");

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    t = (GLfloat)glutGet(GLUT_ELAPSED_TIME)/1000;
    glEnable(GL_DEPTH_TEST);

    //draw using object shader
	glUseProgram(object_shader);
    
    UpdateLightSources();
    UpdateMoon();
    UpdateWolf();
    fireShadow();
    moonShadow();

    //2. Render from camera.
    // With bloom
	useFBO(bloomFbo, fireFbo, moonFbo);
    // Without bloom, also remove blooming() function call 
	//useFBO(NULL, fireFbo, moonFbo);
	
    glViewport(0,0,WINDOW_SIZE,WINDOW_SIZE);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Using the projTex (object) shader
    glUseProgram(object_shader);

	//load both fbo depth maps to shader
	glUniform1i(glGetUniformLocation(object_shader, "textureUnit"),TEX_UNIT);
	glActiveTexture(GL_TEXTURE0 + TEX_UNIT);
	glBindTexture(GL_TEXTURE_2D,fireFbo->depth);

	glUniform1i(glGetUniformLocation(object_shader, "textureUnitMoon"),MOON_TEX_UNIT);
	glActiveTexture(GL_TEXTURE0 + MOON_TEX_UNIT);
	glBindTexture(GL_TEXTURE_2D,moonFbo->depth);

    moveCamera();
	MoveModel();

    uploadMat4ToShader(object_shader, "world_To_View", worldCamera);
    DrawSkyBox();

    drawObjects(object_shader);
	DrawTree();
	DrawFire();
	DrawWolf();
    blooming();

	glutSwapBuffers();
}



int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitContextVersion(3, 2);
	glutInitWindowSize(WINDOW_SIZE, WINDOW_SIZE);
	glutCreateWindow ("Cosy Cabin");
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    init();
   
	glutDisplayFunc(display); 

	glutMainLoop();
	return 0;
}


