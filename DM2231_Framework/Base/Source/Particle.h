#ifndef PARTICLE_H
#define PARTICLE_H

#include "Obj.h"

class CParticle : public CObj
{
public:
	CParticle(void);
	CParticle(int ID, Vector3 translate, Vector3 rotate, Vector3 scale, Vector3 size, Vector3 vel, bool gravity = false, bool active = false);
	virtual ~CParticle(void);

	virtual void Init(int ID, Vector3 translate, Vector3 rotate, Vector3 scale, Vector3 size, Vector3 vel, bool gravity = true, bool active = true);
	void Update(const double dt, float PosY);

	void SetVel(Vector3 vel);
	Vector3 GetVel();

	void SetGravity(bool gravity);
	bool GetGravity();

	void SetActive(bool active);
	bool GetActive();

private:
	Vector3 m_vel;
	bool m_bGravity;
	bool m_bActive;
};

#endif