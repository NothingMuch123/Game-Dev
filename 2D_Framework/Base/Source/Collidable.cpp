#include "Collidable.h"

Collidable::Collidable(Vector2 pos, bool active, CMap *map)
	: pos(pos)
	, active(active)
{
	if (map != NULL)
	{
		tileSize = map->GetTileSize();
	}
}


Collidable::~Collidable()
{
}

void Collidable::Init(Vector2 pos, CMap *map, bool active)
{
	this->pos = pos;
	this->active = active;

	if (map != NULL)
	{
		this->tileSize = map->GetTileSize();
		CalcBound();
	}
}

void Collidable::CalcBound()
{
	minBound.Set(pos.x, pos.y);
	maxBound.Set(pos.x + tileSize * 0.125f, pos.y + tileSize * 0.125f);
}

void Collidable::Reset()
{
	this->active = false;
	this->pos = this->minBound = this->maxBound = Vector2(0, 0);
}

bool Collidable::GetActive()
{
	return this->active;
}

Vector2 Collidable::GetPos()
{
	return pos;
}

Vector2 Collidable::GetMinBound()
{
	return minBound;
}

Vector2 Collidable::GetMaxBound()
{
	return maxBound;
}

bool Collidable::CollideWith(Collidable* other)
{
	if (GetActive()  && other->GetActive() &&
		GetMinBound().x < other->GetMaxBound().x && GetMaxBound().x > other->GetMinBound().x &&
		GetMinBound().y < other->GetMaxBound().y && GetMaxBound().y > other->GetMinBound().y) // Collide
	{
		return true;
	}

	return false;
}
