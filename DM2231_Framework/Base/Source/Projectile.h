#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Obj.h"

class CProjectile : public CObj
{
public:
	enum PROJ_TYPE
	{
		PROJ_NONE = 0,
		PROJ_BULLET,
		PROJ_ROCKET,
	};

	CProjectile(void);
	virtual ~CProjectile(void);
	void Init(int ID, Vector3 translate, Vector3 rotate, Vector3 scale, Vector3 size, const float speed, const float lifetime, const bool status, Vector3 direction, bool render = true);
	void InitNULL();
	void Update(const double dt);

	void SetStatus(const bool bStatus);
	bool GetStatus();

	void SetSpeed(float speed);
	float GetSpeed();

	void SetLifetime(float lifetime);
	float GetLifetime();

	void SetDirection(Vector3 direction);
	Vector3 GetDirection();

protected:
	Vector3 direction;
	bool m_bStatus; // Indicate the status of this projectile
	float speed;
	float lifetime;
};

#endif