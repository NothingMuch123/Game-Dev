#include "PowerUp.h"

PowerUp::PowerUp(Vector2 pos, bool active, ELEMENT_TYPE element, float tileSize) : Collidable(pos, active, tileSize)
{
	m_element = element;
}


PowerUp::~PowerUp()
{
}

void PowerUp::Init(Vector2 pos, float tileSize, bool active, ELEMENT_TYPE element)
{
	Collidable::Init(pos, tileSize, active);
	m_element = element;
}
