#include "Particle.h"


CParticle::CParticle(void) : m_vel(Vector3(0,0,0)), m_bActive(false), m_bGravity(false)
{
}

CParticle::CParticle(int ID, Vector3 translate, Vector3 rotate, Vector3 scale, Vector3 size, Vector3 vel, bool gravity, bool active) : CObj(ID, translate, rotate, scale, size), m_vel(vel), m_bGravity(gravity), m_bActive(active)
{
}

CParticle::~CParticle(void)
{
}

void CParticle::Init(int ID, Vector3 translate, Vector3 rotate, Vector3 scale, Vector3 size, Vector3 vel, bool gravity, bool active)
{
	CObj::Init(ID, translate, rotate, scale, size);
	SetVel(vel);
	SetGravity(gravity);
	SetActive(active);
}

void CParticle::Update(const double dt, float PosY)
{
	if (this->m_bActive)
	{
		if (this->m_bGravity)
		{
			this->m_vel += CObj::m_gravity * dt;	// Update velocity
		}
		this->translate += m_vel * dt;			// Update position based on velocity

		// 
		if (this->translate.y < PosY)
		{
			this->m_bActive =  this->render = false;
		}
	}
}

void CParticle::SetVel(Vector3 vel)
{
	this->m_vel = vel;
}

Vector3 CParticle::GetVel()
{
	return m_vel;
}

void CParticle::SetGravity(bool gravity)
{
	this->m_bGravity = gravity;
}

bool CParticle::GetGravity()
{
	return m_bGravity;
}

void CParticle::SetActive(bool active)
{
	this->m_bActive = active;
}

bool CParticle::GetActive()
{
	return m_bActive;
}