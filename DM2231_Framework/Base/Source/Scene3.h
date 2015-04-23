#ifndef SCENE_3_H
#define SCENE_3_H

#include "Scene.h"
//#include <glm/glm.hpp>
#include "Mtx44.h"
#include "Camera2.h"
#include "Mesh.h"

class Scene3 : public Scene
{
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_TOTAL,
	};
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_SUN,
		GEO_PLANET1,
		GEO_PLANET2,
		GEO_MOON1,
		GEO_RING1,
		GEO_QUAD,
		GEO_CUBE,
		GEO_TORUS,
		NUM_GEOMETRY,
	};
public:
	Scene3();
	~Scene3();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
private:
	unsigned m_vertexArrayID;
	//unsigned m_vertexBuffer[NUM_GEOMETRY];
	//unsigned m_colorBuffer[NUM_GEOMETRY];
	//unsigned m_indexBuffer[NUM_GEOMETRY];
	Mesh* meshList[NUM_GEOMETRY];
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	float rotateAngle;

	Camera2 camera;
};

#endif