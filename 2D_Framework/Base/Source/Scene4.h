#ifndef SCENE_4_H
#define SCENE_4_H

#include "Scene.h"
#include "Mtx44.h"
#include "Camera2.h"
#include "Mesh.h"
#include "MatrixStack.h"

class Scene4 : public Scene
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
		GEO_MOON2,
		GEO_RING1,
		GEO_QUAD,
		GEO_CUBE,
		GEO_TORUS,
		NUM_GEOMETRY,
	};
public:
	Scene4();
	~Scene4();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
private:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	Camera2 camera;

	float rotateAngle;

	MS modelStack;
	MS viewStack;
	MS projectionStack;
};

#endif