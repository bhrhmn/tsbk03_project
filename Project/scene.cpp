#include <SFML/Audio.hpp>
#include "scene.h"
#include "tree.h"


// Global variable definitions (declared extern in header)
Model *ground;
Model *skybox;
Model *sofa;
Model *table;
Model *cabin;
Model *fireplace;
Model *treeBillboard;
Model *fireModel;
Model *fireModel2;
Model *tree_log;
Model *door;

unsigned int myTex;
unsigned int myTex2;
unsigned int cabintex;
unsigned int sofatex; 
unsigned int fireplacetex;
unsigned int maskrosTex;
unsigned int treeTex;
unsigned int fireTex;
unsigned int fire2Tex;
unsigned int logTex;
unsigned int wolfTex;
unsigned int doorTex;

FBOstruct *fbo;
FBOstruct *moonFbo;


mat4 totalGround;
mat4 worldCamera;
mat4 cabinT;
mat4 FireplaceT;
mat4 tableT;
mat4 sofaT;
mat4 modelViewMatrix;
mat4 fireT;
mat4 fireT2;
mat4 logT;
mat4 wolfT;
mat4 doorT;



const int FOREST_SIZE = 12;
mat4 treeMat[FOREST_SIZE]; 

vec3 worldCameraP = { 20.0f, 8.0f, 0.0f };
vec3 worldCameraL = { 20.0f, 8.0f, -25.0f };
vec3 worldCameraV = { 0.0f, 5.0f, 0.0f };

GLuint shybox_shader;
GLuint object_shader;
GLuint shadow_shader;
GLuint tree_shader;

mat4 shadowProjectionMatrix;



  
vec3 firePos = vec3(25, 10.0f, 20.f);
vec3 fireColor = vec3(0.8, 0.5, 0.2);

vec3 fire_start_pos = vec3(35.5, -2.5, 25.5);

vec3 moonPos = vec3(120.0, 70.0f, -120.f);
vec3 moonColor = vec3(0.8f, 0.8f, 1.0f);
GLfloat t = 0;
sf::Sound* fireSound = nullptr;

// Function implementations
void InstantiateModels() {
    ground = LoadDataToModel(vertices, vertex_normals, tex_coords, vertex_normals, indices, 4, 6);
    treeBillboard = LoadDataToModel(tree_vertices, tree_vertex_normals, tree_tex_coords, tree_vertex_normals, tree_indices, 4, 6);
    skybox = LoadModel("skybox/skybox.obj");
    sofa = LoadModel("Models/sofa/model/SOFA.obj.obj");
    table = LoadModel("Models/Table.obj");
    cabin = LoadModel("Models/WoodenCabinObj.obj");
    fireplace = LoadModel("Models/fireplace_blender.obj");
    tree_log = LoadModel("Models/tree_log/low_poly_log.obj");
    door = LoadModel("Models/newdoor.obj");
    
    cabinT = T(20,-10,0) * S(1);
    FireplaceT = T(35,-5,25) * Ry(5*M_PI/4) * S(9);
    tableT = T(20,-12,-10) * S(8);
    sofaT = T(20,-4,-30)* S(8);
    totalGround = T(0,-10,0);
    fireT = T(fire_start_pos.x, fire_start_pos.y, fire_start_pos.z) * Ry(5*M_PI/4) * S(0.1);
    fireT = T(fire_start_pos.x -0.5, fire_start_pos.y, fire_start_pos.z -0.5) * Ry(5*M_PI/4) * S(0.1);
    doorT = T(13,-2,40)*Ry(M_PI*3/2)* S(5.8);
    treeMat[0] = T(150, -5, -10);
    treeMat[1] = T(200, -5, 20);
    treeMat[2] = T(180, -5, -28);
    treeMat[3] = T(120, -5, 35);
    treeMat[4] = T(140, -5, -35);
    treeMat[5] = T(110, -5, -20);
    treeMat[6] = T(80, -5, -43);
    treeMat[7] = T(120, -5, -50);
    treeMat[8] = T(100, -5, 30);
    treeMat[9] = T(130, -5, 0);
    treeMat[10] = T(150, -5, 70);
    treeMat[11] = T(100, -5, 80);
    logT = T(fire_start_pos.x -1.0, -4, fire_start_pos.z -1.0) * Ry(5*M_PI/4) * Ry(M_PI_2) * S(0.025);
    wolfT = T(150, 3.5, 0) * Ry(M_PI_2*3) * Rx(M_PI) * S(0.4);
}




