// shadows.cpp
#include "shadows.h"

#include "lighting.h"
#include "scene.h"
#include "models.h"
#include "rendering.h"



void fireShadow() {
    // Setup the modelview from the light source
    modelViewMatrix = lookAt(firePos, fireLookAt, vec3(0,1,0));
    mat4 lightViewProj = shadowProjectionMatrixFire * modelViewMatrix;

    glUseProgram(object_shader);
    uploadMat4ToShader(object_shader, "lightViewProjMatrix", lightViewProj);

    glUseProgram(shadow_shader);
    uploadMat4ToShader(shadow_shader, "world_To_View", modelViewMatrix);
    uploadMat4ToShader(shadow_shader, "lightViewProjMatrix", lightViewProj);
    glUniform1f(glGetUniformLocation(shadow_shader, "shade"), 0.3);

    // Render scene to FBO
    useFBO(fireFbo, NULL, NULL);
    glViewport(0,0,WINDOW_WIDTH,WINDOW_HEIGHT);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUniform1i(glGetUniformLocation(shadow_shader, "textureUnit"),TEX_UNIT);
    glActiveTexture(GL_TEXTURE0 + TEX_UNIT);
    glBindTexture(GL_TEXTURE_2D,0);

    drawObjects(shadow_shader);
}

void moonShadow() {
    modelViewMatrix = lookAt(moonPos, moonLookAt, vec3(0,1,0));

    mat4 shadowProjectionMatrixMoon = perspective(45, WINDOW_WIDTH/WINDOW_HEIGHT, 10, 500);
    mat4 lightViewProj = shadowProjectionMatrixMoon * modelViewMatrix;

    glUseProgram(object_shader);
    uploadMat4ToShader(object_shader, "lightViewProjMatrixMoon", lightViewProj);

    glUseProgram(shadow_shader);
    uploadMat4ToShader(shadow_shader, "world_To_View", modelViewMatrix);
    uploadMat4ToShader(shadow_shader, "lightViewProjMatrix", lightViewProj);

    useFBO(moonFbo, NULL, NULL);
    glViewport(0,0,WINDOW_WIDTH,WINDOW_HEIGHT);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUniform1i(glGetUniformLocation(shadow_shader, "textureUnitMoon"),MOON_TEX_UNIT);
    glActiveTexture(GL_TEXTURE0 + MOON_TEX_UNIT);
    glBindTexture(GL_TEXTURE_2D,0);

    drawObjects(shadow_shader);
}