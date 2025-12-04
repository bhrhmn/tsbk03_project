// rain.cpp
#include "rain.h"
#include "scene.h"
#include "models.h"

unsigned int vertexArrayObjID;
GLuint rain_program, pos_program, vel_program;
GLfloat slope = 10;
GLfloat a = 0.5;
GLuint count = 1000;
GLuint tex;

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

    rain_program = loadShaders("Shaders/instancing.vert", "Shaders/instancing.frag");
	pos_program = loadShaders("Shaders/minimal.vert", "Shaders/pos.frag");
	vel_program = loadShaders("Shaders/minimal.vert", "Shaders/vel.frag");
	
	uploadUniformIntToShader(pos_program, "posTex", 0);
	uploadUniformIntToShader(pos_program, "velTex", 1);
	uploadUniformIntToShader(vel_program, "posTex", 0);
	uploadUniformIntToShader(vel_program, "velTex", 1);
	uploadUniformIntToShader(rain_program, "posTex", 0);
	
	glUseProgram(rain_program);
    glUniformMatrix4fv(glGetUniformLocation(rain_program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
	
	printError("rain_init");
}

void rain(GLfloat time) 
{

	// don't forget chmod +x efter merge

	glClearColor(0.2,0.2,0.5,0);
	//glDisable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_TEST);
    //glDisable(GL_CULL_FACE);  
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// update position
	glUseProgram(pos_program);
	useFBO(pos2FBO, pos1FBO, vel1FBO);
	DrawModel(treeBillboard, pos_program, "inPosition", NULL, "inTexCord");
	glUseProgram(vel_program);
	uploadUniformFloatToShader(vel_program, "time", time);
	useFBO(vel2FBO, pos1FBO, vel1FBO);
	DrawModel(treeBillboard, vel_program, "inPosition", NULL, "inTexCord");
	glFlush();
	// swap 
	std::swap(pos1FBO, pos2FBO);
	std::swap(vel1FBO, vel2FBO);
	useFBO(NULL, pos1FBO, vel1FBO);

	// draw rain
	glUseProgram(rain_program);
	glActiveTexture(GL_TEXTURE0 + 8); //RAIN_TEX_UNIT);
    glUniform1i(glGetUniformLocation(rain_program, "tex"), 8); //RAIN_TEX_UNIT);
	glActiveTexture(GL_TEXTURE0 + 7); 
    glUniform1i(glGetUniformLocation(rain_program, "pos1"), 7); 	
    glUniform1i(glGetUniformLocation(rain_program, "texSize"), 256); 	// lärdom, kolla ALLTID att man använder rätt namn på variabler 🙃🙃🙃🙃🙃🙃🙃🙃🙃
	uploadMat4ToShader(rain_program, "world_to_view", worldCamera);
	
	glBindVertexArray(vertexArrayObjID);

	DrawModelInstanced(treeBillboard, rain_program, "inPosition", "inNormal", "inTexCord", count);
	
    // glEnable(GL_CULL_FACE); // TA BORT
	printError("rain in display()\n");
}
