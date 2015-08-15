#include "Projectile.h"

float CProjectile::BULLET_SPEED = 700.f;

CProjectile::CProjectile(PROJ_TYPE type, Vector2 pos, Vector2 vel, bool active) : type(type), pos(pos), vel(vel), active(active)
{
}

CProjectile::~CProjectile(void)
{
}

void CProjectile::Init(PROJ_TYPE type, Vector2 pos, Vector2 vel, CMap *map, bool active)
{
	this->type = type;
	this->pos = pos;
	this->vel = vel;
	this->active = active;
	CalcBound(map);
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
		CalcBound(map);
	}
}

void CProjectile::CalcBound(CMap *map)
{
	minBound.Set(pos.x, pos.y);
	maxBound.Set(pos.x + map->GetTileSize() * 0.125f, pos.y + map->GetTileSize() * 0.125f);
}

void CProjectile::Reset()
{
	this->type = PROJ_NONE;
	this->active = false;
	this->pos = this->vel = this->minBound = this->maxBound = Vector2(0,0);
}

CProjectile::PROJ_TYPE CProjectile::GetType()
{
	return type;
}

bool CProjectile::GetActive()
{
	return this->active;
}

Vector2 CProjectile::GetPos()
{
	return pos;
}

Vector2 CProjectile::GetVel()
{
	return vel;
}

Vector2 CProjectile::GetMinBound()
{
	return minBound;
}

Vector2 CProjectile::GetMaxBound()
{
	return maxBound;
}