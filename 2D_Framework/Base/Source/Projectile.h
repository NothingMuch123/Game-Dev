#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Vector2.h"
#include "Map.h"

class CProjectile
{
public:
	static float BULLET_SPEED;
	enum PROJ_TYPE
	{
		PROJ_NONE = 0,
		PROJ_BULLET,
		NUM_PROJ,
	};
	CProjectile(PROJ_TYPE type = PROJ_NONE, Vector2 pos = Vector2(0,0), Vector2 vel = Vector2(0,0), bool active = false);
	~CProjectile(void);

	void Init(PROJ_TYPE type, Vector2 pos, Vector2 vel, CMap *map, bool active = true);
	void Update(const float dt, CMap *map);
	void CalcBound(CMap *map);
	void Reset();

	PROJ_TYPE GetType();
	bool GetActive();
	Vector2 GetPos();
	Vector2 GetVel();
	Vector2 GetMinBound();
	Vector2 GetMaxBound();

private:
	Vector2 pos, vel, minBound, maxBound;
	bool active;
	PROJ_TYPE type;
};

#endif