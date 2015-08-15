#ifndef POWER_UP_H
#define POWER_UP_H

#include "Collidable.h"
#include "Elemental.h"

class PowerUp : public Collidable, public Elemental
{
	public:
		PowerUp(Vector2 pos = Vector2(0, 0), bool active = false, ELEMENT_TYPE element = NORMAL_TYPE, CMap *map = NULL);
		virtual ~PowerUp();

		virtual void Init(Vector2 pos, CMap *map, bool active, ELEMENT_TYPE element);
};

#endif