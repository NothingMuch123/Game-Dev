#ifndef SCENE_5_H
#define SCENE_5_H

#include "Scene.h"
#include "Mtx44.h"
#include "Camera2.h"
#include "Mesh.h"
#include "MatrixStack.h"

class Scene5 : public Scene
{
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_TOTAL,
	};
	enum GEOMETRY_TYPE
	{
		GEO_AXES = 0,
		GEO_QUAD,
		GEO_CUBE1,
		GEO_CUBE2,
		GEO_SPHERE,
		NUM_GEOMETRY,
	};
public:
	Scene5();
	~Scene5();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
private:
	void RenderBody();
	void RenderLimb();
	void RenderRobot();
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