#ifndef SKILL_H
#define SKILL_H

#include "Elemental.h"

#include "Vector3.h"

class Skill : Elemental
{
	float m_speed;
	Vector3 m_scale;
	int m_damage;
	float m_range;
	float m_coolDown;
	float m_lastUsed;
	bool m_piercing;

	public:
		Skill();
		virtual ~Skill();

		void Init(float _speed, Vector3 _scale, int _damage, ELEMENT_TYPE _element, float _range, float _coolDown, bool _piercing);
		void Update(double dt);

		virtual bool Use();

		bool IsSkillRead(void);

		// Setters
		void SetSpeed(float speed);
		void SetScale(Vector3 scale);
		void SetDamage(int damage);
		void SetRange(float range);
		void SetCoolDown(float cooldown);
		void SetPiercing(bool pierce);

		// Getters
		float GetSpeed(void);
		Vector3 GetScale(void);
		int GetDamage(void);
		float GetRange(void);
		float GetCoolDown(void);
		bool GetPiercing(void);
};

#endif