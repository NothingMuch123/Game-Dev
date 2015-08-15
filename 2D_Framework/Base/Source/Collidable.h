#ifndef COLLIDABLE_H
#define COLLIDABLE_H

#include "Vector2.h"
#include "Map.h"

class Collidable
{
	public:
		Collidable(Vector2 pos = Vector2(0, 0), bool active = false, CMap *map = NULL);
		virtual ~Collidable();

	virtual void Init(Vector2 pos, CMap *map, bool active);
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
		Vector2 pos, minBound, maxBound;
};

#endif