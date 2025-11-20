#include "models.h"
#include "scene.h"

// Model variables
Model *ground, *skybox, *sofa, *table, *cabin, *fireplace, *newCabin, *roof, *floor3;
Model *treeBillboard, *tree_log, *door, *squareModel;
mat4 totalGround, cabinT, FireplaceT, tableT, sofaT;
mat4 fireT, fireT2, logT, wolfT, doorT, newCabinT, floorT, roofT;
const int FOREST_SIZE = 12;
mat4 treeMat[FOREST_SIZE];

// Square model data
GLfloat square[] = {-1,-1,0, -1,1,0, 1,1,0, 1,-1,0};
GLfloat squareTexCoord[] = {0,0, 0,1, 1,1, 1,0};
GLuint squareIndices[] = {0,1,2, 0,2,3};


unsigned int myTex;
unsigned int myTex2;
unsigned int cabintex;
unsigned int newCabinTex;
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
    newCabin = LoadModel("Models/maincottage.obj");
    roof = LoadModel("Models/roof.obj");    
    floor3 = LoadModel("Models/floor.obj");

    squareModel = LoadDataToModel(
            (vec3 *)square, NULL, (vec2 *)squareTexCoord, NULL,
            squareIndices, 4, 6);

    cabinT = T(20,-10,0) * S(1);
    FireplaceT = T(35,-5,25) * Ry(5*M_PI/4) * S(9);
    tableT = T(20,-12,-10) * S(8);
    sofaT = T(20,-4,-30)* S(8);
    totalGround = T(0,-10,0);
    fireT = T(fire_start_pos.x, fire_start_pos.y, fire_start_pos.z) * Ry(5*M_PI/4) * S(0.1);
    fireT = T(fire_start_pos.x -0.5, fire_start_pos.y, fire_start_pos.z -0.5) * Ry(5*M_PI/4) * S(0.1);
    doorT = T(13,-2,40)*Ry(M_PI*3/2)* S(5.8);
    newCabinT = T(-15,2,10)*Ry(M_PI*3/2)* S(5.8);
    floorT = T(-15,2,10)*Ry(M_PI*3/2)* S(5.8);
    roofT = T(-15,2,10)*Ry(M_PI*3/2)* S(5.8);
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
    logT = T(fire_start_pos.x -1.0, -4, fire_start_pos.z -1.0) * Ry(5*M_PI/4) * Ry(M_PI_2) * S(0.025);
    wolfT = T(150, 3.5, 0) * Ry(M_PI_2*3) * Rx(M_PI) * S(0.4);
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

    glActiveTexture(GL_TEXTURE20);
    LoadTGATextureSimple("Models/wall.tga", &newCabinTex);
    glBindTexture(GL_TEXTURE_2D, newCabinTex);

    glActiveTexture(GL_TEXTURE21);
    LoadTGATextureSimple("Models/roof.tga", &roofTex);
    glBindTexture(GL_TEXTURE_2D, roofTex);

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