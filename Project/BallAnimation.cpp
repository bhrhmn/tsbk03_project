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

#include "models.h"
#include "scene.h"

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
#define kMaxBones 14
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

//bone debug
GLuint boneVAO = 0;
GLuint boneVBO = 0;
GLuint boneShader = 0;



//////////////////////////////////////
//		B O N E
// Desc:  A simple bone structure with position and rotation.
//        rot could have been mat3 but matrix creation in VectorUtils
//        only supports mat4. (We could also cast from that, of course.)
typedef struct Bone
{
	vec3 pos;
	mat4 rot;
	vec3 offset;
}
Bone;

mat4 inverseBindPose[kMaxBones];

///////////////////////////////////////
//		G _ B O N E S
// Our "skeleton"
Bone g_bones[kMaxBones]; // Original data, do not change
Bone g_bonesRes[kMaxBones]; // For animation, change to animate
int boneParent[kMaxBones] =
{-1, //root
0, 0, 0, 0, //shoulders
1, 2, 3, 4,
5, 6};



void setupBindPoseInverse()
{
	mat4 boneTransform[kMaxBones];

	for (int i = 0; i < kMaxBones; i++)
	{
		int parent = boneParent[i];

		// Build local transform from rotation and translation at bind pose
		mat4 localTransform = g_bones[i].rot;
		localTransform.m[3]  = g_bones[i].pos.x;
		localTransform.m[7]  = g_bones[i].pos.y;
		localTransform.m[11] = g_bones[i].pos.z;

		if (parent >= 0)
			boneTransform[i] = boneTransform[parent] * localTransform;
		else
			boneTransform[i] = localTransform;

		// Calculate and store inverse bind pose matrix
		inverseBindPose[i] = InvertMat4(boneTransform[i]);
	}
}


///////////////////////////////////////////////////////
//		S E T U P  B O N E S
//
void setupBones(GLuint shader)
{
	int bone;
	//Hip Core
	g_bones[0].pos = vec3(0, 20, -15);

	//Right front hip
	g_bones[1].pos = vec3(5, 17, 7);
	//Left front hip
	g_bones[2].pos = vec3(-6, 17, 7);
	//Right back hip
	g_bones[3].pos = vec3(5, 17, -17);
	//Left back hip
	g_bones[4].pos = vec3(-6, 17, -17);

	//Right front knee
	g_bones[5].pos = vec3(5, 5, 7);
	//Left front knee
	g_bones[6].pos = vec3(-6, 5, 7);

	//Right front paw
	g_bones[7].pos = vec3(5, -0, 7);
	//Left front paw
	g_bones[8].pos = vec3(-6, -0, 7);
	//Right back paw
	g_bones[9].pos = vec3(5, -0, -21);
	//Left Back paw
	g_bones[10].pos = vec3(-6, -0, -21);

	g_bones[11].pos = vec3(0, 25, 10);

	g_bones[12].pos = vec3(0, 30, 25);

	//Tail
	g_bones[13].pos = vec3(0, 35, -25);




	for (bone = 0; bone < kMaxBones; bone++)
	{
		g_bones[bone].rot = IdentityMatrix();
		g_bones[bone].offset = vec3(0,0,0);
	}

	setupBindPoseInverse();
}


float objWeight[MAX_VERTICES][kMaxBones];
vec3 g_vertsResObj[MAX_VERTICES];


void ConnectVertToBone(Model* model)
{
	vec3* verticesArray = model->vertexArray;
	printf("Vertices: ");
	printVec3(*verticesArray);
	memset(objWeight, 0, sizeof(objWeight));

	int maxVerts = model->numVertices;
	if (maxVerts > MAX_VERTICES) maxVerts = MAX_VERTICES;
	mat4 invWolfT = InvertMat4(wolfObjT);


	for (int vertex = 0; vertex < maxVerts; vertex++)
	{
		float total = 0;
		for (int bone = 0; bone < kMaxBones; bone++)
		{
			vec3 boneObjPos = MultVec3(invWolfT, g_bones[bone].pos);

			vec3 relpos = boneObjPos - model->vertexArray[vertex];

			if (vertex < MAX_VERTICES && relpos.y < 10.f && relpos.y > -10.f
				&& relpos.x < 10.f && relpos.x > -10.f
				&& relpos.z < 10.f && relpos.z > -10.f)
			{
				objWeight[vertex][bone] = 1.0f;
				total += 1.0f;
				printf("Small thresh");
			}
			else if (vertex < MAX_VERTICES && relpos.y < 50.f && relpos.y > -50.f
				&& relpos.x < 50.f && relpos.x > -50.f
				&& relpos.z < 50.f && relpos.z > -50.f)
			{
				objWeight[vertex][bone] = 0.2f;
				total += 0.2f;
				printf("big thresh");
			}

		}
		if (total > 0.0f)
		{
			for (int b = 0; b < kMaxBones; b++)
				objWeight[vertex][b] /= total;
		}
	}

	for (int vertex = 0; vertex < maxVerts; vertex++)
	{
		float total = 0;
		for (int bone = 0; bone < kMaxBones; bone++)
		{
			total += objWeight[vertex][bone];
		}
		if (total < 0.99f) // Not properly normalized or no weights
		{
			printf("Vertex %d has total weight %f\n", vertex, total);
		}
	}

}

