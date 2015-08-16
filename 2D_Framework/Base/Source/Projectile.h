#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Collidable.h"

#include "Vector2.h"
#include "Map.h"

class CProjectile : public Collidable
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

	void Init(PROJ_TYPE type, Vector2 pos, Vector2 vel, float tileSize, bool active = true);
	void Update(const float dt, float screenWidth, float screenHeight);
	void Reset();

	PROJ_TYPE GetType();
	Vector2 GetVel();

private:
	Vector2 vel;
	PROJ_TYPE type;
};

#endif