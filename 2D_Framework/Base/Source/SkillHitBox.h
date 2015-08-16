#ifndef SKILL_HIT_BOX_H
#define SKILL_HIT_BOX_H

#include "Projectile.h"
#include "IOwnerID.h"

class SkillHitBox :	public CProjectile, public IOwnerID
{
	int m_damage;

	protected:
		void setDefaultOwner();

	public:
		SkillHitBox(PROJ_TYPE type = PROJ_NONE, Vector2 pos = Vector2(0, 0), Vector2 vel = Vector2(0, 0), int damage = 0, OWNER_TYPE owner = PLAYER_TYPE, bool active = false);
		virtual ~SkillHitBox();

		void Init(PROJ_TYPE type, Vector2 pos, Vector2 vel, float tileSize, OWNER_TYPE owner, bool active = true);

		void CalcBounds();
};

#endif