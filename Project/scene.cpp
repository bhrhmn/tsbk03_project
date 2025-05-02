
#include "scene.h"

// We assign one texture unit in which to store the transformation.
#define TEX_UNIT 0

// Global variable definitions (declared extern in header)
Model *ground;
Model *skybox;
Model *sofa;
Model *table;
Model *cabin;
Model *fireplace;

unsigned int myTex;
unsigned int myTex2;
unsigned int cabintex;
unsigned int sofatex;
unsigned int fireplacetex;



FBOstruct *fbo;

mat4 totalGround;
mat4 worldCamera;
mat4 cabinT;
mat4 FireplaceT;
mat4 tableT;
mat4 sofaT;
mat4 modelViewMatrix;

vec3 worldCameraP = { 0.0f, 5.0f, 25.0f };
vec3 worldCameraL = { 0.0f, 5.0f, 0.0f };
vec3 worldCameraV = { 0.0f, 5.0f, 0.0f };

GLuint ground_shader;
GLuint shybox_shader;
GLuint object_shader;
GLuint shadow_shader;

mat4 shadowProjectionMatrix;



vec3 firePos = vec3(33, 0, 23);
vec3 fireColor = vec3(0.8, 0.5, 0.2);

GLfloat t = 0;


// Function implementations
void InstantiateModels() {
    ground = LoadDataToModel(vertices, vertex_normals, tex_coords, vertex_normals, indices, 4, 6);
    skybox = LoadModel("skybox/skybox.obj");
    sofa = LoadModel("Models/sofa/model/SOFA.obj.obj");
    table = LoadModel("Models/Table.obj");
    cabin = LoadModel("Models/WoodenCabinObj.obj");
    fireplace = LoadModel("Models/fireplace_blender.obj");
    
    cabinT = T(20,-10,0) * S(1);
    FireplaceT = T(35,-5,25) * Ry(5*M_PI/4) * S(9);
    tableT = T(20,-12,-10) * S(8);
    sofaT = T(20,-4,-30)* S(8);
    

    totalGround = T(0,-10,0);
}

void InstantiateTextures() {


    glActiveTexture(GL_TEXTURE1);
    LoadTGATextureSimple("Models/grass.tga", &myTex2);
    glBindTexture(GL_TEXTURE_2D, myTex2);

    glActiveTexture(GL_TEXTURE2);
    LoadTGATextureSimple("Models/WoodCabinDif.tga", &cabintex);
    glBindTexture(GL_TEXTURE_2D, cabintex);

    glActiveTexture(GL_TEXTURE3);
    LoadTGATextureSimple("Models/stonebrick.tga", &fireplacetex);
    glBindTexture(GL_TEXTURE_2D, fireplacetex);

    glActiveTexture(GL_TEXTURE4);
    LoadTGATextureSimple("Models/sofa/color.tga", &sofatex);
    glBindTexture(GL_TEXTURE_2D, sofatex);

    glActiveTexture(GL_TEXTURE5);
    LoadTGATextureSimple("skybox-alt/atmosphere-cloud.tga", &myTex);
    glBindTexture(GL_TEXTURE_2D, myTex);

    glActiveTexture(GL_TEXTURE6);

}

void OnTimer(int value) {
    glutPostRedisplay();
    glutTimerFunc(20, &OnTimer, value);
}

