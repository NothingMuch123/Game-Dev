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

void Skill::SetSpeed(float speed)
{
	m_speed = speed;
}

void Skill::SetScale(Vector3 scale)
{
	m_scale.Set(scale.x, scale.y, scale.z);
}

void Skill::SetDamage(int damage)
{
	m_damage = damage;
}

void Skill::SetRange(float range)
{
	m_range = range;
}

void Skill::SetCoolDown(float cooldown)
{
	m_coolDown = cooldown;
}

void Skill::SetPiercing(bool pierce)
{
	m_piercing = pierce;
}

float Skill::GetSpeed(void)
{
	return m_speed;
}

Vector3 Skill::GetScale(void)
{
	return m_scale;
}

int Skill::GetDamage(void)
{
	return m_damage;
}

float Skill::GetRange(void)
{
	return m_range;
}

float Skill::GetCoolDown(void)
{
	return m_coolDown;
}

bool Skill::GetPiercing(void)
{
	return m_piercing;
}
