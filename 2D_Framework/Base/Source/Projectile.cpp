#include "Projectile.h"

float CProjectile::BULLET_SPEED = 700.f;

CProjectile::CProjectile(PROJ_TYPE type, Vector2 pos, Vector2 vel, bool active) : Collidable(pos, active, NULL)
	, vel(vel)
	, type(type)
{
}

CProjectile::~CProjectile(void)
{
}

void CProjectile::Init(PROJ_TYPE type, Vector2 pos, Vector2 vel, CMap *map, bool active)
{
	this->type = type;
	this->vel = vel;

	Collidable::Init(pos, map, active);
}

void CProjectile::Update(const float dt, CMap *map)
{
	if (pos.x < 0 || pos.x > map->GetScreen_Width() || pos.y < 0 || pos.y > map->GetScreen_Height())
	{
		Reset();
	}
	if (active)
	{
		pos = pos + (vel * dt);
		CalcBound();
	}
}

void CProjectile::Reset()
{
	Collidable::Reset();

	this->type = PROJ_NONE;
	this->vel = Vector2(0,0);
}

CProjectile::PROJ_TYPE CProjectile::GetType()
{
	return type;
}

Vector2 CProjectile::GetVel()
{
	return vel;
}