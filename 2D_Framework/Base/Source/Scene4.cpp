#include "Scene4.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"


Scene4::Scene4()
{
}

Scene4::~Scene4()
{
}

void Scene4::Init()
{
	
	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 
	
	glEnable(GL_CULL_FACE);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	//m_programID = LoadShaders( "SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader" );
	//m_programID = LoadShaders( "SimpleTransform.vertexshader", "SingleColor.fragmentshader" );
	m_programID = LoadShaders( "Shader//TransformVertexShader.vertexshader", "Shader//ColorFragmentShader.fragmentshader" );
	
	// Get a handle for our "MVP" uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	
	// Use our shader
	glUseProgram(m_programID);

	//remove all glGenBuffers, glBindBuffer, glBufferData code


	camera.Init(Vector3(40, 30, 30), Vector3(0, 0, 0), Vector3(0, 1, 0));

	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	meshList[GEO_RING1] = MeshBuilder::GenerateRing("ring1", Color(0, 1, 1), 36, 1.f,  0.6f);
	meshList[GEO_SUN] = MeshBuilder::GenerateSphere("sun", Color(1, 1, 0), 18, 36, 1.f);
	meshList[GEO_PLANET1] = MeshBuilder::GenerateSphere("planet1", Color(0, 1, 0), 18, 36, 1.f);
	meshList[GEO_PLANET2] = MeshBuilder::GenerateSphere("planet2", Color(1, 0, 1), 18, 36, 1.f);
	meshList[GEO_MOON2] = MeshBuilder::GenerateSphere("moon2", Color(1, 1, 1), 18, 36, 1.f);

	//meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", 2, 2);
	//meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", 1, 1, 1);
	//meshList[GEO_TORUS] = MeshBuilder::GenerateCylinder("torus", 36, 36, 5, 1);
	
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 units
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
	projectionStack.LoadMatrix(perspective);
	
	rotateAngle = 0;
}

void Scene4::Update(double dt)
{
	if(Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	if(Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);
	
	if(Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	rotateAngle += (float)(10 * dt);

	camera.Update(dt);
}

void Scene4::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Mtx44 MVP;

	glEnableVertexAttribArray(0); // 1st attribute buffer : vertices
	glEnableVertexAttribArray(1); // 2nd attribute buffer : colors
	
	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
						camera.position.x, camera.position.y, camera.position.z,
						camera.target.x, camera.target.y, camera.target.z,
						camera.up.x, camera.up.y, camera.up.z
					);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	meshList[GEO_AXES]->Render();
	//meshList[GEO_QUAD]->Render();
	//meshList[GEO_CUBE]->Render();

	//meshList[GEO_RING1]->Render();

	modelStack.PushMatrix(); //push - entire solar system
	modelStack.PushMatrix(); //push - sun only
	modelStack.Scale(10, 10, 10);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	meshList[GEO_SUN]->Render();
	modelStack.PopMatrix(); //pop - sun only

	modelStack.PushMatrix(); //push - planet2 subsystem
	modelStack.Rotate(rotateAngle, 0, 1, 0);
	modelStack.Translate(-20, 0, 0);
	modelStack.PushMatrix(); //push - planet2 only
	modelStack.Scale(2, 2, 2);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	meshList[GEO_PLANET2]->Render();
	modelStack.PopMatrix(); //pop - planet2 only
	modelStack.PushMatrix(); //push - moon2
	modelStack.Rotate(90, 0, 0, 1);
	modelStack.Rotate(2.f * rotateAngle, 0, 1, 0);
	modelStack.Translate(3, 0, 0);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	meshList[GEO_MOON2]->Render();
	modelStack.PopMatrix(); //pop - moon2
	modelStack.PopMatrix(); //pop - planet2 subsystem
	
	modelStack.PushMatrix(); //push - planet1 subsystem
	modelStack.Rotate(1.2f * rotateAngle, 0, 1, 0);
	modelStack.Translate(40, 0, 0);
	modelStack.PushMatrix(); //push - planet1 only
	modelStack.Scale(3, 3, 3);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	meshList[GEO_PLANET1]->Render();
	modelStack.PopMatrix(); //pop - planet1 only
	modelStack.PushMatrix(); //push - ring1
	modelStack.Rotate(30, 0, 0, 1);
	modelStack.Rotate(2.f * rotateAngle, 0, 1, 0);
	modelStack.Scale(5, 5, 5);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	meshList[GEO_RING1]->Render();
	modelStack.PopMatrix(); //pop - ring1
	modelStack.PopMatrix(); //pop - planet1 subsystem
	modelStack.PopMatrix(); //pop - entire solar system

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void Scene4::Exit()
{
	// Cleanup VBO
	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if(meshList[i])
			delete meshList[i];
	}
	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);
}
