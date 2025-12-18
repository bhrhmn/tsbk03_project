#include "rendering.h"
#include "models.h"
#include "camera.h"
#include "scene.h"
#include "BallAnimation.h"


void DrawCabin(GLuint shader){
    glActiveTexture(GL_TEXTURE20);
    glUniform1i(glGetUniformLocation(shader, "texUnit"), 20);
    uploadMat4ToShader(shader, "model_To_World", cabinT);
	DrawModel(cabin, shader, "in_Position", "inNormal", "inTexCord");
    printError("DrawCabin");
}


void DrawWindow(GLuint shader, mat4 windowTranslation, Model *window)
{
    glActiveTexture(GL_TEXTURE22);
    glUniform1i(glGetUniformLocation(shader, "texUnit"), 22);
    uploadMat4ToShader(shader, "model_To_World", windowTranslation);
    DrawModel(window, shader, "in_Position", "inNormal", "inTexCord");
    printError("DrawCabin");
}

void DrawWolfObj(GLuint shader)
{
    glActiveTexture(GL_TEXTURE22);
    glUniform1i(glGetUniformLocation(shader, "texUnit"), 22);
    uploadMat4ToShader(shader, "model_To_World", wolfObjT);
    DrawModel(wolfObj, shader, "in_Position", "inNormal", "inTexCord");
    printError("DrawCabin");
}


void DrawFloor(GLuint shader)
{
    glActiveTexture(GL_TEXTURE21);
    glUniform1i(glGetUniformLocation(shader, "texUnit"), 21);
    uploadMat4ToShader(shader, "model_To_World", floorT);
    DrawModel(floorObj, shader, "in_Position", "inNormal", "inTexCord");
    printError("DrawCabin");
}


void DrawRoof(GLuint shader)
{
    glActiveTexture(GL_TEXTURE21);
    glUniform1i(glGetUniformLocation(shader, "texUnit"), 21);
    uploadMat4ToShader(shader, "model_To_World", roofT);
    DrawModel(roof, shader, "in_Position", "inNormal", "inTexCord");
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

void DrawTree(){
    glDisable(GL_CULL_FACE);
    glUseProgram(tree_shader);
    glActiveTexture(GL_TEXTURE8);
    glUniform1i(glGetUniformLocation(tree_shader, "texUnit"), 8);
    uploadMat4ToShader(tree_shader, "world_To_View", worldCamera);
    vec4 shade = vec4(0.5, 0.5, 0.5, 1.0);
    glUniform4fv(glGetUniformLocation(tree_shader, "shade"), 1, &shade.x); // color of shadow

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
    vec4 shade = vec4(1.4, 1.2, 1.0, 1.0);
    glUniform4fv(glGetUniformLocation(tree_shader, "shade"), 1, &shade.x); // color of shadow

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
    vec4 shade = vec4(0.5, 0.5, 0.5, 1.0);
    glUniform4fv(glGetUniformLocation(tree_shader, "shade"), 1, &shade.x); // color of shadow

    glActiveTexture(GL_TEXTURE12);
    glUniform1i(glGetUniformLocation(tree_shader, "texUnit"), 12);
    uploadMat4ToShader(tree_shader, "model_To_World", wolfT);
    DrawModel(treeBillboard, tree_shader, "in_Position", "inNormal", "inTexCord");

    glEnable(GL_CULL_FACE);

    printError("DrawWolf\n");
}
void UpdateWolf() {
    mat4 start_position = T(150, 3.5, 1000);
    int speed = 200;
    int re_enter_speed = 2000;
    int pos = (int)(t*speed) % re_enter_speed;
    wolfT = start_position * T(0, 0, -pos) * Ry(M_PI_2*3) * Rx(M_PI) * S(0.4);
    wolfObjT = start_position * T(3, -13, -pos) * Ry(M_PI_2*2) * S(0.2);
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
    DrawFloor(shader);
    DrawRoof(shader);
    DrawWindow(shader, window1T ,wind1 );
    DrawWindow(shader, window2T ,wind1 );
    DrawWindow(shader, window3T ,wind1 );
    DrawWindow(shader, window4T ,wind1 );
    DrawWolfObj(shader);

    animateObj(shader);
}