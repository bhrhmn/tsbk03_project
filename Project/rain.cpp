// rain.cpp
#include "rain.h"
#include "scene.h"
#include "models.h"

unsigned int vertexArrayObjID;
GLuint rain_program, pos_program, vel_program;
GLfloat slope = 10;
GLfloat a = 0.5;
GLuint count = 5000;
GLuint tex;

bool ping = true;

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

	printf((std::to_string(time) + "\n").c_str());

	glClearColor(0.2,0.2,0.5,0);
	//glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);  	// behåll!!
	glDisable(GL_DEPTH_TEST);	// behåll!!
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// update position
	glUseProgram(vel_program);
	uploadUniformFloatToShader(vel_program, "time", time);
	glUseProgram(pos_program);
	uploadUniformFloatToShader(pos_program, "time", time);
	if (time > 1)
	{
		if (ping)
		{
			printf("ping\n");
		// Draw p1 and v1 to p2 and v2
			useFBO(pos2FBO, pos1FBO, vel1FBO);
		// Update position
			DrawModel(treeBillboard, pos_program, "inPosition", NULL, "inTexCoord");	// det verkar inte som att positionen ändras hmmmmmmm
			useFBO(vel2FBO, pos1FBO, vel1FBO);
		// Update velocity
			DrawModel(treeBillboard, vel_program, "inPosition", NULL, "inTexCoord");
			glFlush();
		}
		else
		{
		// Same thing opposite ping-pong
			printf("pong\n");
			useFBO(pos1FBO, pos2FBO, vel2FBO);
			DrawModel(treeBillboard, pos_program, "inPosition", NULL, "inTexCoord");
			useFBO(vel1FBO, pos2FBO, vel2FBO);
			DrawModel(treeBillboard, vel_program, "inPosition", NULL, "inTexCoord");
			glFlush();
		}
	}	

	ping = !ping; 

	useFBO(NULL, pos1FBO, pos2FBO); 
	// draw rain
	glEnable(GL_DEPTH_TEST);		// behåll!!
	glUseProgram(rain_program);
	int tex = 7;
	glActiveTexture(GL_TEXTURE0 + tex); //RAIN_TEX_UNIT);
    glUniform1i(glGetUniformLocation(rain_program, "tex"), tex); //RAIN_TEX_UNIT);  // texture 0 och 1 (pos och vel) är typ svarta?? dom borde vara röd respektive grön🙃
	glActiveTexture(GL_TEXTURE0); 
	glUniform1i(glGetUniformLocation(rain_program, "pos1"), 0); 
	glActiveTexture(GL_TEXTURE1); 
	glUniform1i(glGetUniformLocation(rain_program, "pos2"), 1); 
    glUniform1i(glGetUniformLocation(rain_program, "texSize"), 256); 	// lärdom, kolla ALLTID att man använder rätt namn på variabler 🙃🙃🙃🙃🙃🙃🙃🙃🙃
	uploadMat4ToShader(rain_program, "world_to_view", worldCamera);
	// glActiveTexture(GL_TEXTURE0);
	// glUniform1i(glGetUniformLocation(rain_program, "posTex"),0);
	
	glBindVertexArray(vertexArrayObjID);
	
	DrawModelInstanced(treeBillboard, rain_program, "inPosition", "inNormal", "inTexCord", count);
	
    // glEnable(GL_CULL_FACE); // TA BORT
	printError("rain in display()\n");
}