void init(void) {
    dumpInfo();

    // GL inits
    glClearColor(0.8,0.8,0.8,0);

    // Load and compile shader
    ground_shader = loadShaders("Shaders/ground.vert", "Shaders/ground.frag");
    shybox_shader = loadShaders("Shaders/skybox.vert", "Shaders/skybox.frag");
    shadow_shader = loadShaders("Shaders/shadow.vert", "Shaders/shadow.frag");
    object_shader = loadShaders("Shaders/object.vert", "Shaders/object.frag");


    printError("init shader");
    
    // Textures
    InstantiateTextures();
    printError("Init Textures");

    shadowProjectionMatrix = perspective(45, WINDOW_SIZE/WINDOW_SIZE, 10, 4000);

    
    // Models
    InstantiateModels();
    printError("Init Models");
    
    // Upload Projection Matrix Once to each shader
    glUseProgram(shybox_shader);
    glUniformMatrix4fv(glGetUniformLocation(shybox_shader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
    glUseProgram(ground_shader);
    glUniformMatrix4fv(glGetUniformLocation(ground_shader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
    glUseProgram(object_shader);
    mat4 scaleBiasMatrix = T(0.5, 0.5, 0.0) * S(0.5, 0.5, 1.0);

    glUniformMatrix4fv(glGetUniformLocation(object_shader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
    uploadMat4ToShader(object_shader, "scaleBiasMatrix", scaleBiasMatrix);

    // upload fire
    glUniform3fv(glGetUniformLocation(object_shader, "firePos"), 1, &firePos.x);
    glUniform3fv(glGetUniformLocation(object_shader, "fireColor"), 1, &fireColor.x);

    glUseProgram(shadow_shader);
    uploadMat4ToShader(shadow_shader, "projectionMatrix", shadowProjectionMatrix);


    //setTextureMatrix
    uploadMat4ToShader(shadow_shader, "scaleBiasMatrix", scaleBiasMatrix);

    
    // Start timer
    glutTimerFunc(20, &OnTimer, 0);
    printError("init arrays");

}

void moveCamera(){
    vec3 direction = normalize(worldCameraL - worldCameraP);
    vec3 side_dir = normalize(cross(vec3(0,1,0), direction));
    if (glutKeyIsDown('a')) {
        worldCameraL += side_dir;
        worldCameraP += side_dir;
    }
    if (glutKeyIsDown('d')) {
        worldCameraL -= side_dir;
        worldCameraP -= side_dir;
    }
    if (glutKeyIsDown('w')) {
        worldCameraL += direction;
        worldCameraP += direction;
    }
    if (glutKeyIsDown('s')) {
        worldCameraL -= direction;
        worldCameraP -= direction;
    }
    if (glutKeyIsDown('q')) {
        worldCameraL = worldCameraP + Ry(0.05)*direction;
    }
    if (glutKeyIsDown('e')) {
        worldCameraL = worldCameraP + Ry(-0.05)*direction;
    }
}

void DrawCabin(GLuint shader){
    glActiveTexture(GL_TEXTURE2);
    glUniform1i(glGetUniformLocation(shader, "texUnit"), 2); // Texture unit 0
    uploadMat4ToShader(shader, "model_To_World", cabinT);
	DrawModel(cabin, shader, "in_Position", "inNormal", "inTexCord");
    printError("DrawCabin");
}

void DrawFireplace(GLuint shader){
    glActiveTexture(GL_TEXTURE3);
    glUniform1i(glGetUniformLocation(shader, "texUnit"), 3); 
    uploadMat4ToShader(shader, "model_To_World", FireplaceT);
	DrawModel(fireplace, shader, "in_Position", "inNormal", "inTexCord");
    printError("DrawFireplace");
}

void DrawTable(GLuint shader){
    glActiveTexture(GL_TEXTURE2);
    glUniform1i(glGetUniformLocation(shader, "texUnit"), 2); // Texture unit 0
	uploadMat4ToShader(shader, "model_To_World", tableT);
	DrawModel(table, shader, "in_Position", "inNormal", "inTexCord");
    printError("DrawTable");
}

void DrawSofa(GLuint shader){
    glActiveTexture(GL_TEXTURE4);
    glUniform1i(glGetUniformLocation(shader, "texUnit"), 4); 
	uploadMat4ToShader(shader, "model_To_World", sofaT);
	DrawModel(sofa, shader, "in_Position", "inNormal", "inTexCord");
    printError("DrawSofa");
}

void DrawSkyBox(){
    glUseProgram(shybox_shader);
    vec3 newWorldCamera = worldCameraP;
    worldCamera = lookAtv(newWorldCamera, worldCameraL, worldCameraV);
    glDisable(GL_DEPTH_TEST);

    glActiveTexture(GL_TEXTURE5);
    glUniform1i(glGetUniformLocation(shybox_shader, "texUnit"), 5);
    
    mat4 worldCameraCopy = worldCamera;
    worldCameraCopy.m[3] = 0;
    worldCameraCopy.m[7] = 0;
    worldCameraCopy.m[11] = 0;
    uploadMat4ToShader(shybox_shader, "world_To_View", worldCameraCopy);

    uploadMat4ToShader(shybox_shader, "model_To_World", IdentityMatrix());
    DrawModel(skybox, shybox_shader, "in_Position", "inNormal", "inTexCord");

    glEnable(GL_DEPTH_TEST);
    printError("DrawSkyBox");
}

void DrawGround(){
    //Draw ground
    glUseProgram(ground_shader);
    glActiveTexture(GL_TEXTURE1);
    glUniform1i(glGetUniformLocation(ground_shader, "texUnit"), 1); // Texture unit 1
    uploadMat4ToShader(ground_shader, "world_To_View", worldCamera);
    uploadMat4ToShader(ground_shader, "model_To_World", totalGround);
    DrawModel(ground, ground_shader, "in_Position", "inNormal", "inTexCord");

    printError("DrawGround");
}

void UpdateLightSources(){
    firePos = vec3(33+sin(t*10)*2, 0, 23);
    glUniform3fv(glGetUniformLocation(object_shader, "firePos"), 1, &firePos.x);
    printError("UpdateLightSources");
}

void drawObjects(GLuint shader){
    DrawCabin(shader);
    DrawFireplace(shader);
    DrawSofa(shader);
    DrawTable(shader);

}


void display(void)
{
	printError("pre display");

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    t = (GLfloat)glutGet(GLUT_ELAPSED_TIME)/1000;
    
    moveCamera();
    DrawSkyBox();
    DrawGround();


	
    //draw using object shader
	glUseProgram(object_shader);
    uploadMat4ToShader(object_shader, "world_To_View", worldCamera);
    UpdateLightSources();

    //Shadow things
    // Setup the modelview from the light source
    vec3 table_pos = vec3(20,-12,-10);
	modelViewMatrix = lookAt(firePos.x, firePos.y, firePos.z,
        table_pos.x, table_pos.y, table_pos.z, 0,1,0);

    glUseProgram(shadow_shader);
    uploadMat4ToShader(shadow_shader, "world_To_View", modelViewMatrix);


    // 1. Render scene to FBO

	useFBO(fbo, NULL, NULL);
	glViewport(0,0,WINDOW_SIZE,WINDOW_SIZE);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE); // Depth only
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Using the simple shader
	glUniform1i(glGetUniformLocation(shadow_shader, "textureUnit"),TEX_UNIT);
	glActiveTexture(GL_TEXTURE0 + TEX_UNIT);
	glBindTexture(GL_TEXTURE_2D,0);
	
	drawObjects(shadow_shader);

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
    //Binds to the active texture. 
    fbo = initFBO2(WINDOW_SIZE, WINDOW_SIZE, 0, 1);

	glutDisplayFunc(display); 

	glutMainLoop();
	return 0;
}


