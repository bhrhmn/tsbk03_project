// rain.cpp
#include "rain.h"
#include "scene.h"
#include "models.h"

unsigned int vertexArrayObjID;
GLuint rain_program, pos_program, vel_program, simple_program;
GLfloat slope = 10;
GLfloat a = 0.5;
GLuint count = 5000;
GLuint tex;
GLfloat prev_time = 0.0;
Model *squareM;
#define pSize 0.01f

GLfloat vertices2[] = {	-1.0f,-1.0f,0.0f,
						-1.0f,1.0f,0.0f,
						1.0f,1.0f,0.0f,
						1.0f,-1.0f,0.0f};
GLfloat texcoord2[] = {	0.0f, 0.0f,
						0.0f, 1.0f,
						1.0f, 1.0f,
						1.0f, 0.0f};
GLuint indices2[] = {0,1,3, 3,1,2};

GLfloat particleVertices[] = {
						-pSize,-pSize,0.0f,
						-pSize,pSize,0.0f,
						pSize, pSize,0.0f,
						pSize,-pSize,0.0f};

bool ping = true;
int init_positions_tex = 19;

void DrawModelInstanced(Model *m, GLuint program, const char* vertexVariableName, const char* normalVariableName, const char* texCoordVariableName, int count)
{
	// Ragnemalm's DrawModel but with instancing
	if (m != NULL)
	{
		GLint loc;
		
		glBindVertexArray(m->vao);	// Select VAO
		glUseProgram(program); // Added 2022-03

		glBindBuffer(GL_ARRAY_BUFFER, m->vb);
		loc = glGetAttribLocation(program, vertexVariableName);
		if (loc >= 0)
		{
			glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, 0); 
			glEnableVertexAttribArray(loc);
		}
		// else
		// 	LOLError("DrawModel", vertexVariableName);
		
		if (normalVariableName!=NULL)
		{
			loc = glGetAttribLocation(program, normalVariableName);
			if (loc >= 0)
			{
				glBindBuffer(GL_ARRAY_BUFFER, m->nb);
				glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
				glEnableVertexAttribArray(loc);
			}
			// else
			// 	LOLError("DrawModel", normalVariableName);
		}
		if ((m->texCoordArray != NULL)&&(texCoordVariableName != NULL))
		{
			loc = glGetAttribLocation(program, texCoordVariableName);
			if (loc >= 0)
			{
				glBindBuffer(GL_ARRAY_BUFFER, m->tb);
				glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, 0);
				glEnableVertexAttribArray(loc);
			}
			// else
			// 	LOLError("DrawModel", texCoordVariableName);
		}
		glDrawElementsInstanced(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0L, count);
	}
}

void rain_init() 
{

	squareM = LoadDataToModel((vec3 *)vertices2, NULL, (vec2 *)texcoord2, NULL, indices2, 4, 6);

    rain_program = loadShaders("Shaders/instancing.vert", "Shaders/instancing.frag");
	pos_program = loadShaders("Shaders/minimal.vert", "Shaders/pos.frag");
	vel_program = loadShaders("Shaders/minimal.vert", "Shaders/vel.frag");
	simple_program = loadShaders("Shaders/minimal.vert", "Shaders/simple.frag");
	
	uploadUniformIntToShader(pos_program, "posTex", 0);
	uploadUniformIntToShader(pos_program, "velTex", 1);
	uploadUniformIntToShader(pos_program, "startPosTex", init_positions_tex);
	uploadUniformIntToShader(vel_program, "velTex", 1);
	int tex = 19;
    uploadUniformIntToShader(rain_program, "tex", tex); 
	uploadUniformIntToShader(rain_program, "pos1", 0); 
	uploadUniformIntToShader(rain_program, "pos2", 1); 

    uploadUniformFloatToShader(rain_program, "texSize", 256.0); 	

	
	glUseProgram(rain_program);
	uploadMat4ToShader(rain_program, "projectionMatrix", projectionMatrix);

	printError("rain_init");
}

void rain(GLfloat time) 
{
    glDisable(GL_CULL_FACE);  	
	glDisable(GL_DEPTH_TEST);	

	// update position
	if (time < 1)
	{
		// write initial positions
		glUseProgram(simple_program);
		glActiveTexture(GL_TEXTURE0 + init_positions_tex);
		uploadUniformIntToShader(simple_program, "textureUnit", init_positions_tex);
		useFBO(pos1FBO, NULL, NULL);
		DrawModel(squareM, simple_program, "inPosition", NULL, "inTexCoord");
		useFBO(pos2FBO, NULL, NULL);
		DrawModel(squareM, simple_program, "inPosition", NULL, "inTexCoord");	
	}
	else { 
		if (ping)
		{
			uploadUniformFloatToShader(pos_program, "time", time);
			useFBO(pos2FBO, pos1FBO, vel1FBO);
			// Update position
			DrawModel(squareM, pos_program, "inPosition", NULL, "inTexCoord");	
			
			uploadUniformFloatToShader(vel_program, "delta_time", time - prev_time);
			useFBO(vel2FBO, pos1FBO, vel1FBO);
			// Update velocity
			DrawModel(squareM, vel_program, "inPosition", NULL, "inTexCoord");
			glFlush();
		}
		else
		{
			// pong
			// Same thing but opposite 
			uploadUniformFloatToShader(pos_program, "time", time);
			useFBO(pos1FBO, pos2FBO, vel2FBO);
			DrawModel(squareM, pos_program, "inPosition", NULL, "inTexCoord");

			uploadUniformFloatToShader(vel_program, "delta_time", time - prev_time);
			useFBO(vel1FBO, pos2FBO, vel2FBO);
			DrawModel(squareM, vel_program, "inPosition", NULL, "inTexCoord");
			glFlush();
		}
		ping = !ping; 
	}
	prev_time = time;
		
	// draw rain
	glEnable(GL_DEPTH_TEST);	
	useFBO(NULL, pos1FBO, pos2FBO); 
	uploadMat4ToShader(rain_program, "world_to_view", worldCamera);
	vec3 offset = vec3(1000, -100, 500);
	uploadUniformVec3ToShader(rain_program, "pos_offset", offset);
	// glBindVertexArray(vertexArrayObjID); 
    glClear(GL_DEPTH_BUFFER_BIT);
	DrawModelInstanced(squareM, rain_program, "inPosition", "inNormal", "inTexCoord", count);

	printError("rain in display()\n");
}
