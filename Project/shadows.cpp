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

    // 3. DO NOT disable color writes
    // glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); // <-- REMOVE THIS LINE!

    // 4. Clear buffers
    glClearDepth(1.0);
    glClear(GL_DEPTH_BUFFER_BIT); // Clears the new GL_DEPTH_ATTACHMENT (renderbuffer)

    GLfloat clearColor[4] = { far_plane, far_plane, far_plane, 1.0f };
    glClearBufferfv(GL_COLOR, 0, clearColor); // Clears COLOR_ATTACHMENT0

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glUseProgram(shadow_cube_shader);

    float near_plane = 0.1f;
    mat4 shadowProj = perspective(90.0f, 1.0f, near_plane, far_plane);
    vec3 lightPos = firePos;
    mat4 shadowTransforms[6];

    // +X
    shadowTransforms[0] = shadowProj * lookAt(lightPos,
                                              lightPos + vec3(1.0, 0.0, 0.0),
                                              vec3(0.0, -1.0, 0.0));
    // -X
    shadowTransforms[1] = shadowProj * lookAt(lightPos,
                                              lightPos + vec3(-1.0, 0.0, 0.0),
                                              vec3(0.0, -1.0, 0.0));
    // +Y
    shadowTransforms[2] = shadowProj * lookAt(lightPos,
                                              lightPos + vec3(0.0, 1.0, 0.0),
                                              vec3(0.0, 0.0, 1.0));
    // -Y
    shadowTransforms[3] = shadowProj * lookAt(lightPos,
                                              lightPos + vec3(0.0, -1.0, 0.0),
                                              vec3(0.0, 0.0, -1.0));
    // +Z
    shadowTransforms[4] = shadowProj * lookAt(lightPos,
                                              lightPos + vec3(0.0, 0.0, 1.0),
                                              vec3(0.0, -1.0, 0.0));
    // -Z
    shadowTransforms[5] = shadowProj * lookAt(lightPos,
                                              lightPos + vec3(0.0, 0.0, -1.0),
                                              vec3(0.0, -1.0, 0.0));

    // 10. Upload transforms to shader
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

    // 11. Upload light position and far plane
    GLint lightPosLoc = glGetUniformLocation(shadow_cube_shader, "lightPos");
    if (lightPosLoc != -1) {
        glUniform3fv(lightPosLoc, 1, &lightPos.x);
    }

    GLint farPlaneLoc = glGetUniformLocation(shadow_cube_shader, "far_plane");
    if (farPlaneLoc != -1) {
        glUniform1f(farPlaneLoc, far_plane);
    }

    // 12. Render scene (geometry shader handles 6 faces)
    drawObjects(shadow_cube_shader);

    // 14. Restore state
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glBindFramebuffer(GL_FRAMEBUFFER, oldFBO);
    glViewport(oldViewport[0], oldViewport[1], oldViewport[2], oldViewport[3]);

    // 15. Bind depth cube map to main shader
    glUseProgram(object_shader);

    GLint shadowCubeMapLoc = glGetUniformLocation(object_shader, "shadowCubeMap");
    if (shadowCubeMapLoc != -1) {
        glUniform1i(shadowCubeMapLoc, TEX_UNIT);
        glActiveTexture(GL_TEXTURE0 + TEX_UNIT);
        glBindTexture(GL_TEXTURE_CUBE_MAP, shadowCubeFBO->depth);
    }

    // 16. Upload light position and far plane to main shader too
    GLint firePosLoc = glGetUniformLocation(object_shader, "firePos");
    if (firePosLoc != -1) {
        glUniform3fv(firePosLoc, 1, &lightPos.x);
    }

    GLint farPlaneLocObj = glGetUniformLocation(object_shader, "far_plane");
    if (farPlaneLocObj != -1) {
        glUniform1f(farPlaneLocObj, far_plane);
    }
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