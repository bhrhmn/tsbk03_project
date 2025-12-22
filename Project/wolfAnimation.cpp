//
// Created by Luna on 2025-12-17.
//

#include "wolfAnimation.h"



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

#define kMaxBones 16

#define MAX_VERTICES 3000

//bone debug
GLuint boneVAO;
GLuint boneVBO;
GLuint boneShader;

Model *wolf;

mat4 inverseBindPose[kMaxBones];
float Weights[MAX_VERTICES][kMaxBones];
vec3 g_vertsRes[MAX_VERTICES];

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

/*
 * Array which points each bone to its parent.
 */
int boneParent[kMaxBones] =
{-1, //root
0, 1, 0, //Neck, head, tail
1, 1, 0, 0, //Front shoulders, back shoulders
4, 5, 6, 7, //Front knees, back knees
8, 9, 10, 11}; //Front paws, back paws.


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

		inverseBindPose[i] = InvertMat4(boneTransform[i]);
	}
}


///////////////////////////////////////////////////////
//		S E T U P  B O N E S
//
void setupBones()
{
	//Hip Core
	g_bones[0].pos = vec3(0, 20, -15);
	//Neck
	g_bones[1].pos = vec3(0, 20, 10);
	//Head
	g_bones[2].pos = vec3(0, 30, 22);
	//Tail
	g_bones[3].pos = vec3(0, 14, -28);

	//Right front hip
	g_bones[4].pos = vec3(5, 17, 7);
	//Left front hip
	g_bones[5].pos = vec3(-6, 17, 7);
	//Right back hip
	g_bones[6].pos = vec3(5, 17, -17);
	//Left back hip
	g_bones[7].pos = vec3(-6, 17, -17);

	//Right front knee
	g_bones[8].pos = vec3(5, 5, 7);
	//Left front knee
	g_bones[9].pos = vec3(-6, 5, 7);

	//Right Back knee
	g_bones[10].pos = vec3(5, 5, -22);
	//Left Back knee
	g_bones[11].pos = vec3(-6, 5, -22);

	//Right front paw
	g_bones[12].pos = vec3(5, -0, 7);
	//Left front paw
	g_bones[13].pos = vec3(-6, -0, 7);
	//Right back paw
	g_bones[14].pos = vec3(5, -0, -21);
	//Left Back paw
	g_bones[15].pos = vec3(-6, -0, -21);


	for (int bone = 0; bone < kMaxBones; bone++)
	{
		g_bones[bone].rot = IdentityMatrix();
	}

	setupBindPoseInverse();
}


void CalculateWeights(Model* model)
{
	vec3* verticesArray = model->vertexArray;
	printVec3(*verticesArray);
	memset(Weights, 0, sizeof(Weights));

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
				Weights[vertex][bone] = 1.0f;
				total += 1.0f;
			}
			else if (vertex < MAX_VERTICES && relpos.y < 30.f && relpos.y > -30.f
				&& relpos.x < 25.f && relpos.x > -25.f
				&& relpos.z < 30.f && relpos.z > -30.f)
			{
				Weights[vertex][bone] = 0.5f;
				total += 0.5f;
			}
			else if (vertex < MAX_VERTICES && relpos.y < 60.f && relpos.y > -60.f
				&& relpos.x < 30.f && relpos.x > -30.f
				&& relpos.z < 60.f && relpos.z > -60.f)
			{
				Weights[vertex][bone] = 0.05f;
				total += 0.05f;
			}

		}
		if (total > 0.0f)
		{
			for (int b = 0; b < kMaxBones; b++)
				Weights[vertex][b] /= total;
		}
	}

	for (int vertex = 0; vertex < maxVerts; vertex++)
	{
		float total = 0;
		for (int bone = 0; bone < kMaxBones; bone++)
		{
			total += Weights[vertex][bone];
		}
		if (total < 0.99f) // Not properly normalized or no weights
		{
			printf("Vertex %d has total weight %f\n", vertex, total);
		}
	}

}


/*
 * Updates vertice position based on the bones current position.
 */
