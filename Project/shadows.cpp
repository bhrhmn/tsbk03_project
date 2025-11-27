// shadows.cpp
#include "shadows.h"

#include "lighting.h"
#include "scene.h"
#include "models.h"
#include "rendering.h"



void renderCubeShadowMap() {
    // 1. Setup cube shadow FBO (replaces useFBO)

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport); // Save current viewport

    glBindFramebuffer(GL_FRAMEBUFFER, shadowCubeFBO->fb);
    glViewport(0, 0, WINDOW_HEIGHT, WINDOW_HEIGHT);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); // No color output at all
    glClear(GL_DEPTH_BUFFER_BIT);

    // 2. Use the new depth cube shader (replaces shadow_shader)
    glUseProgram(shadow_cube_shader);


    vec3 lightPos = firePos;

    // Generate 6 view-projection matrices for cube faces
    shadowTransforms[0] = shadowProj * lookAt(lightPos, lightPos + vec3(1.0, 0.0, 0.0), vec3(0.0, -1.0, 0.0));
    shadowTransforms[1] = shadowProj * lookAt(lightPos, lightPos + vec3(-1.0, 0.0, 0.0), vec3(0.0, -1.0, 0.0));
    shadowTransforms[2] = shadowProj * lookAt(lightPos, lightPos + vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 1.0));
    shadowTransforms[3] = shadowProj * lookAt(lightPos, lightPos + vec3(0.0, -1.0, 0.0), vec3(0.0, 0.0, -1.0));
    shadowTransforms[4] = shadowProj * lookAt(lightPos, lightPos + vec3(0.0, 0.0, 1.0), vec3(0.0, -1.0, 0.0));
    shadowTransforms[5] = shadowProj * lookAt(lightPos, lightPos + vec3(0.0, 0.0, -1.0), vec3(0.0, -1.0, 0.0));

    // Upload all 6 transforms to shader
    for (int i = 0; i < 6; ++i) {
        char uniformName[32];
        sprintf(uniformName, "shadowTransforms[%d]", i);
        uploadMat4ToShader(shadow_cube_shader, uniformName, shadowTransforms[i]);
    }

    // Upload light position and far plane
    glUniform3fv(glGetUniformLocation(shadow_cube_shader, "lightPos"), 1, &lightPos.x);
    glUniform1f(glGetUniformLocation(shadow_cube_shader, "far_plane"), farFire);

    // 4. Render scene once (geometry shader handles 6 faces)
    drawObjects(shadow_cube_shader);

    // 5. Restore state (same as old method)
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // 6. Update main shader with cube shadow map
    glUseProgram(object_shader);
    glUniform1i(glGetUniformLocation(object_shader, "shadowCubeMap"), TEX_UNIT);
    glActiveTexture(GL_TEXTURE0 + TEX_UNIT);
    glBindTexture(GL_TEXTURE_CUBE_MAP, shadowCubeFBO->depth);

    glUniform3fv(glGetUniformLocation(object_shader, "firePos"), 1, &lightPos.x);
    glUniform1f(glGetUniformLocation(object_shader, "far_plane"), farFire);

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]); // Restore original
}


void fireShadow() {
    return;
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