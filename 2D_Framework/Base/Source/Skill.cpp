#include "Skill.h"

Skill::Skill()
{
}


Skill::~Skill()
{
}

void Skill::Init(float _speed, Vector3 _scale, int _damage, ELEMENT_TYPE _element, float _range, float _coolDown, bool _piercing)
{
	m_speed = _speed;
	m_scale = _scale;
	m_damage = _damage;
	m_element = _element;
	m_range = _range;
	m_coolDown = _coolDown;
	m_piercing = _piercing;
}

void Skill::Update(double dt)
{
	m_lastUsed += dt;
}

bool Skill::Use()
{
	// If cooled down
	if (IsSkillRead())
	{
		// Reset cooldown timer
		m_lastUsed = 0.0f;

		return true;
	}

	return false;
}

bool Skill::IsSkillRead(void)
{
	return m_lastUsed > m_coolDown;
}
