#include "models.h"

#include "MicroGlut.h"
#include "scene.h"
#include "BallAnimation.h"

// Model variables
Model *ground, *skybox, *sofa, *table, *cabin, *fireplace, *roof, *floorObj, *wind1, *wind2, *wind3, *wind4;
Model *treeBillboard, *tree_log, *door, *squareModel, *wolfObj;

mat4 totalGround, cabinT, FireplaceT, tableT, sofaT, roofT, floorT, window1T, window2T, window3T, window4T;
mat4 fireT, fireT2, logT, wolfT, doorT, newCabinT, wolfObjT;
vec3 fireStartPosition, cabinCenter;

float fireRotation;
const int FOREST_SIZE = 12;
mat4 treeMat[FOREST_SIZE];

// Square model data
GLfloat square[] = {-1,-1,0, -1,1,0, 1,1,0, 1,-1,0};
GLfloat squareTexCoord[] = {0,0, 0,1, 1,1, 1,0};
GLuint squareIndices[] = {0,1,2, 0,2,3};


unsigned int myTex;
unsigned int myTex2;
unsigned int cabinTex;
unsigned int sofatex;
unsigned int fireplacetex;
unsigned int maskrosTex;
unsigned int treeTex;
unsigned int fireTex;
unsigned int fire2Tex;
unsigned int logTex;
unsigned int wolfTex;
unsigned int doorTex;
unsigned int roofTex;
unsigned int windowTex;

// for moving model
mat4* modelT;
float currentTX;
float currentTY;
float currentTZ;
float currentRy;
float currentScale;

void InstantiateModels() {
    ground = LoadDataToModel(vertices, vertex_normals, tex_coords, vertex_normals, indices, 4, 6);
    treeBillboard = LoadDataToModel(tree_vertices, tree_vertex_normals, tree_tex_coords, tree_vertex_normals, tree_indices, 4, 6);
    skybox = LoadModel("skybox/skybox.obj");
    sofa = LoadModel("Models/sofa/model/SOFA.obj.obj");
    table = LoadModel("Models/Table.obj");
    fireplace = LoadModel("Models/fireplace_blender.obj");
    tree_log = LoadModel("Models/tree_log/low_poly_log.obj");
    door = LoadModel("Models/newdoor.obj");
    cabin = LoadModel("Models/maincottage.obj");
    floorObj = LoadModel("Models/floor.obj");
    roof = LoadModel("Models/roof.obj");
    wind1 = LoadModel("Models/window.obj");
    wolfObj = LoadModel("Models/Wolf.obj");

    printf("ball");
    ball(wolfObj);

    squareModel = LoadDataToModel(
            reinterpret_cast<vec3 *>(square), nullptr, reinterpret_cast<vec2 *>(squareTexCoord), nullptr,
            squareIndices, 4, 6);

    FireplaceT = T(35,-5,25) * Ry(-M_PI/2) * S(9);
    tableT = T(15.5, -15.5, 30.0) * Ry(0.000) * S(8.000);
    sofaT = T(-11.5, -8.5, 29.5) * Ry(1.571) * S(8.000);
    roofT = T(-15,2,10)*Ry(M_PI*3/2)* S(5.8);
    window1T = T(-52.5, 8.0, 17.5) * Ry(-0.000) * S(5.800);
    window2T = T(-52.5, 7.5, 48.5) * Ry(0.000) * S(5.800);

    window3T = T(50.0, 7.5, 46.5) * Ry(-3.142) * S(5.800);
    window4T = T(49.5, 8.0, 16.0) * Ry(-3.142) * S(5.800);
    wolfObjT = T(0, -3, 0)   * S(0.2);

    totalGround = T(0,-10,0);

    //fireplace
    FireplaceT = T(41.5, -8.0, 29.5) * Ry(-1.571) * S(9.000);
    fireT = T(40.5, -5.0, 29.0) * Ry(-1.571) * S(0.1);
    fireT2 = T(41.0, -5.0, 29.8) * Ry(-1.571) * S(0.8);
    logT = T(40.0, -7.0, 29.0) * Ry(-3.271)* S(0.025);
    fireStartPosition = vec3(40.5, -5.3, 29.0);
    fireRotation = atan2(fireT.m[2], fireT.m[0]);

    wolfT = T(150, 3.5, 0) * Ry(M_PI_2*3) * Rx(M_PI) * S(0.4);
    doorT = T(-5.0, -2.0, -3.5) * Ry(-1.571) * S(5.800);

    cabinT = T(-15,2,10)*Ry(M_PI*3/2)* S(5.8);

    cabinCenter = vec3(tableT.m[3], tableT.m[7], tableT.m[11]);

    newCabinT = T(-15,2,10)*Ry(M_PI*3/2)* S(5.8);
    floorT = T(-15,2,10)*Ry(M_PI*3/2)* S(5.8);



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


    modelT = &wolfObjT; // change to desired model
    currentScale = modelT->m[5];
    currentRy = atan2(modelT->m[2]* currentScale, modelT->m[0]* currentScale);
    currentTX = modelT->m[3];
    currentTY = modelT->m[7];
    currentTZ = modelT->m[11];
}