void initFireplaceSound() {
    static sf::SoundBuffer buffer;
    if (!buffer.loadFromFile("fireplace.wav")) {
        printf("Error: Could not load sound file!\n");
        return;
    }
    
    fireSound = new sf::Sound(buffer);
    fireSound->setLoop(true);
    fireSound->play();
}

void InstantiateTextures() {

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
    LoadTGATextureSimple("skybox/steven-c-skydome-render.tga", &myTex);
    glBindTexture(GL_TEXTURE_2D, myTex);

    glActiveTexture(GL_TEXTURE6);
    LoadTGATextureSimple("Models/grass.tga", &myTex2);
    glBindTexture(GL_TEXTURE_2D, myTex2);

    glActiveTexture(GL_TEXTURE7);
    LoadTGATextureSimple("Models/flames.tga", &maskrosTex);
    glBindTexture(GL_TEXTURE_2D, maskrosTex);

    glActiveTexture(GL_TEXTURE8);
    LoadTGATextureSimple("Models/tree.tga", &treeTex);
    glBindTexture(GL_TEXTURE_2D, treeTex);

    glActiveTexture(GL_TEXTURE9);
    LoadTGATextureSimple("Models/fire.tga", &fireTex);
    glBindTexture(GL_TEXTURE_2D, fireTex);

    glActiveTexture(GL_TEXTURE10);
    LoadTGATextureSimple("Models/fire2.tga", &fire2Tex);
    glBindTexture(GL_TEXTURE_2D, fire2Tex);

    glActiveTexture(GL_TEXTURE11);
    LoadTGATextureSimple("Models/tree_log/log_diffuse.tga", &logTex);
    glBindTexture(GL_TEXTURE_2D, logTex);

    glActiveTexture(GL_TEXTURE15);
    LoadTGATextureSimple("Models/Doorcolor.tga", &doorTex);
    glBindTexture(GL_TEXTURE_2D, doorTex);

    glActiveTexture(GL_TEXTURE12);
    // LoadTGATextureSimple("Models/wolf.tga", &wolfTex);
    // glBindTexture(GL_TEXTURE_2D, wolfTex);
    unsigned width, height;
    std::vector<unsigned char> image; // RGBA output
    lodepng::decode(image, width, height, "Models/wolf.png");
    //glGenTextures(1, &wolfTex);
    glBindTexture(GL_TEXTURE_2D, wolfTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    

    glActiveTexture(GL_TEXTURE13);

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
    shybox_shader = loadShaders("Shaders/skybox.vert", "Shaders/skybox.frag");
    shadow_shader = loadShaders("Shaders/shadow.vert", "Shaders/shadow.frag");
    object_shader = loadShaders("Shaders/object.vert", "Shaders/object.frag");
    tree_shader = loadShaders("Shaders/tree.vert", "Shaders/tree.frag");
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
    glUseProgram(object_shader);
    glUniformMatrix4fv(glGetUniformLocation(object_shader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
    uploadMat4ToShader(object_shader, "scaleBiasMatrix", scaleBiasMatrix);
    glUseProgram(shadow_shader);
    uploadMat4ToShader(shadow_shader, "projectionMatrix", shadowProjectionMatrix);
    uploadMat4ToShader(shadow_shader, "scaleBiasMatrix", scaleBiasMatrix);
    glUseProgram(tree_shader);
    glUniformMatrix4fv(glGetUniformLocation(tree_shader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);

    // upload fire
    glUseProgram(object_shader);
    glUniform3fv(glGetUniformLocation(object_shader, "firePos"), 1, &firePos.x);
    glUniform3fv(glGetUniformLocation(object_shader, "fireColor"), 1, &fireColor.x);
    glUniform3fv(glGetUniformLocation(object_shader, "moonPos"), 1, &moonPos.x);
    glUniform3fv(glGetUniformLocation(object_shader, "moonColor"), 1, &moonColor.x);
    
    // Start timer
    glutTimerFunc(20, &OnTimer, 0);
    initFireplaceSound();
    printError("init arrays");
}

bool inCabin(vec3 newCameraP){
    float minX = 0.f;
    float maxX = 40.f;
    float minZ = -35.f;
    float maxZ = 35.f;

    if(newCameraP.x < minX || newCameraP.x > maxX){
        return false;
    }
    if(newCameraP.z < minZ || newCameraP.z > maxZ){
        return false;
    }
    return true;
}

void moveCamera(){
    vec3 direction = normalize(worldCameraL - worldCameraP);
    vec3 side_dir = normalize(cross(vec3(0,1,0), direction));
    vec3 oldCameraP = worldCameraP;
    vec3 oldCameraL = worldCameraL;

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
    // go up and down
    if (glutKeyIsDown('j')) {
        worldCameraL -= vec3(0, 1, 0);
        worldCameraP -= vec3(0, 1, 0);
    }
    if (glutKeyIsDown('k')) {
        worldCameraL += vec3(0, 1, 0);
        worldCameraP += vec3(0, 1, 0);
    }
    // look up and down
    if (glutKeyIsDown('h')) {
        worldCameraL = normalize(ArbRotate(side_dir, M_PI/100)*direction) + worldCameraP;
    }
    if (glutKeyIsDown('l')) {
        worldCameraL = normalize(ArbRotate(side_dir, -M_PI/100)*direction) + worldCameraP;
    }

    if(!inCabin(worldCameraP)){
        worldCameraP = oldCameraP;
        worldCameraL = oldCameraL; 
    }
    worldCamera = lookAtv(worldCameraP, worldCameraL, worldCameraV);

    if (glutKeyIsDown('c')) {
        worldCamera = modelViewMatrix;
    }

}


void DrawCabin(GLuint shader){
    glActiveTexture(GL_TEXTURE2);
    glUniform1i(glGetUniformLocation(shader, "texUnit"), 2); 
    uploadMat4ToShader(shader, "model_To_World", cabinT);
	DrawModel(cabin, shader, "in_Position", "inNormal", "inTexCord");
    printError("DrawCabin");
}

void DrawDoor(GLuint shader){
    glActiveTexture(GL_TEXTURE15);
    glUniform1i(glGetUniformLocation(shader, "texUnit"), 15); 
	uploadMat4ToShader(shader, "model_To_World", doorT);
	DrawModel(door, shader, "in_Position", "inNormal", "inTexCord");
    printError("DrawDoor");
}

void DrawFireplace(GLuint shader){
    glActiveTexture(GL_TEXTURE3);
    glUniform1i(glGetUniformLocation(shader, "texUnit"), 3); 
    uploadMat4ToShader(shader, "model_To_World", FireplaceT);
	DrawModel(fireplace, shader, "in_Position", "inNormal", "inTexCord");
    printError("DrawFireplace");
}

void DrawLog(GLuint shader){
    glActiveTexture(GL_TEXTURE11);
    glUniform1i(glGetUniformLocation(shader, "texUnit"), 11); 
	uploadMat4ToShader(shader, "model_To_World", logT);
	DrawModel(tree_log, shader, "in_Position", "inNormal", "inTexCord");
    printError("DrawLog");
}

void DrawSofa(GLuint shader){
    glActiveTexture(GL_TEXTURE4);
    glUniform1i(glGetUniformLocation(shader, "texUnit"), 4); 
	uploadMat4ToShader(shader, "model_To_World", sofaT);
	DrawModel(sofa, shader, "in_Position", "inNormal", "inTexCord");
    printError("DrawSofa");
}

void DrawTable(GLuint shader){
    glActiveTexture(GL_TEXTURE2);
    glUniform1i(glGetUniformLocation(shader, "texUnit"), 2); 
	uploadMat4ToShader(shader, "model_To_World", tableT);
	DrawModel(table, shader, "in_Position", "inNormal", "inTexCord");
    printError("DrawTable");
}


void DrawSkyBox(){
    glUseProgram(shybox_shader);
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


void DrawGround(GLuint shader){
    //Draw ground
    glActiveTexture(GL_TEXTURE6);
    glUniform1i(glGetUniformLocation(shader, "texUnit"), 6); // Texture unit 6
    uploadMat4ToShader(shader, "world_To_View", worldCamera);
    uploadMat4ToShader(shader, "model_To_World", totalGround);
    DrawModel(ground, shader, "in_Position", "inNormal", "inTexCord");
    printError("DrawGround");
}

float randFloat() { 
    return (float)rand() / (float)RAND_MAX; 
}

float flicker(float time, float speed, float intensity) {
    static float lastValue = 0.5f;
    static float lastChangeTime = 0.0f;
    
    if (time - lastChangeTime > 0.05f) { // 0.05s tick
        lastValue = 0.5f + (randFloat() - 0.5f) * intensity;
        lastChangeTime = time;
    }
    return lastValue;
}

  
void DrawTree(){ 
    glDisable(GL_CULL_FACE);
    glUseProgram(tree_shader);
    glActiveTexture(GL_TEXTURE8);
    glUniform1i(glGetUniformLocation(tree_shader, "texUnit"), 8); 
    uploadMat4ToShader(tree_shader, "world_To_View", worldCamera);
    glUniform1f(glGetUniformLocation(tree_shader, "shade"), 0.5); // color of shadow

    mat4 translations[] = {T(0, 0, 0), T(150, 0, 0), T(0, 0, 150), T(0, 0, -150)};
    for (int j = 0; j < 4; j++) {
        for (int i = 0; i<FOREST_SIZE; i++) {
            mat4 rotation = Ry(0);
            if (i%2 == 0) {
                rotation = Ry(M_PI * (i+1) / 5);
            }
            mat4 t = treeMat[i] * translations[j];
            //tree 1    
            uploadMat4ToShader(tree_shader, "model_To_World", t*rotation);
            DrawModel(treeBillboard, tree_shader, "in_Position", "inNormal", "inTexCord");
            //tree 1.1
            uploadMat4ToShader(tree_shader, "model_To_World", t*rotation*Ry(M_PI_2));
            DrawModel(treeBillboard, tree_shader, "in_Position", "inNormal", "inTexCord"); 
        }
    }
    
    glEnable(GL_CULL_FACE);

    printError("DrawTree\n");
}

void DrawFire(){ 
    glDisable(GL_CULL_FACE);
    glUseProgram(tree_shader);
    uploadMat4ToShader(tree_shader, "world_To_View", worldCamera);
    glUniform1f(glGetUniformLocation(tree_shader, "shade"), 0.65); // color of shadow
    
    //fire 1  
    glActiveTexture(GL_TEXTURE9);
    glUniform1i(glGetUniformLocation(tree_shader, "texUnit"), 9); 
    uploadMat4ToShader(tree_shader, "model_To_World", fireT);
    DrawModel(treeBillboard, tree_shader, "in_Position", "inNormal", "inTexCord");   

    // //fire 2  
    uploadMat4ToShader(tree_shader, "model_To_World", fireT2);
    DrawModel(treeBillboard, tree_shader, "in_Position", "inNormal", "inTexCord");

    glEnable(GL_CULL_FACE);

    printError("DrawFire\n");
}

void DrawWolf(){ 
    glDisable(GL_CULL_FACE);
    glUseProgram(tree_shader);
    uploadMat4ToShader(tree_shader, "world_To_View", worldCamera);
    glUniform1f(glGetUniformLocation(tree_shader, "shade"), 0.5); // color of shadow
    
    glActiveTexture(GL_TEXTURE12);
    glUniform1i(glGetUniformLocation(tree_shader, "texUnit"), 12); 
    uploadMat4ToShader(tree_shader, "model_To_World", wolfT);
    DrawModel(treeBillboard, tree_shader, "in_Position", "inNormal", "inTexCord");   

    glEnable(GL_CULL_FACE);

    printError("DrawWolf\n");
}

void UpdateLightSources(){
    float fireJitterX = flicker(t, 10.0f, 0.5f); // horizontal
    float fireJitterY = flicker(t, 15.0f, 0.3f); // Vertical
    firePos = vec3(33 + fireJitterX, -1.0f + fireJitterY, 23);
    glUniform3fv(glGetUniformLocation(object_shader, "firePos"), 1, &firePos.x);
    

    float fireIntensity = 0.8f + 0.2f * flicker(t, 8.0f, 1.0f);
    fireColor = vec3(242.f/256, 125.f/256, 12.f/256) * fireIntensity;
    glUniform3fv(glGetUniformLocation(object_shader, "fireColor"), 1, &fireColor.x);

    float f  = randFloat() / 400.0;
    float f2 = randFloat() / 300.0;
    float f3 = sin(t/2) * sin(t/3) / 2.0;
    float f4 = sin(t/3) / 2.0;
    fireT =  T(fire_start_pos.x, fire_start_pos.y, fire_start_pos.z) * Ry(5*M_PI/4) * T(fire_start_pos.x*f, 0, 0) * T(f4, 0, 0) * S(0.1);
    fireT2 =  T(fire_start_pos.x -0.5, fire_start_pos.y, fire_start_pos.z -0.5) * Ry(5*M_PI/4) * T((fire_start_pos.x-1)*f2, 0, 0) * T(f3, 0, 0) * Ry(M_PI) * S(0.07);

    printError("UpdateLightSources");
}


void UpdateWolf() {
    mat4 start_position = T(150, 3.5, 1000);
    int speed = 200;
    int re_enter_speed = 2000;
    int pos = (int)(t*speed) % re_enter_speed;
    wolfT = start_position * T(0, 0, -pos) * Ry(M_PI_2*3) * Rx(M_PI) * S(0.4);

    printError("UpdateWolf");
}

void UpdateMoon() {
    if (moonPos.z >= 90) return;
    moonPos = moonPos + vec3(0, 0, 0.05);
}

void drawObjects(GLuint shader){
    glUseProgram(shader);
    DrawGround(shader);
    DrawCabin(shader);
    DrawFireplace(shader);
    DrawSofa(shader);
    DrawTable(shader);
    DrawLog(shader);
    DrawDoor(shader);
}


void fireShadow(){
    //Shadow things
    // Setup the modelview from the light source
    vec3 table_pos = vec3(20,0.0f,-10);
	modelViewMatrix = lookAt(firePos,
                            table_pos,
                            vec3( 0,1,0));
    

    mat4 lightViewProj = shadowProjectionMatrix * modelViewMatrix;
    glUseProgram(object_shader);

    uploadMat4ToShader(object_shader, "lightViewProjMatrix", lightViewProj);    

    glUseProgram(shadow_shader);
    uploadMat4ToShader(shadow_shader, "world_To_View", modelViewMatrix);
    uploadMat4ToShader(shadow_shader, "lightViewProjMatrix", lightViewProj);    
    glUniform1f(glGetUniformLocation(shadow_shader, "shade"), 0.3); // color of shadow

    // 1. Render scene to FBO
	useFBO(fbo, NULL, NULL);
	glViewport(0,0,WINDOW_SIZE,WINDOW_SIZE);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE); // Depth only
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Using the simple shader
	glUniform1i(glGetUniformLocation(shadow_shader, "textureUnit"),TEX_UNIT);
	glActiveTexture(GL_TEXTURE0 + TEX_UNIT);
	glBindTexture(GL_TEXTURE_2D,0);
    printError("1. Render scene to FBO");
	drawObjects(shadow_shader);

}

void moonShadow(){

    vec3 table_pos = vec3(20,8,-10);
    modelViewMatrix = lookAt(moonPos,
        table_pos,
        vec3( 0,1,0));

    mat4 shadowProjectionMatrixMoon = perspective(45, WINDOW_SIZE/WINDOW_SIZE, 10, 500);
    mat4 lightViewProj = shadowProjectionMatrixMoon * modelViewMatrix;
    glUseProgram(object_shader);

    uploadMat4ToShader(object_shader, "lightViewProjMatrixMoon", lightViewProj);    

    glUseProgram(shadow_shader);
    uploadMat4ToShader(shadow_shader, "world_To_View", modelViewMatrix);
    uploadMat4ToShader(shadow_shader, "lightViewProjMatrix", lightViewProj);    

    // 1. Render scene to FBO
    useFBO(moonFbo, NULL, NULL);
    glViewport(0,0,WINDOW_SIZE,WINDOW_SIZE);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE); // Depth only 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Using the simple shader
    glUniform1i(glGetUniformLocation(shadow_shader, "textureUnitMoon"),MOON_TEX_UNIT);
    glActiveTexture(GL_TEXTURE0 + MOON_TEX_UNIT);
    glBindTexture(GL_TEXTURE_2D,0); 
    printError("1. Render scene to FBO");
    drawObjects(shadow_shader);

}  
void display(void)
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
	useFBO(NULL, fbo, moonFbo);
	
    glViewport(0,0,WINDOW_SIZE,WINDOW_SIZE);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Using the projTex (object) shader
    glUseProgram(object_shader);

	//load both fbo depth maps to shader
	glUniform1i(glGetUniformLocation(object_shader, "textureUnit"),TEX_UNIT);
	glActiveTexture(GL_TEXTURE0 + TEX_UNIT);
	glBindTexture(GL_TEXTURE_2D,fbo->depth);

	glUniform1i(glGetUniformLocation(object_shader, "textureUnitMoon"),MOON_TEX_UNIT);
	glActiveTexture(GL_TEXTURE0 + MOON_TEX_UNIT);
	glBindTexture(GL_TEXTURE_2D,moonFbo->depth);

    moveCamera();

    uploadMat4ToShader(object_shader, "world_To_View", worldCamera);
    DrawSkyBox();

    drawObjects(object_shader);
    DrawTree();
    DrawFire();
    DrawWolf();

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

    glActiveTexture(GL_TEXTURE14);
    moonFbo = initFBO2(WINDOW_SIZE, WINDOW_SIZE, 0, 1);
   
	glutDisplayFunc(display); 

	glutMainLoop();
	return 0;
}