void changeMesh()
{
	int maxVerts = wolf->numVertices;
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
		vec4 v4 = SetVec4(wolf->vertexArray[vert].x,
						  wolf->vertexArray[vert].y,
						  wolf->vertexArray[vert].z,
						  1.0f);

		// Apply skinning using bone transforms and inverse bind poses
		for (int i = 0; i < kMaxBones; i++)
		{
			if (Weights[vert][i] == 0.0f) continue;

			// Transform vertex to bone local space using inverse bind pose
			vec4 localPos = inverseBindPose[i] * v4;

			// Transform local position by current bone transform
			vec4 transformed = boneTransform[i] * localPos;

			finalPos = finalPos + Weights[vert][i] * SetVector(transformed.x, transformed.y, transformed.z);
		}
		g_vertsRes[vert] = finalPos;
	}
}

/*
 * Changes positions of bones.
 */

void updateBones(GLuint shader)
{
	float t = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	memcpy(g_bonesRes, g_bones, sizeof(g_bones));
	float speed = 5.0f;

	float phase = t * speed;

	float footswing = sinf(phase) * 0.2f;
	float kneeBend   = fmaxf(0.0f, sinf(phase)*0.5); // only when forward
	float hipSwing   = sinf(phase) * 0.1f;

	float footswingL = cosf(phase) * 0.2f;
	float kneeBendL   = fmaxf(0.0f, cosf(phase)*0.5); // only when forward
	float hipSwingL   = cosf(phase) * 0.1f;

	//Right front leg
	g_bonesRes[12].rot =Rx (footswing);
	g_bonesRes[8].rot =Rx (-kneeBend);
	g_bonesRes[4].rot =Rx (hipSwing);

	//Left Front leg
	g_bonesRes[13].rot = Rx (footswingL);
	g_bonesRes[9].rot = Rx (-kneeBendL);
	g_bonesRes[5].rot = Rx (hipSwingL);

	//Right back leg
	g_bonesRes[14].rot = Rx (footswingL);
	g_bonesRes[10].rot = Rx (-kneeBendL);
	g_bonesRes[6].rot = Rx (hipSwingL);

	//Left back leg
	g_bonesRes[15].rot = Rx (footswing);
	g_bonesRes[11].rot = Rx (-kneeBend);
	g_bonesRes[7].rot = Rx (hipSwing);

	//Tail
	g_bonesRes[3].rot = Ry (footswing);

	//Head
	g_bonesRes[2].rot = Rx (hipSwingL);

	changeMesh(); // calculates g_vertsRes for all vertices

	// upload all vertices
	int numVerts = wolf->numVertices;
	glBindVertexArray(wolf->vao);
	glBindBuffer(GL_ARRAY_BUFFER, wolf->vb);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3) * numVerts, g_vertsRes);

	DrawModel(wolf, shader, "in_Position", "inNormal", "inTexCoord");
}


void DrawBones()
{
	glUseProgram(boneShader);

	uploadMat4ToShader(boneShader, "world_To_View", worldCamera);
	uploadMat4ToShader(boneShader, "projection", projectionMatrix);

	glBindVertexArray(boneVAO);
	glDrawArrays(GL_POINTS, 0, kMaxBones);
	glBindVertexArray(0);
}


/*
 * Uses bone shaders to show positions of bones. Must be used together with drawBones() in rendering.cpp.
 * Also, stop the wolf from moving in UpdateWolf (WolfObjT).
 */

void boneDebug()
{
	glGenVertexArrays(1, &boneVAO);
	glGenBuffers(1, &boneVBO);

	glBindVertexArray(boneVAO);
	glBindBuffer(GL_ARRAY_BUFFER, boneVBO);

	vec3 bonePositions[kMaxBones];
	for(int i=0; i<kMaxBones; i++)
		bonePositions[i] = g_bones[i].pos;

	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * kMaxBones, bonePositions, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

/////////////////////////////////////////
//		M A I N
/////////////////////////////////////////

void wolfAnimation(Model *model)
{
	wolf = model;
	setupBones();

	CalculateWeights(wolf);

	// Load bone shader
	boneShader = loadShaders("Shaders/bone.vert", "Shaders/bone.frag");

	//Uncomment this as well as drawBones in rendering.cpp make bone positions visible
	boneDebug();
}
