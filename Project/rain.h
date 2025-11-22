#ifndef RAIN_H
#define RAIN_H

#include "VectorUtils4.h"
#include "MicroGlut.h"
#include "LittleOBJLoader.h"

// variables


//  functions
void rain_init();
void rain();
void DrawModelInstanced(Model *m, GLuint program, const char* vertexVariableName, const char* normalVariableName, const char* texCoordVariableName, int count);

#endif