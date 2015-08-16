#ifndef POWER_UP_H
#define POWER_UP_H

#include "Collidable.h"
#include "Elemental.h"

class PowerUp : public Collidable, public Elemental
{
	public:
		enum POWER_UP_TYPE
		{
			FIRE_TYPE,
			WATER_TYPE,
			AIR_TYPE,
			NUM_POWER_UPS
		};

	private:
		POWER_UP_TYPE powerType;

	public:
		PowerUp(Vector2 pos = Vector2(0, 0), bool active = false, ELEMENT_TYPE element = NORMAL_TYPE, float tileSize = 0);
		virtual ~PowerUp();

		virtual void Init(Vector2 pos, float tileSize, bool active, ELEMENT_TYPE element);
};

#endif