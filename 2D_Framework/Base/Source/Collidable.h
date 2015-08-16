#ifndef COLLIDABLE_H
#define COLLIDABLE_H

#include "Vector2.h"

class Collidable
{
public:
	Collidable(Vector2 pos = Vector2(), Vector2 scale = Vector2(1, 1), bool active = false, float tileSize = 0);
	virtual ~Collidable();

	virtual void Init(Vector2 pos, float tileSize, bool active);
	virtual void CalcBound();
	virtual void Reset();

	void SetActive(bool _active);
	void SetPos(Vector2 _pos);

	bool GetActive();
	Vector2 GetPos();
	Vector2 GetMinBound();
	Vector2 GetMaxBound();

	bool CollideWith(Collidable* other);

protected:
	bool active;
	int tileSize;
	Vector2 pos, scale, minBound, maxBound;
};

#endif