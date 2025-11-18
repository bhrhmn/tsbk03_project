// lighting.cpp
#include "lighting.h"
#include "scene.h"
#include "models.h"

Model *fireModel;
Model *fireModel2;

vec3 firePos = vec3(25.f, 10.0f, 20.f);
vec3 fireColor = vec3(2.5f, 2.7f, 2.4f);
vec3 fire_start_pos = vec3(35.5f, -2.5f, 25.5f);

vec3 moonPos = vec3(120.0f, 70.0f, -120.f);
vec3 moonColor = vec3(0.8f, 0.8f, 1.0f);

float randFloat() {
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

float flicker(const float time, float speed, const float intensity) {
    static float lastValue = 0.5f;
    static float lastChangeTime = 0.0f;

    if (time - lastChangeTime > 0.05f) {
        lastValue = 0.5f + (randFloat() - 0.5f) * intensity;
        lastChangeTime = time;
    }
    return lastValue;
}

void UpdateLightSources() {
    const float fireJitterX = flicker(t, 10.0f, 0.5f);
    const float fireJitterY = flicker(t, 15.0f, 0.3f);
    firePos = vec3(33 + fireJitterX, -1.0f + fireJitterY, 23);
    glUniform3fv(glGetUniformLocation(object_shader, "firePos"), 1, &firePos.x);

    float fireIntensity = 2.8f + 0.2f * flicker(t, 8.0f, 1.0f);
    fireColor = vec3(242.f/256, 125.f/256, 12.f/256) * fireIntensity;
    glUniform3fv(glGetUniformLocation(object_shader, "fireColor"), 1, &fireColor.x);

    // Fire animation updates
    const float f  = randFloat() / 400.0f;
    const float f2 = randFloat() / 300.0f;
    const float f3 = sin(t/2) * sin(t/3) / 2.0f;
    const float f4 = sin(t/3) / 2.0f;
    fireT =  T(fire_start_pos.x, fire_start_pos.y, fire_start_pos.z) * Ry(5*M_PI/4) * T(fire_start_pos.x*f, 0, 0) * T(f4, 0, 0) * S(0.1);
    fireT2 =  T(fire_start_pos.x -0.5f, fire_start_pos.y, fire_start_pos.z -0.5f) * Ry(5*M_PI/4) * T((fire_start_pos.x-1)*f2, 0, 0) * T(f3, 0, 0) * Ry(M_PI) * S(0.07f);
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
    glUniform1f(glGetUniformLocation(lowpass_shader, "windowWidth"), 1.0 /overFlowFbo->width);
    glUniform1f(glGetUniformLocation(lowpass_shader, "windowHeight"), 1.0 /overFlowFbo->height);

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