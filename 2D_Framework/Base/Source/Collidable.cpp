#include "Collidable.h"

Collidable::Collidable(Vector2 pos, bool active, float tileSize)
	: pos(pos)
	, active(active)
{

	this->tileSize = tileSize;
}


Collidable::~Collidable()
{
}

void Collidable::Init(Vector2 pos, float tileSize, bool active)
{
	this->pos = pos;
	this->active = active;


	this->tileSize = tileSize;
	CalcBound();

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

void Collidable::SetActive(bool _active)
{
	active = _active;
}

void Collidable::SetPos(Vector2 _pos)
{
	pos = _pos;
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
