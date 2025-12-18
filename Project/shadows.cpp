// shadows.cpp
#include "shadows.h"

#include "lighting.h"
#include "scene.h"
#include "models.h"
#include "rendering.h"
#define SHADOW_CUBE_SIZE WINDOW_HEIGHT


void renderCubeShadowMap() {
    // 1. Save current state
    GLint oldFBO, oldViewport[4];
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldFBO);
    glGetIntegerv(GL_VIEWPORT, oldViewport);

    // 2. Bind depth FBO
    glBindFramebuffer(GL_FRAMEBUFFER, shadowCubeFBO->fb);
    glViewport(0, 0, SHADOW_CUBE_SIZE, SHADOW_CUBE_SIZE);
    glClear(GL_DEPTH_BUFFER_BIT);
    glDisable(GL_CULL_FACE);
    glUseProgram(shadow_cube_shader);
    vec3 lightPos = firePos;

    shadowTransforms[0] = shadowProj * lookAt(lightPos, lightPos + vec3( 1.0, 0.0, 0.0), vec3(0.0,-1.0, 0.0));
    shadowTransforms[1] = shadowProj * lookAt(lightPos, lightPos + vec3(-1.0, 0.0, 0.0), vec3(0.0,-1.0, 0.0));
    shadowTransforms[2] = shadowProj * lookAt(lightPos, lightPos + vec3( 0.0, 1.0, 0.0), vec3(0.0, 0.0, 1.0));
    shadowTransforms[3] = shadowProj * lookAt(lightPos, lightPos + vec3( 0.0,-1.0, 0.0), vec3(0.0, 0.0,-1.0));
    shadowTransforms[4] = shadowProj * lookAt(lightPos, lightPos + vec3( 0.0, 0.0, 1.0), vec3(0.0,-1.0, 0.0));
    shadowTransforms[5] = shadowProj * lookAt(lightPos, lightPos + vec3( 0.0, 0.0,-1.0), vec3(0.0,-1.0, 0.0));

    for (int i = 0; i < 6; ++i) {
        char uniformName[32];
        sprintf(uniformName, "shadowTransforms[%d]", i);

        GLint loc = glGetUniformLocation(shadow_cube_shader, uniformName);
        if (loc != -1) {
            glUniformMatrix4fv(loc, 1, GL_TRUE, shadowTransforms[i].m);
        } else {
            printf("Warning: Uniform '%s' not found\n", uniformName);
        }
    }

    // Upload light position and far plane
    glUniform3fv(glGetUniformLocation(shadow_cube_shader, "lightPos"), 1, &lightPos.x);
    glUniform1f(glGetUniformLocation(shadow_cube_shader, "far_plane"), far_plane);

    // Render scene for shadow cube
    drawObjects(shadow_cube_shader);

    // Restore state
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glBindFramebuffer(GL_FRAMEBUFFER, oldFBO);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    // Bind depth cube map to main shader
    glUseProgram(object_shader);
    glUniform1i(glGetUniformLocation(object_shader, "shadowCubeMap"), TEX_UNIT);
    glActiveTexture(GL_TEXTURE0 + TEX_UNIT);
    glBindTexture(GL_TEXTURE_CUBE_MAP, shadowCubeFBO->depth);

    // Upload light position and far plane to main shader too
    glUniform3fv(glGetUniformLocation(object_shader, "firePos"), 1, &lightPos.x);
    glUniform1f(glGetUniformLocation(object_shader, "far_plane"), far_plane);

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