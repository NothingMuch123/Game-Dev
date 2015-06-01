#ifndef SCENE_TERRAIN_H
#define SCENE_TERRAIN_H

#include "Projectile.h"
#include "Minimap.h"
#include "SceneBase.h"

class SceneTerrain : public SceneBase
{
public:
	enum WEAPON_ACTION
	{
		WA_NIL = 0,
		WA_FIRE,
		WA_RELOAD,
		WA_CHANGEWEAPON,
		WA_TOTAL,
	};

	SceneTerrain();
	~SceneTerrain();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	virtual void Reset();
	virtual void UpdateCameraStatus(unsigned char key);
	virtual void UpdateWeaponStatus(unsigned char key);

	void SetHUD(const bool m_bHUDmode);
	void RenderSkyPlane();
	void RenderTerrain();
	void RenderSkybox();
	void RenderTextInWorld();
	void RenderObject();
	void Render2D();

private:
	CMinimap *m_cMinimap; // Handle to the minimap

	// Vector of bullet;
	std::vector<CProjectile*> projectileList;
	float FireRateCounter;

	float rotateAngle;
};

#endif