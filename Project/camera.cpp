#include "camera.h"
#include "scene.h"
// Camera variables
vec3 worldCameraP = { 25.0f, 8.0f, 0.0f };
vec3 worldCameraL = { 30.0f, 8.0f, 25.0f };
vec3 worldCameraV = { 0.0f, 5.0f, 0.0f };
mat4 worldCamera;
bool mouseCaptured = false;

bool inCabin(vec3 newCameraP){
    return true; 
    float minX = 0.f, maxX = 40.f;
    float minZ = -35.f, maxZ = 35.f;
    return !(newCameraP.x < minX || newCameraP.x > maxX || 
             newCameraP.z < minZ || newCameraP.z > maxZ);
}

void mouseMotion(int x, int y) {
    static bool ignoreNextEvent = false;

    if (!mouseCaptured) {
        ignoreNextEvent = false;
        return;
    }

    if (ignoreNextEvent) {
        ignoreNextEvent = false;
        return;
    }

    float dx = static_cast<float>(x) - WINDOW_SIZE/2.f;
    float dy = WINDOW_SIZE/2.f - static_cast<float>(y);

    if (dx != 0 || dy != 0) {
        // Mouse rotation code here
        
        glutWarpPointer(WINDOW_SIZE/2.f, WINDOW_SIZE/2.f);
        ignoreNextEvent = true;
    }
}

void mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        mouseCaptured = !mouseCaptured;
        if (mouseCaptured) {
            glutWarpPointer(WINDOW_SIZE/2, WINDOW_SIZE/2);
        }
    }
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
        //worldCamera = modelViewMatrix;
    }

}