void moveFires() {
    //FireplaceT = T(41.5, -8.5, 27.5) * Ry(-M_PI/2) * S(9);
    fireT = T(currentTX, currentTY+2.7f, currentTZ) * Ry(currentRy) * S(0.1);
    fireT2 = T(currentTX - 0.5f, currentTY+2.7f, currentTZ + 0.8f) * Ry(currentRy) * S(0.1);
    logT = T(currentTX, currentTY+1.f, currentTZ) * Ry(currentRy-1.7f) * S(0.025);
}
void printFires() {
    printf("FireplaceT = T(%.1f, %.1f, %.1f) * Ry(%.3f) * S(%.3f)\n",currentTX, currentTY, currentTZ, currentRy, currentScale);
    printf("fireT = T(%.1f, %.1f, %.1f) * Ry(%.3f) \n", currentTX, currentTY+2.7f, currentTZ, currentRy);
    printf("fireT2 = T(%.1f, %.1f, %.1f) * Ry(%.3f) \n", currentTX - 0.5f, currentTY+2.7f, currentTZ + 0.8f, currentRy);
    printf("logT = T(%.1f, %.1f, %.1f) * Ry(%.3f) \n", currentTX, currentTY+1.f, currentTZ, currentRy-1.7f);
}

void MoveModel() {
    float moveSpeed = 0.5f;
    float rotateAngle = M_PI / 180.0f * 5.0f;;

    if (glutKeyIsDown(GLUT_KEY_LEFT)) {
        currentTX -= moveSpeed;
        *modelT = T(currentTX, currentTY, currentTZ) * Ry(currentRy) * S(currentScale);

    }
    if (glutKeyIsDown(GLUT_KEY_RIGHT)) {
        currentTX += moveSpeed;
        *modelT = T(currentTX, currentTY, currentTZ) * Ry(currentRy) * S(currentScale);

    }
    if (glutKeyIsDown(GLUT_KEY_UP)) {
        currentTZ -= moveSpeed;
        *modelT = T(currentTX, currentTY, currentTZ) * Ry(currentRy) * S(currentScale);

    }
    if (glutKeyIsDown(GLUT_KEY_DOWN)) {
        currentTZ += moveSpeed;
        *modelT = T(currentTX, currentTY, currentTZ) * Ry(currentRy) * S(currentScale);

    }
    // move up and down with 'i' and 'o' keys
    if (glutKeyIsDown('i')) {
        currentTY += moveSpeed;
        *modelT = T(currentTX, currentTY, currentTZ) * Ry(currentRy) * S(currentScale);

    }
    if (glutKeyIsDown('o')) {
        currentTY -= moveSpeed;
        *modelT = T(currentTX, currentTY, currentTZ) * Ry(currentRy) * S(currentScale);
    }

    // Rotation with 'r' and 't' keys
    if (glutKeyIsDown('r')) {
        currentRy -= rotateAngle;
        *modelT = T(currentTX, currentTY, currentTZ) * Ry(currentRy) * S(currentScale);

    }
    if (glutKeyIsDown('t')) {
        currentRy += rotateAngle;
        *modelT = T(currentTX, currentTY, currentTZ) * Ry(currentRy) * S(currentScale);

    }
    if (glutKeyIsDown('p')) {
        printf("= T(%.1f, %.1f, %.1f) * Ry(%.3f) * S(%.3f);\n",currentTX, currentTY, currentTZ, currentRy, currentScale);
        //printFires();

    }
    //moveFires();
}

void InstantiateTextures() {

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

    glActiveTexture(GL_TEXTURE20);
    LoadTGATextureSimple("Models/wall.tga", &cabinTex);
    glBindTexture(GL_TEXTURE_2D, cabinTex);

    glActiveTexture(GL_TEXTURE21);
    LoadTGATextureSimple("Models/roof.tga", &roofTex);
    glBindTexture(GL_TEXTURE_2D, roofTex);

    glActiveTexture(GL_TEXTURE22);
    LoadTGATextureSimple("Models/window.tga", &windowTex);
    glBindTexture(GL_TEXTURE_2D, windowTex);

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