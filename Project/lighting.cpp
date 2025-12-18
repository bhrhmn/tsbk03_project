// lighting.cpp
#include "lighting.h"
#include "scene.h"
#include "models.h"
#include "shadows.h"

Model *fireModel;
Model *fireModel2;

vec3 firePos;
vec3 fireColor = vec3(2.5f, 2.7f, 2.4f);
vec3 fireLookAt;

vec3 moonPos = vec3(120.0f, 100.0f, -120.f);
vec3 moonColor = vec3(0.8f, 0.8f, 1.0f);
vec3 moonLookAt;

float randFloat() {
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

float flicker(const float time, const float intensity) {
    static float lastValue = 0.5f;
    static float lastChangeTime = 0.0f;

    if (time - lastChangeTime > 0.05f) {
        lastValue = 0.5f + (randFloat() - 0.5f) * intensity;
        lastChangeTime = time;
    }
    return lastValue;
}
void UpdateLightSources() {
    const float fireJitterX = flicker(t, 0.1f);
    const float fireJitterY = flicker(t, 0.1f);

    const float f4 = sin(t/3) / 10.0f;

    fireT = T(fireStartPosition.x + fireJitterX + f4 +1,fireStartPosition.y + fireJitterY - 3, fireStartPosition.z -1) * Ry(fireRotation+ M_PI/2) * S(0.1f);
    fireT2 = T(fireStartPosition.x - fireJitterX - f4+1,fireStartPosition.y + fireJitterY - 3 ,fireStartPosition.z -1) * Ry(fireRotation) * S(0.1f);
    firePos = vec3(fireStartPosition.x + fireJitterX, fireStartPosition.y + fireJitterY, fireStartPosition.z);
    float fireIntensity = 1.5f + flicker(t, 1.0f);
    fireColor = vec3(242.f/256, 125.f/256, 12.f/256) * fireIntensity;

    glUniform3fv(glGetUniformLocation(object_shader, "firePos"), 1, &firePos.x);
    glUniform3fv(glGetUniformLocation(shadow_cube_shader, "lightPos"), 1, &firePos.x);
    glUniform3fv(glGetUniformLocation(object_shader, "fireColor"), 1, &fireColor.x);
    UpdateMoon();
}
void initLighting() {
    firePos = vec3(fireStartPosition.x, fireStartPosition.y, fireStartPosition.z);
    moonLookAt = cabinCenter;
    glUniform3fv(glGetUniformLocation(object_shader, "firePos"), 1, &firePos.x);
    glUniform3fv(glGetUniformLocation(object_shader, "fireColor"), 1, &fireColor.x);
    glUniform3fv(glGetUniformLocation(object_shader, "moonPos"), 1, &moonPos.x);
    glUniform3fv(glGetUniformLocation(object_shader, "moonColor"), 1, &moonColor.x);

}
void UpdateMoon() {
    if (moonPos.z >= 90) return;
    moonPos = moonPos + vec3(0, 0, 0.05);
}

void blooming()
{
    // hdr
    useFBO(overFlowFbo, bloomFbo, nullptr);
    glUseProgram(overflow_shader);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    DrawModel(squareModel, overflow_shader, "in_Position", nullptr, "in_TexCoord");

    glUseProgram(lowpass_shader);
    glUniform1f(glGetUniformLocation(lowpass_shader, "windowWidth"), 1.0f /overFlowFbo->width);
    glUniform1f(glGetUniformLocation(lowpass_shader, "windowHeight"), 1.0f /overFlowFbo->height);

    // ping-pong
    for (int i {0}; i < 100; i++)
    {
        useFBO(tempFbo, overFlowFbo, NULL);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        DrawModel(squareModel, lowpass_shader, "in_Position", nullptr, "in_TexCoord");

        std::swap(tempFbo, overFlowFbo);
    }

    // draw to screen
    glUseProgram(bloom_shader);
    glUniform1i(glGetUniformLocation(bloom_shader, "texUnitBloom"),1);
    useFBO(nullptr, bloomFbo, overFlowFbo);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    DrawModel(squareModel, bloom_shader, "in_Position", nullptr, "in_TexCoord");

    printError("Blooming");
}