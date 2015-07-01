#include "Scene3.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
// Include GLM
//#include <glm/gtc/matrix_transform.hpp>
//using namespace glm;


Scene3::Scene3()
{
}

Scene3::~Scene3()
{
}

void Scene3::Init()
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

	// Or, for an ortho camera :
	//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates
	
	// Our ModelViewProjection : multiplication of our 3 matrices
	//glm::mat4 MVP        = Projection * View * Model; // Remember, matrix multiplication is the other way around

	//static const GLfloat vertex_buffer_data[] = { 
	//	-0.5,0.5,0.5,
	//	-0.5,-0.5,0.5,
	//	0.5,-0.5,0.5,
	//	0.5,0.5,0.5,
	//	-0.5,0.5,-0.5,
	//	-0.5,-0.5,-0.5,
	//	0.5,-0.5,-0.5,
	//	0.5,0.5,-0.5,
	//	//-1.0f,-1.0f,-1.0f,
	//	//-1.0f,-1.0f, 1.0f,
	//	//-1.0f, 1.0f, 1.0f,
	//	// 1.0f, 1.0f,-1.0f,
	//	//-1.0f,-1.0f,-1.0f,
	//	//-1.0f, 1.0f,-1.0f,
	//	// 1.0f,-1.0f, 1.0f,
	//	//-1.0f,-1.0f,-1.0f,
	//	// 1.0f,-1.0f,-1.0f,
	//	// 1.0f, 1.0f,-1.0f,
	//	// 1.0f,-1.0f,-1.0f,
	//	//-1.0f,-1.0f,-1.0f,
	//	//-1.0f,-1.0f,-1.0f,
	//	//-1.0f, 1.0f, 1.0f,
	//	//-1.0f, 1.0f,-1.0f,
	//	// 1.0f,-1.0f, 1.0f,
	//	//-1.0f,-1.0f, 1.0f,
	//	//-1.0f,-1.0f,-1.0f,
	//	//-1.0f, 1.0f, 1.0f,
	//	//-1.0f,-1.0f, 1.0f,
	//	// 1.0f,-1.0f, 1.0f,
	//	// 1.0f, 1.0f, 1.0f,
	//	// 1.0f,-1.0f,-1.0f,
	//	// 1.0f, 1.0f,-1.0f,
	//	// 1.0f,-1.0f,-1.0f,
	//	// 1.0f, 1.0f, 1.0f,
	//	// 1.0f,-1.0f, 1.0f,
	//	// 1.0f, 1.0f, 1.0f,
	//	// 1.0f, 1.0f,-1.0f,
	//	//-1.0f, 1.0f,-1.0f,
	//	// 1.0f, 1.0f, 1.0f,
	//	//-1.0f, 1.0f,-1.0f,
	//	//-1.0f, 1.0f, 1.0f,
	//	// 1.0f, 1.0f, 1.0f,
	//	//-1.0f, 1.0f, 1.0f,
	//	// 1.0f,-1.0f, 1.0f
	//};

	//// One color for each vertex. They were generated randomly.
	//static const GLfloat color_buffer_data[] = { 
	//	0.583f,  0.771f,  0.014f,
	//	0.609f,  0.115f,  0.436f,
	//	0.327f,  0.483f,  0.844f,
	//	0.822f,  0.569f,  0.201f,
	//	0.435f,  0.602f,  0.223f,
	//	0.310f,  0.747f,  0.185f,
	//	0.597f,  0.770f,  0.761f,
	//	0.559f,  0.436f,  0.730f,
	//	//0.359f,  0.583f,  0.152f,
	//	//0.483f,  0.596f,  0.789f,
	//	//0.559f,  0.861f,  0.639f,
	//	//0.195f,  0.548f,  0.859f,
	//	//0.014f,  0.184f,  0.576f,
	//	//0.771f,  0.328f,  0.970f,
	//	//0.406f,  0.615f,  0.116f,
	//	//0.676f,  0.977f,  0.133f,
	//	//0.971f,  0.572f,  0.833f,
	//	//0.140f,  0.616f,  0.489f,
	//	//0.997f,  0.513f,  0.064f,
	//	//0.945f,  0.719f,  0.592f,
	//	//0.543f,  0.021f,  0.978f,
	//	//0.279f,  0.317f,  0.505f,
	//	//0.167f,  0.620f,  0.077f,
	//	//0.347f,  0.857f,  0.137f,
	//	//0.055f,  0.953f,  0.042f,
	//	//0.714f,  0.505f,  0.345f,
	//	//0.783f,  0.290f,  0.734f,
	//	//0.722f,  0.645f,  0.174f,
	//	//0.302f,  0.455f,  0.848f,
	//	//0.225f,  0.587f,  0.040f,
	//	//0.517f,  0.713f,  0.338f,
	//	//0.053f,  0.959f,  0.120f,
	//	//0.393f,  0.621f,  0.362f,
	//	//0.673f,  0.211f,  0.457f,
	//	//0.820f,  0.883f,  0.371f,
	//	//0.982f,  0.099f,  0.879f
	//};

	//static const GLuint index_buffer_data[] = {
	//	0, 1, 3,
	//	2, 3, 1,
	//	3, 2, 7,
	//	6, 7, 2,
	//	7, 6, 4,
	//	5, 4, 6,
	//	4, 5, 0,
	//	1, 0, 5,
	//	4, 0, 7,
	//	3, 7, 0,
	//	1, 5, 2,
	//	0, 2, 5,
	//};

	//glGenBuffers(1, &m_vertexBuffer);
	//glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);
	//
	//glGenBuffers(1, &m_colorBuffer);
	//glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data), color_buffer_data, GL_STATIC_DRAW);
	//
	//glGenBuffers(1, &m_indexBuffer);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_buffer_data), index_buffer_data, GL_STATIC_DRAW);
 
	//glBindVertexArray(0);

	//camera.position.Set(4, 4, 3); // Camera is at (4,3,3), in World Space
	//camera.target.Set(0, 0, 0); // and looks at the origin
	//camera.up.Set(0, 1, 0);  // Head is up (set to 0,-1,0 to look upside-down)

	//remove all glGenBuffers, glBindBuffer, glBufferData code
	camera.Init(Vector3(10, 8, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));

	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	meshList[GEO_RING1] = MeshBuilder::GenerateRing("ring1", Color(1, 1, 1), 36, 1.f,  0.8f);
	meshList[GEO_SUN] = MeshBuilder::GenerateSphere("sun", Color(1, 1, 1), 18, 36, 5.f);
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 0), 1);
	//meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", 1, 1, 1);
	//meshList[GEO_TORUS] = MeshBuilder::GenerateCylinder("torus", 36, 36, 5, 1);
	
	rotateAngle = 0;
}

void Scene3::Update(double dt)
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

void Scene3::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnableVertexAttribArray(0); // 1st attribute buffer : vertices
	glEnableVertexAttribArray(1); // 2nd attribute buffer : colors

	Mtx44 projectionMat;
	Mtx44 viewMat;
	Mtx44 modelMat;
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 units
	projectionMat.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
	// Camera matrix
	viewMat.SetToLookAt(
						camera.position.x, camera.position.y, camera.position.z,
						camera.target.x, camera.target.y, camera.target.z,
						camera.up.x, camera.up.y, camera.up.z
					);
	// Model matrix : an identity matrix (model will be at the origin)
	modelMat.SetToIdentity();


	Mtx44 MVP = projectionMat * viewMat * modelMat; // Remember, matrix multiplication is the other way around
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	
	//glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	meshList[GEO_AXES]->Render();
	meshList[GEO_QUAD]->Render();
	//meshList[GEO_CUBE]->Render();

	//meshList[GEO_RING1]->Render();
	//meshList[GEO_SUN]->Render();

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void Scene3::Exit()
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
