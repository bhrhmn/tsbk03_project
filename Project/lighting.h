#ifndef LIGHTING_H
#define LIGHTING_H

#include "VectorUtils4.h"

// Lighting function declarations
void UpdateLightSources();
void UpdateMoon();
float flicker(float time, float speed, float intensity);
float randFloat();
void blooming();

#endif