Model *wolf;


void changeMesh(Model *model)
{
	int maxVerts = model->numVertices;
	if (maxVerts > MAX_VERTICES)
		maxVerts = MAX_VERTICES;

	// Calculate current bone transforms (with animation)
	mat4 boneTransform[kMaxBones];
	for (int i = 0; i < kMaxBones; i++)
	{
		int parent = boneParent[i];

		mat4 localTransform = g_bonesRes[i].rot;
		localTransform.m[3]  = g_bonesRes[i].pos.x;
		localTransform.m[7]  = g_bonesRes[i].pos.y;
		localTransform.m[11] = g_bonesRes[i].pos.z;

		if (parent >= 0)
			boneTransform[i] = boneTransform[parent] * localTransform;
		else
			boneTransform[i] = localTransform;
	}

	// For each vertex
	for (int vert = 0; vert < maxVerts; vert++)
	{
		vec3 finalPos = SetVector(0, 0, 0);
		vec4 v4 = SetVec4(model->vertexArray[vert].x,
						  model->vertexArray[vert].y,
						  model->vertexArray[vert].z,
						  1.0f);

		// Apply skinning using bone transforms and inverse bind poses
		for (int i = 0; i < kMaxBones; i++)
		{
			if (objWeight[vert][i] == 0.0f) continue;

			// Transform vertex to bone local space using inverse bind pose
			vec4 localPos = inverseBindPose[i] * v4;

			// Transform local position by current bone transform
			vec4 transformed = boneTransform[i] * localPos;

			finalPos = finalPos + objWeight[vert][i] * SetVector(transformed.x, transformed.y, transformed.z);
		}
		g_vertsResObj[vert] = finalPos;
	}
}






void animateObj(GLuint shader)
{
	float t = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	float a = sinf(t) * 0.5f;

	memcpy(g_bonesRes, g_bones, sizeof(g_bones));
	float speed = 5.0f;

	float phase = t * speed;

	float footswing = sinf(phase) * 0.2f;
	printf("%f", footswing);

	g_bonesRes[7].rot =Rx(footswing);
	//g_bonesRes[1].offset += vec3(0.2f, 0.0f, 0.0f);

	/*
	float hipSwing   = cosf(phase) * 0.1f;
	float kneeBend   = fmaxf(0.0f, sinf(phase)) * 1.0f; // only when forward
	float footLift   = fmaxf(0.0f, sinf(phase)) * 0.8f;

	g_bonesRes[4].rot    = Rx(hipSwing);
*/
	changeMesh(wolf); // calculates g_vertsResObj for all vertices


	// upload all vertices
	int numVerts = wolf->numVertices;
	glBindVertexArray(wolf->vao);
	glBindBuffer(GL_ARRAY_BUFFER, wolf->vb);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3) * numVerts, g_vertsResObj);

	DrawModel(wolf, shader, "in_Position", "inNormal", "inTexCoord");
}

void DrawBones(void)
{
	glUseProgram(boneShader);

	uploadMat4ToShader(boneShader, "world_To_View", worldCamera);
	uploadMat4ToShader(boneShader, "projection", projectionMatrix);

	glBindVertexArray(boneVAO);
	glDrawArrays(GL_POINTS, 0, kMaxBones);
	glBindVertexArray(0);
}

/////////////////////////////////////////
//		M A I N
//


void ball(Model *model)
{
	wolf = model;

	setupBones(0);

	ConnectVertToBone(wolf);
	changeMesh(wolf);

	// Load bone shader
	boneShader = loadShaders("Shaders/bone.vert", "Shaders/bone.frag");

	// Create VAO/VBO
	glGenVertexArrays(1, &boneVAO);
	glGenBuffers(1, &boneVBO);

	glBindVertexArray(boneVAO);
	glBindBuffer(GL_ARRAY_BUFFER, boneVBO);

	vec3 bonePositions[kMaxBones];
	for(int i=0; i<kMaxBones; i++)
		bonePositions[i] = g_bones[i].pos;

	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * kMaxBones, bonePositions, GL_STATIC_DRAW);


	// Attribute 0 = position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}


/*
void ball(Model *model)
{
	wolf = model;
	glEnable(GL_DEPTH_TEST);


	g_shader = loadShaders("Shaders/shadow.vert" , "Shaders/shadow.frag");
	setupBones(g_shader);

	ConnectVertToBone(model);
	changeMesh(model);

}

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

			vec3 q = objPos - g_bones[b].pos;   // relative to original bone
			q = g_bonesRes[b].rot * q;          // rotate
			q += g_bonesRes[b].pos;             // move with new bone position
			q += g_bonesRes[b].offset;          // apply extra offset if needed

			result += w * q;
		}

		g_vertsResObj[i] = result;

	}
}
*/