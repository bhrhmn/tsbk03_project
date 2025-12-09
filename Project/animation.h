//
// Created by jorunn on 2025-12-09.
//

#ifndef SCENEPROJECT_ANIMATION_H
#define SCENEPROJECT_ANIMATION_H
#include "VectorUtils4.h"
#include "MicroGlut.h"

extern void openDoor();

bool keyCooldown();
bool lookAtDoor(vec3 newCameraP, vec3 direction);

#endif //SCENEPROJECT_ANIMATION_H