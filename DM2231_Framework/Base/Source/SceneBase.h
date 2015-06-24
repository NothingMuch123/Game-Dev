#ifndef SCENEBASE_H
#define SCENEBASE_H

#include "Scene.h"
#include "irrKlang.h"
#include "Mtx44.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include <vector>
#include "LoadHmap.h"
#include "Obj.h"

class SceneBase : public Scene
{
public:
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHTENABLED,
		U_NUMLIGHTS,
		U_LIGHT0_TYPE,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		U_LIGHT1_TYPE,
		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_COLOR_TEXTURE_ENABLED1,
		U_COLOR_TEXTURE1,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,

		// Fog
		U_FOG_COLOR,
		U_FOG_START,
		U_FOG_END,
		U_FOG_DENSITY,
		U_FOG_TYPE,
		U_FOG_ENABLE,
		U_TOTAL,
	};
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_CROSSHAIR,
		GEO_SKYPLANE,
		GEO_TERRAIN,
		GEO_SEA,
		GEO_SPRITE_ANIMATION,
		GEO_LIGHTBALL,
		GEO_SPHERE,
		GEO_QUAD,
		GEO_CUBE,
		GEO_RING,
		GEO_CONE,
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,
		GEO_OBJECT,
		GEO_TEXT,

		GEO_PLATFORM,
		GEO_TARGET,

		// Weapons
		GEO_ROCKET_LAUNCHER,
		GEO_PISTOL,
		GEO_SNIPER,
		GEO_SMG,
		GEO_SCOPE,

		// Ammo
		GEO_PISTOL_BULLET,
		GEO_ROCKET_BULLET,
		GEO_SNIPER_BULLET,
		GEO_SMG_BULLET,
		GEO_AMMO_CRATE,

		GEO_MINIMAP_AMMOCRATE, // Ammo crate icon for minimap
		GEO_MINIMAP_TARGET,
		GEO_HEALTH,
		GEO_UI_BORDER,

		// Particle
		GEO_RAIN_PARTICLE,

		NUM_GEOMETRY,
	};

	SceneBase(void);
	virtual ~SceneBase(void);

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	virtual void Reset();

	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMesh(Mesh *mesh, bool enableLight);
	void RenderMeshIn2D(Mesh *mesh, bool enableLight, float size = 1.0f, float x = 0.0f, float y = 0.0f, float rotate = 0.f);

protected:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	Camera3 camera;

	MS modelStack;
	MS viewStack;
	MS projectionStack;

	Light lights[2];

	bool bLightEnabled;
	bool fogEnabled;

	float fps;
	float fov;

	// Object
	std::vector<CObj*> objList;
	CObj *obj;
	
	// Terrain
	std::vector<unsigned char> m_heightMap;
	Vector3 terrainSize;
};

#endif