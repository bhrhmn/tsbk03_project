#ifndef SHADOWS_H
#define SHADOWS_H

#include "scene.h"
//#include "VectorUtils4.h"
#define TEX_UNIT 0
#define MOON_TEX_UNIT 1
inline unsigned int depthCubeMap;

inline float aspect = WINDOW_HEIGHT / WINDOW_HEIGHT;
inline float const nearFire = 0.1f;
inline float const far_plane = 500.0f;
inline mat4 shadowProj = perspective(90.0f, 1.f, nearFire, far_plane);
inline mat4 shadowTransforms[6];


// Shadow function declarations
void fireShadow();
void renderCubeShadowMap();
void moonShadow();
void initShadow();

#endif