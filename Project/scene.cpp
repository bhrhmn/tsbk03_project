
#include "scene.h"

// Global variable definitions (declared extern in header)
Model *ground;
Model *skybox;
Model *sofa;
Model *table;
Model *cabin;
Model *fireplace;
Model *door;


unsigned int myTex;
unsigned int myTex2;
unsigned int cabintex;
unsigned int sofatex;
unsigned int fireplacetex;


mat4 totalGround;
mat4 worldCamera;
mat4 cabinT;
mat4 FireplaceT;
mat4 tableT;
mat4 sofaT;
mat4 doorT;

vec3 worldCameraP = { 0.0f, 5.0f, 25.0f };
vec3 worldCameraL = { 0.0f, 5.0f, 0.0f };
vec3 worldCameraV = { 0.0f, 5.0f, 0.0f };

GLuint ground_shader;
GLuint shybox_shader;
GLuint object_shader;
GLuint shadow_shader;


vec3 firePos = vec3(33, 0, 23);
vec3 fireColor = vec3(0.8, 0.5, 0.2);


vec3 moonPos = vec3(100.f, 100.f, 70.f);
vec3 moonColor = vec3(0.8, 0.8, 0.9);

GLfloat t = 0;

//Define depthMap
unsigned int depthMapFBO;
unsigned int RENDER_WIDTH = 1024;
unsigned int SHADOW_MAP_RATIO = 1;
unsigned int RENDER_HEIGHT = 1024;

GLuint fboId;
GLuint depthTextureId;

//Light lookAt
float moonLookAt[3] = {0.0f,0.0f,0.0f};

//GLuint shadowMapUniform;

void UpdateLightSources(){
    glUseProgram(object_shader);

    printError("UpdateLightSources");

    firePos = vec3(33+sin(t*10)*2, 0, 23);
    glUniform3fv(glGetUniformLocation(object_shader, "firePos"), 1, &firePos.x);
}

void DrawCabin(){
    glActiveTexture(GL_TEXTURE2);
    glUniform1i(glGetUniformLocation(object_shader, "texUnit"), 2); // Texture unit 0
    uploadMat4ToShader(object_shader, "model_To_World", cabinT);
	DrawModel(cabin, object_shader, "in_Position", "inNormal", "inTexCord");
    printError("DrawCabin");
}

void DrawDoor(){
    glActiveTexture(GL_TEXTURE2);
    glUniform1i(glGetUniformLocation(object_shader, "texUnit"), 2); // Texture unit 0
    uploadMat4ToShader(object_shader, "model_To_World", doorT);
	DrawModel(door, object_shader, "in_Position", "inNormal", "inTexCord");
    printError("DrawCabin");
}

void DrawFireplace(){
    glActiveTexture(GL_TEXTURE3);
    glUniform1i(glGetUniformLocation(object_shader, "texUnit"), 3); 
    uploadMat4ToShader(object_shader, "model_To_World", FireplaceT);
	DrawModel(fireplace, object_shader, "in_Position", "inNormal", "inTexCord");
    printError("DrawFireplace");
}

void DrawTable(){
    glActiveTexture(GL_TEXTURE2);
    glUniform1i(glGetUniformLocation(object_shader, "texUnit"), 2); // Texture unit 0
	uploadMat4ToShader(object_shader, "model_To_World", tableT);
	DrawModel(table, object_shader, "in_Position", "inNormal", "inTexCord");
    printError("DrawTable");
}

void DrawSofa(){
    glActiveTexture(GL_TEXTURE4);
    glUniform1i(glGetUniformLocation(object_shader, "texUnit"), 4); 
	uploadMat4ToShader(object_shader, "model_To_World", sofaT);
	DrawModel(sofa, object_shader, "in_Position", "inNormal", "inTexCord");
    printError("DrawSofa");
}

void DrawSkyBox(){
    glUseProgram(shybox_shader);
    vec3 newWorldCamera = worldCameraP;
    worldCamera = lookAtv(newWorldCamera, worldCameraL, worldCameraV);
    glDisable(GL_DEPTH_TEST);

    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shybox_shader, "texUnit"), 0);
    
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

void drawObjects(){
    //draw using object shader
    printError("DrawError");

	
    
    DrawCabin();
    DrawFireplace();
    DrawSofa();
    DrawTable();
    DrawDoor();
    //DrawGround();
}

void drawObjectsShadows(){
    //draw using object shader
    printError("DrawError");
    glDisable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE7);

    uploadMat4ToShader(shadow_shader, "model_To_World", cabinT);
	DrawModel(cabin, shadow_shader, "in_Position", "inNormal", "inTexCord");

    // uploadMat4ToShader(shadow_shader, "model_To_World", FireplaceT);
	// DrawModel(fireplace, shadow_shader, "in_Position", "inNormal", "inTexCord");

    // uploadMat4ToShader(shadow_shader, "model_To_World", sofaT);
	// DrawModel(sofa, shadow_shader, "in_Position", "inNormal", "inTexCord");
    
    // uploadMat4ToShader(shadow_shader, "model_To_World", tableT);
	// DrawModel(table, shadow_shader, "in_Position", "inNormal", "inTexCord");
    
    // uploadMat4ToShader(shadow_shader, "model_To_World", doorT);
	// DrawModel(door, shadow_shader, "in_Position", "inNormal", "inTexCord");

    uploadMat4ToShader(shadow_shader, "model_To_World", totalGround);
    DrawModel(ground, shadow_shader, "in_Position", "inNormal", "inTexCord");
    printError("renderscene");

}



