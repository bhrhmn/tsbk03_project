// rain.cpp
#include "rain.h"
#include "scene.h"
#include "models.h"

unsigned int vertexArrayObjID;
GLuint rain_program;
GLfloat slope = 10;
GLfloat a = 0.5;
GLuint count = 1000000;
GLuint tex;
GLfloat position_data[256*256*3]; // random data kanske hmm öööööööööööööööööööööööööööööööööööööööööööööööööööööööööööööööööööööööööööööööö 
FBOstruct *pos1;

void DrawModelInstanced(Model *m, GLuint program, const char* vertexVariableName, const char* normalVariableName, const char* texCoordVariableName, int count)
{
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
	// unsigned int vertexBufferObjID;

    rain_program = loadShaders("Shaders/instancing.vert", "Shaders/instancing.frag");
	glUseProgram(rain_program);
	printError("init shader");
    // Upload geometry to the GPU:

    glUniformMatrix4fv(glGetUniformLocation(rain_program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);

	//glActiveTexture(GL_TEXTURE19);
    //pos1 = initFBO2(WINDOW_SIZE, WINDOW_SIZE, 0, 1);

	// End of upload of geometry
}

void rain() 
{
	glClearColor(0.2,0.2,0.5,0);
	//glDisable(GL_DEPTH_TEST);
	//glEnable(GL_DEPTH_TEST);
    //glDisable(GL_CULL_FACE);  // TA BORT, väggar försvinner :(
	
	vec3 rain_pos[] {
		vec3(0.f, 0.f, 0.f), 
		vec3(10.f, 0.f, 0.f),
		vec3(10.f, 0.f, 10.f),
		vec3(0.f, 0.f, 10.f),
		vec3(5.f, 0.f, 0.f),
	};
	
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glUseProgram(rain_program);
	
    mat4 rot, trans, total;
	glActiveTexture(GL_TEXTURE0 + 8); //RAIN_TEX_UNIT);
    glUniform1i(glGetUniformLocation(rain_program, "tex"), 8); //RAIN_TEX_UNIT);
	glActiveTexture(GL_TEXTURE19); 	
    glUniform1i(glGetUniformLocation(rain_program, "pos1"), 19); //RAIN_TEX_UNIT); 	
    //glUniform3fv(glGetUniformLocation(rain_program, "rain_pos"), 5, &rain_pos->x);
    glUniform1i(glGetUniformLocation(rain_program, "texsize"), 256); //RAIN_TEX_UNIT); 	
	trans = T(-50.0, 10.0, 0.0) * S(1);
	uploadMat4ToShader(rain_program, "world_to_view", worldCamera);
	uploadMat4ToShader(rain_program, "model_to_world", trans);
	
	// int 😎 {0};
	// std::cout << 😎 << std::endl;
	
	glBindVertexArray(vertexArrayObjID);	// Select VAO
	// Draw the triangle "count" times!
	// glDrawArraysInstanced(GL_TRIANGLES, 0, 3, count);

	DrawModelInstanced(treeBillboard, rain_program, "in_Position", "inNormal", "inTexCord", count);
	
    // glEnable(GL_CULL_FACE); // TA BORT

	printError("rain in display()\n");
}
