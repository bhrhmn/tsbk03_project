#ifndef CAMERA_H
#define CAMERA_H

#include "VectorUtils4.h"
#include "MicroGlut.h"

// Camera variables
extern vec3 worldCameraP;
extern vec3 worldCameraL;
extern vec3 worldCameraV;
extern mat4 worldCamera;
extern bool mouseCaptured;

// Camera functions
void moveCamera();
void mouseMotion(int x, int y);
void mouseClick(int button, int state, int x, int y);
bool inCabin(vec3 newCameraP);

#endif