#include "Scene2.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Mtx44.h"

#include <iostream>

Scene2::Scene2()
{
}

Scene2::~Scene2()
{
}

void Scene2::Init()
{
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glEnable(GL_DEPTH_TEST);

	//Default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	// Generate buffers
	glGenBuffers(2, &m_vertexBuffer[0]);
	glGenBuffers(2, &m_colorBuffer[0]);

	//Load vertex and fragment shaders
	m_programID = LoadShaders( "Shader//TransformVertexShader.vertexshader", "Shader//ColorFragmentShader.fragmentshader" );
	// Get a handle for our "MVP" uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	// Use our shader
	glUseProgram(m_programID);

	// An array of 3 vectors which represents 3 vertices
	static const GLfloat vertex_buffer_data[] = {
		//-1.0f, -1.0f, 0.0f,
		//-0.5f, -1.0f, 0.0f,
		//-0.75f,  -0.5f, 0.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		0.0f,  1.0f, 1.0f,
		//-0.5f, -0.5f, 0.0f,
		//0.0f, -0.5f, 0.0f,
		//-0.25f,  0.0f, 0.0f,
	};
	static const GLfloat vertex_buffer_data_2[] = {
		//0.5f, -1.0f, 0.0f,
		//1.0f, -1.0f, 0.0f,
		//0.75f,  -0.5f, 0.0f,
		-0.5f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.25f,  0.5f, 0.0f,
	};
	
	static const GLfloat color_buffer_data[] = {
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
	};

	static const GLfloat color_buffer_data_2[] = {
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
	};
 
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data), color_buffer_data, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data_2), vertex_buffer_data_2, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data_2), color_buffer_data_2, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	rotAngle = 0;
	offsetX = 0;
}

void Scene2::Update(double dt)
{
	rotAngle += (float)(10 * dt); //magic number is no good, how to change?
	offsetX += (float)(10 * dt);
	if(offsetX > 80)
		offsetX -= 160;
}

void Scene2::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnableVertexAttribArray(0); // 1st attribute buffer : vertices
	glEnableVertexAttribArray(1); // 2nd attribute buffer : colors

	Mtx44 translate, rotate, scale;
	Mtx44 model;
	Mtx44 view;
	Mtx44 projection;
	Mtx44 MVP;
	translate.SetToIdentity();
	rotate.SetToIdentity();
	scale.SetToIdentity();

	view.SetToIdentity(); //no need camera for now, set it at World's origin
	projection.SetToOrtho(-80, 80, -60, 60, -1000, 1000); //Our world is a cube defined by these boundaries

	// -----------------------per geometry-------------------------- //

	scale.SetToScale(5, 5, 5);
	rotate.SetToRotation(-rotAngle, 0, 0, 1);
	model = translate * rotate * scale;
	MVP = projection * view * model; // Remember, matrix multiplication is the other way around
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]); //update the shader when mvp is changed
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, 3); // Draw the triangle !
	
	scale.SetToScale(5, 5, 5);
	rotate.SetToIdentity();
	translate.SetToTranslation(offsetX, 0, 0);
	model = translate * rotate * scale;
	MVP = projection * view * model; // Remember, matrix multiplication is the other way around
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, 3); // Draw the triangle !
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void Scene2::Exit()
{
	// Cleanup VBO
	glDeleteBuffers(2, &m_vertexBuffer[0]);
	glDeleteBuffers(2, &m_colorBuffer[0]);
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}
