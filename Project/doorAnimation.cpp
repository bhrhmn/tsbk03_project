//
// Created by jorun on 2025-12-09.
//

#include "doorAnimation.h"
#include "camera.h"
#include "models.h"

bool doorIsOpen = false;
float lastKeyPressed = glutGet(GLUT_ELAPSED_TIME) * 0.001f;

void openDoor()
{
    vec3 direction = normalize(worldCameraL - worldCameraP);
    if (keyCooldown()){
        if (glutKeyIsDown('b') && lookAtDoor(worldCameraP, direction) && !doorIsOpen)
        {
            printf("openDoor");
            doorT = doorT * Ry(1.5);
            doorIsOpen = true;
        }
        else if (glutKeyIsDown('b') && lookAtDoor(worldCameraP, direction) && doorIsOpen)
        {
            printf("closeDoor");
            doorT = doorT * Ry(-1.5);
            doorIsOpen = false;
        }
    }
}

bool lookAtDoor(vec3 newCameraP, vec3 direction)
{
    //Possibly change to check if the door is in frustum.
    vec3 doorTVec =  vec3(doorT.m[3], doorT.m[7], doorT.m[11]);
    vec3 toCamera = normalize(doorTVec - newCameraP);
    float dotCamera = dot(direction, toCamera);
    if (dotCamera > 0.8f)
    {
        return true;
    }
    return false;
}


bool keyCooldown(){
    float now = glutGet(GLUT_ELAPSED_TIME) * 0.001f;
    if (now  - lastKeyPressed > 0.1f){
        lastKeyPressed = now;
        return true;
    }
    return false;
}
