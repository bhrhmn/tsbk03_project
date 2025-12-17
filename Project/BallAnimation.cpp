//
// Created by Luna on 2025-12-17.
//

#include "BallAnimation.h"



#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "MicroGlut.h"
#include "VectorUtils4.h"
#include "GL_utilities.h"
#include "LittleOBJLoader.h"
// uses framework Cocoa
// uses framework OpenGL
#include <string.h>

// Ref till shader
GLuint g_shader;


typedef struct Triangle
{
  GLuint        v1;
  GLuint        v2;
  GLuint        v3;
} Triangle;

#define CYLINDER_SEGMENT_LENGTH 0.37
#define kMaxRow 100
#define kMaxCorners 8
#define kMaxBones 4
#define kMaxg_poly ((kMaxRow-1) * kMaxCorners * 2)
#ifndef Pi
#define Pi 3.1416
#endif
#ifndef true
#define true 1
#endif

#define BONE_LENGTH 4.0
#define MAX_VERTICES 3000

Triangle g_poly[kMaxg_poly];

// vertices
vec3 g_vertsOrg[kMaxRow][kMaxCorners];
vec3 g_normalsOrg[kMaxRow][kMaxCorners];
vec3 g_vertsRes[kMaxRow][kMaxCorners];
vec3 g_normalsRes[kMaxRow][kMaxCorners];

// vertex attributes
float g_boneWeights[kMaxRow][kMaxCorners][kMaxBones];

float skinningWeight[kMaxRow] = {0.0, 0.1, 0.25, 0.3, 0.4, 0.5, 0.6, 0.75, 0.9, 1.0};
vec2 g_boneWeightVis[kMaxRow][kMaxCorners]; // Copy data to here to visualize your weights

Model *cylinderModel; // Collects all the above for drawing with glDrawElements

mat4 modelViewMatrixB, projectionMatrixB;




//////////////////////////////////////
//		B O N E
// Desc:  A simple bone structure with position and rotation.
//        rot could have been mat3 but matrix creation in VectorUtils
//        only supports mat4. (We could also cast from that, of course.)
typedef struct Bone
{
  vec3 pos;
  mat4 rot;
} Bone;


///////////////////////////////////////
//		G _ B O N E S
// Our "skeleton"
Bone g_bones[kMaxBones]; // Original data, do not change
Bone g_bonesRes[kMaxBones]; // For animation, change to animate


///////////////////////////////////////////////////////
//		S E T U P  B O N E S
//
void setupBones(void)
{
	int bone;

	g_bones[0].pos = vec3(0.01, 0.3, 0.75);
	g_bones[1].pos = vec3(0.01, 0.02, 0.75);
	g_bones[2].pos = vec3(40, -20, 0);
	g_bones[3].pos = vec3(-10, -80, -10);

	for (bone = 0; bone < kMaxBones; bone++)
	{
		g_bones[bone].rot = IdentityMatrix();
	}
}



float objWeight[MAX_VERTICES][kMaxBones];
vec3 g_vertsResObj[MAX_VERTICES];


void ConnectVertToBone(Model *model)
{
	vec3* verticesArray = model->vertexArray;
	printf("Vertices: ");
	printVec3(*verticesArray);
	memset(objWeight, 0, sizeof(objWeight));

	int maxVerts = model->numVertices;
	if (maxVerts > MAX_VERTICES) maxVerts = MAX_VERTICES;

	for (int vertex = 0; vertex < maxVerts; vertex++)
	{
		float total = 0;
		for (int bone = 0; bone < kMaxBones; bone++){
			vec3 relpos = g_bones[bone].pos - model->vertexArray[vertex];
			if (vertex < MAX_VERTICES && relpos.y < 50.f && relpos.y > -50.f
				&& relpos.x < 50.f && relpos.x > -50.f)
			{
				objWeight[vertex][bone] = 1.0f;
				total += 1.0f;
			}
			else if (vertex < MAX_VERTICES && relpos.y < 4.f && relpos.y > -4.f
				&& relpos.x < 4.f && relpos.x > -4.f)
			{
				objWeight[vertex][bone] = 0.2f;
				total += 0.2f;
			}
		}
		if (total > 0.0f)
		{
			for (int b = 0; b < kMaxBones; b++)
				objWeight[vertex][b] /= total;
		}

	}
}

Model *wolf;
void changeMesh(Model *model)
{
	int maxVerts = model->numVertices;
	if (maxVerts > MAX_VERTICES) maxVerts = MAX_VERTICES;

	for (int i = 0; i < maxVerts; i++)
	{

		vec3 objPos = model->vertexArray[i];

		float weightSum = 0.0f;
		for (int b = 0; b < kMaxBones; b++)
			weightSum += objWeight[i][b];

		if (weightSum == 0.0f)
		{
			g_vertsResObj[i] = objPos;
			continue;
		}

		vec3 result = vec3(0,0,0);

		for (int b = 0; b < kMaxBones; b++)
		{
			float w = objWeight[i][b];
			if (w == 0.0f) continue;

			vec3 q = objPos - g_bonesRes[b].pos;
			q = g_bonesRes[b].rot * q;
			q += g_bonesRes[b].pos;

			result += w * q;
		}

		g_vertsResObj[i] = result;

		if (i == 0)
		{
			printf("orig: %f %f %f  ->  new: %f %f %f\n",
				objPos.x, objPos.y, objPos.z,
				g_vertsResObj[i].x,
				g_vertsResObj[i].y,
				g_vertsResObj[i].z);
		}
	}
}


void animateObj(GLuint shader)
{
	float t = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	float a = sinf(t) * 0.5f;

	memcpy(g_bonesRes, g_bones, sizeof(g_bones));
	g_bonesRes[2].rot = Rz(-a);
	g_bonesRes[3].rot = Rz(a);

	changeMesh(wolf); // calculates g_vertsResObj for all vertices

	// upload all vertices
	int numVerts = wolf->numVertices;
	glBindVertexArray(wolf->vao);
	glBindBuffer(GL_ARRAY_BUFFER, wolf->vb);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3) * numVerts, g_vertsResObj);

	DrawModel(wolf, shader, "in_Position", "inNormal", "inTexCoord");
}





/////////////////////////////////////////
//		M A I N
//
void ball(Model *model)
{
	wolf = model;
  // Set up depth buffer
  glEnable(GL_DEPTH_TEST);
  // initiering
  setupBones();

	ConnectVertToBone(model);
	changeMesh(model);


	// Build Model from cylinder data
	/*cylinderModel = LoadDataToModel(
			(vec3*) g_vertsRes,
			(vec3*) g_normalsRes,
			(vec2*) g_boneWeightVis, // texCoords
			NULL, // (GLfloat*) g_boneWeights, // colors
			(GLuint*) g_poly, // indices
			kMaxRow*kMaxCorners,
			kMaxg_poly * 3);*/

  g_shader = loadShaders("Shaders/shadow.vert" , "Shaders/shadow.frag");

}
