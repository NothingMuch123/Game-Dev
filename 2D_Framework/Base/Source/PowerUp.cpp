#include "PowerUp.h"

PowerUp::PowerUp(Vector2 pos, bool active, ELEMENT_TYPE element, CMap *map) : Collidable(pos, active, map)
{
	m_element = element;
}


PowerUp::~PowerUp()
{
}

void PowerUp::Init(Vector2 pos, CMap * map, bool active, ELEMENT_TYPE element)
{
	Collidable::Init(pos, map, active);
	m_element = element;
}
