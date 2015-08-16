#include "SkillHitBox.h"



void SkillHitBox::setDefaultOwner()
{
	// Owner is set in the init function
}

SkillHitBox::SkillHitBox(PROJ_TYPE type, Vector2 pos, Vector2 vel, int damage, OWNER_TYPE owner, bool active) : CProjectile(PROJ_BULLET, pos, vel, active)
	, m_damage(damage)
{
}

SkillHitBox::~SkillHitBox()
{
}

void SkillHitBox::Init(PROJ_TYPE type, Vector2 pos, Vector2 vel, float tileSize, OWNER_TYPE owner, bool active)
{
	CProjectile::Init(type, pos, vel, tileSize, active);

	m_owner = owner;
}

void SkillHitBox::CalcBounds()
{
	Collidable::CalcBound();
}
