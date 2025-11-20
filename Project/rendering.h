#ifndef RENDERING_H
#define RENDERING_H

#include "GL_utilities.h"
#include "VectorUtils4.h"

// Rendering functions
void DrawCabin(GLuint shader);
void DrawDoor(GLuint shader);
void DrawFireplace(GLuint shader);
void DrawLog(GLuint shader);
void DrawSofa(GLuint shader);
void DrawTable(GLuint shader);
void DrawSkyBox();
void DrawGround(GLuint shader);
void DrawTree();
void DrawFire();
void DrawRoof(GLuint shader);
void DrawFloor(GLuint shader);

void DrawWolf();
void UpdateWolf();
void drawObjects(GLuint shader);

#endif