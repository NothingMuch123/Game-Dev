#ifndef SCENE_TEXT_H
#define SCENE_TEXT_H

#include "Scene.h"
#include "Mtx44.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "Minimap.h"
#include "Map.h"
#include "Vector2.h"
#include "Character.h"
#include "Projectile.h"
#include "Target.h"
#include "Enemy.h"
#include "EnemySpawner.h"
#include "EnemyIn2D.h"
#include "Strategy_Patrol.h"
#include "Collidable.h"
#include "Skill.h"
#include "SkillHitBox.h"

class SceneText : public Scene
{
	static float MAX_SHOOT_TIME;
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
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_CROSSHAIR,
		GEO_LIGHTBALL,
		GEO_SPHERE,
		GEO_SPHERE2,
		GEO_SPHERE3,
		GEO_SPHERE4,
		GEO_SPHERE5,
		GEO_SPHERE6,
		GEO_SPHERE7,
		GEO_SPHERE8,
		GEO_SPHERE9,
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
		GEO_GRASS_DARKGREEN,
		GEO_GRASS_LIGHTGREEN,
		GEO_BACKGROUND,
		GEO_TILEGROUND,
		GEO_TILETREE,
		GEO_TILESTRUCTURE,
		GEO_TILEHERO,
		GEO_OBJECT,
		GEO_TEXT,
		GEO_LIVE,

		// Projectile
		GEO_BULLET,

		// Tile
		GEO_TILE_DIRT,
		GEO_TILE_GRASS,
		GEO_TILE_STONE,
		GEO_TILE_SLAB,
		GEO_TILE_FLOATING,

		GEO_TILE_CONCRETE,
		GEO_TILE_CONCRETE_TOP,

		// Target
		GEO_TARGET_CLOSE,

		NUM_GEOMETRY,
	};

public:
	SceneText();
	~SceneText();

	virtual void Init();
	virtual void InitCharacter();
	virtual void Update(double dt);
	virtual void UpdateCameraStatus(const unsigned char key, const bool status = true); // Update Camera status
	virtual void UpdateCharacterStatus(const CCharacter::CHARACTER_ACTION action, const bool status = true); // Update Character status
	virtual void Render();
	virtual void Exit();

	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshIn2D(Mesh *mesh, bool enableLight, float size=1.0f, float x=0.0f, float y=0.0f, bool rotate=false);
	void RenderMesh(Mesh *mesh, bool enableLight);
	void RenderGround();
	void RenderSkybox();
	void RenderBackGround();
	void Render2DMesh(Mesh *mesh, const bool enableLight, const float size=1.0f, const float x=0.0f, const float y=0.0f, const float rotateY=0.f, const float rotateZ = 0.f);

	void InitProjList();
	void RenderCollideList();
	CProjectile* FetchProj();

	void RenderEnemyList();

	void RenderTargetList(std::vector<CTarget*> targetList, CMap *map);

	enum WEAPON_ACTION
	{
		WA_NIL = 0,
		WA_FIRE,
		WA_RELOAD,
		WA_CHANGEWEAPON,
		WA_TOTAL,
	};

private:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	Camera3 camera;

	float rotateAngle;

	MS modelStack;
	MS viewStack;
	MS projectionStack;

	Light lights[2];

	bool bLightEnabled;

	float fps;

	// Handle to the minimap
	CMinimap* m_cMinimap;

	// Handle to the tilemaps
	CMap* m_cMap;
	void RenderTileMap(CMap *map);

	// Handle to screen-based map
	vector<CMap*> levelMaps;

	// Character information
	std::vector<CCharacter*> characterList;
	void RenderCharacter();

	// Codes for parallax scrolling
	//CMap *m_cRearMap;
	//void RenderRearTileMap();

	// Projectile
	//std::vector<CProjectile*> projList;
	float shootTimer;

	// Collidables
	std::vector<Collidable*> collideList;

	// Game details
	int level;
	float score;

	// Target
	//std::vector<CTarget*> targetList_lvl1, targetList_lvl2;

	// Enemy
	vector<CEnemyIn2D*> enemyList;
	//std::vector<CEnemy*> enemyList;
	//std::vector<CEnemySpawner*> enemySpawnerList_lvl1, enemySpawnerList_lvl2;

	// Game data
	/*bool gameEnded;
	float levelTime;*/
	int lives;

	// Attack
	void characterAttack(Skill skill);
	SkillHitBox* fetchSkillHitBox(void);
};

#endif