void setupShadowMatrices(float position_x,float position_y,float position_z,float lookAt_x,float lookAt_y,float lookAt_z)
{
    float far2 = far;
    float near2 = near; 
    float fov = M_PI / 4;
    float top2 = near2 / tanf(fov / 2.0f);
    float bottom2 = -top2;
    float right2 = top2 * RENDER_WIDTH / RENDER_HEIGHT;
    float left2 = -right2;

    const GLfloat projectionMatrix2[] = {2.0f*near2/(right2-left2), 0.0f, 0.0f, 0.0f,
                                            0.0f, 2.0f*near2/(top2-bottom2), 0.0f, 0.0f,
                                            (right2+left2)/(right2-left2), (top2+bottom2)/(top2-bottom2), -(far2 + near2)/(far2 - near2), -1.0f,
                                            0.0f, 0.0f, -2.0f*far2*near2/(far2-near2), 0.0f };
    glUseProgram(shadow_shader);
    glUniformMatrix4fv(glGetUniformLocation(shadow_shader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix2);
    mat4 l = lookAt(position_x,position_y,position_z,lookAt_x,lookAt_y,lookAt_z,0,1,0);
    uploadMat4ToShader(shadow_shader, "world_To_View", l);
}

void setupObjectMatrices()
{
    glUseProgram(object_shader);
    uploadMat4ToShader(object_shader, "world_To_View", worldCamera);
    glUniformMatrix4fv(glGetUniformLocation(object_shader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
}



void ShadowMap(){ 
    int shadowMapWidth = RENDER_WIDTH * SHADOW_MAP_RATIO;
    int shadowMapHeight = RENDER_HEIGHT * SHADOW_MAP_RATIO;
    GLenum FBOstatus;
    // Try to use a texture depth component

    glGenTextures(1, &depthTextureId);
    glBindTexture(GL_TEXTURE_2D, depthTextureId);

    
    // GL_LINEAR does not make sense for depth texture.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    printError("bindtexture");

    glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    // create a framebuffer object
    glGenFramebuffers(1, &fboId);
    glBindFramebuffer(GL_FRAMEBUFFER, fboId);
    
    // Instruct OpenGL that we won't bind a color texture to the FBO
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    
    // attach the texture to FBO depth attachment point
    glFramebufferTexture2D(GL_FRAMEBUFFER,
        GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D, depthTextureId, 0);
    
        // check FBO status
    FBOstatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(FBOstatus != GL_FRAMEBUFFER_COMPLETE)
        printf("GL_FRAMEBUFFER_COMPLETE failed, CANNOT use FBO\n");
    
        // switch back to window-system-provided framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}





void setTextureMatrix(void)
	{
		static double modelView[16];
		static double projection[16];
		
		// Moving from unit cube [-1,1] to [0,1]  
		const GLdouble bias[16] = {	
			0.5, 0.0, 0.0, 0.0, 
			0.0, 0.5, 0.0, 0.0,
			0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0};
		
		// Grab modelview and transformation matrices
		glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
		glGetDoublev(GL_PROJECTION_MATRIX, projection);
		
		
		//glMatrixMode(GL_TEXTURE);
		glActiveTextureARB(GL_TEXTURE7);
		
		glLoadIdentity();	
		glLoadMatrixd(bias);
		
		// concatating all matrices into one.
		glMultMatrixd (projection);
		glMultMatrixd (modelView);
		
		// Go back to normal matrix mode
		glMatrixMode(GL_MODELVIEW);
	}		

    void renderScene(void)
    {

        //1. First step: Render from the light POV to a FBO, depth values only
        // Bind the depth FBO
        
        glBindFramebuffer(GL_FRAMEBUFFER,fboId);//Rendering offscreen
        
        //Using the fixed pipeline to render to the depthbuffer
        // TODO: create simple shader, doesn't matter what it draws in fragment shader
        glUseProgram(simpleShader);

        
        // In the case we render the shadowmap to a higher resolution, the viewport must be modified accordingly.
        glViewport(0,0,RENDER_WIDTH * SHADOW_MAP_RATIO,RENDER_HEIGHT*SHADOW_MAP_RATIO);

        // Clear previous frame values
        glClear( GL_DEPTH_BUFFER_BIT);

        //Disable color rendering, we only want to write to the Z-Buffer
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

        // Setup the projection and modelview from the light source
        setupShadowMatrices(moonPos.x, moonPos.y, moonPos.z, moonLookAt[0], moonLookAt[1], moonLookAt[2]);

        // Cull front, render only backface, to avoid self-shadowing
        glCullFace(GL_FRONT);

        drawObjects();
        
        // Save modelview/projection matrice into the texture matrix
        // and add the scale and bias transformation
        //setTextureMatrix();
        
        //==================================
        //2. Render from camera with the Z-buffer FBO
        glBindFramebuffer(GL_FRAMEBUFFER,0);
        glViewport(0,0,RENDER_WIDTH,RENDER_HEIGHT);
        
        //Enabling color write (previously disabled for z-buffer rendering)
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        
        // Clear previous frame values
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        //Using the shadow shader
        // TODO: add logic in shadow shader to object_shader
        //       shadowCoord = surfacePos typ
        glUseProgram(object_shader);

        glUniform1i(glGetUniformLocation(shadow_shader, "shadowMap"), 7); // Texture unit 0

        //glUniform1i("shadowMap", GL_TEXTURE7);
        //glUniform1i(texunit,TEX_UNIT);
        glActiveTexture(GL_TEXTURE7);
        glBindTexture(GL_TEXTURE_2D, depthTextureId);
        
        // Setup the projection and modelview from the camera
        setupObjectMatrices();
        glCullFace(GL_BACK);
        drawObjects();
        glutSwapBuffers();
    }

// Function implementations
void InstantiateModels() {
    ground = LoadDataToModel(vertices, vertex_normals, tex_coords, vertex_normals, indices, 4, 6);
    skybox = LoadModel("skybox/skybox.obj");
    sofa = LoadModel("Models/sofa/model/SOFA.obj.obj");
    table = LoadModel("Models/Table.obj");
    cabin = LoadModel("Models/WoodenCabinObj.obj");
    fireplace = LoadModel("Models/fireplace_blender.obj");
    door = LoadModel("Models/door.obj");

    
    cabinT = T(20,-10,0) * S(1);
    FireplaceT = T(35,-5,25) * Ry(5*M_PI/4) * S(9);
    tableT = T(20,-12,-10) * S(8);
    sofaT = T(20,-4,-30)* S(8);
    doorT = T(13,-5,27)*Ry(M_PI/2)* S(6);

    

    totalGround = T(0,-10,0);
}

void InstantiateTextures() {
    glActiveTexture(GL_TEXTURE0);
    LoadTGATextureSimple("skybox-alt/atmosphere-cloud.tga", &myTex);
    glBindTexture(GL_TEXTURE_2D, myTex);

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
    
}

void OnTimer(int value) {
    glutPostRedisplay();
    glutTimerFunc(20, &OnTimer, value);
}

void init(void) {
    dumpInfo();

    // GL inits
    glClearColor(0.8,0.8,0.8,0);
    glDisable(GL_CULL_FACE);

    // Load and compile shader
    ground_shader = loadShaders("Shaders/ground.vert", "Shaders/ground.frag");
    shybox_shader = loadShaders("Shaders/skybox.vert", "Shaders/skybox.frag");
    object_shader = loadShaders("Shaders/object.vert", "Shaders/object.frag");
    shadow_shader = loadShaders("Shaders/shadow.vert", "Shaders/shadow.frag");
    
    printError("init shader");
    
    // Textures
    InstantiateTextures();
    printError("Init Textures");
    
    // Models
    InstantiateModels();
    printError("Init Models");
    
    // Upload Projection Matrix Once to each shader
    glUseProgram(shybox_shader);
    glUniformMatrix4fv(glGetUniformLocation(shybox_shader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
    glUseProgram(ground_shader);
    glUniformMatrix4fv(glGetUniformLocation(ground_shader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
    glUseProgram(object_shader);
    glUniformMatrix4fv(glGetUniformLocation(object_shader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);

    // upload fire
    glUniform3fv(glGetUniformLocation(object_shader, "firePos"), 1, &firePos.x);
    glUniform3fv(glGetUniformLocation(object_shader, "fireColor"), 1, &fireColor.x);

    //Upload moon
    glUniform3fv(glGetUniformLocation(object_shader, "moonPos"), 1, &moonPos.x);
    glUniform3fv(glGetUniformLocation(object_shader, "moonColor"), 1, &moonColor.x);
    
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




void display(void)
{
	printError("pre display");

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    t = (GLfloat)glutGet(GLUT_ELAPSED_TIME)/1000;
    
    moveCamera();
    DrawSkyBox();
    //DrawGround();
	
    renderScene();

    UpdateLightSources();

	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitContextVersion(3, 2);
	glutInitWindowSize(WINDOW_SIZE, WINDOW_SIZE);
	glutCreateWindow ("Cosy Cabin");
    ShadowMap();
    printError("Under shadow map");
    glEnable(GL_DEPTH_TEST);
    glClearColor(0, 0, 0, 1);
	glutDisplayFunc(display); 
	init();
	glutMainLoop();
	return 0;
}


