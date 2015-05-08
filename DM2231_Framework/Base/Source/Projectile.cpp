#include "Projectile.h"


CProjectile::CProjectile(void) : speed(NULL), lifetime(NULL), m_bStatus(false)
{
}


CProjectile::~CProjectile(void)
{
}

void CProjectile::Init(int ID, Vector3 translate, Vector3 rotate, Vector3 scale, Vector3 size, const float speed, const float lifetime, const bool status, Vector3 direction,  bool render)
{
	SetSpeed(speed);
	SetLifetime(lifetime);
	SetStatus(status);
	SetDirection(direction);
	CObj::Init(ID, translate, rotate, scale, size, render);
}

void CProjectile::SetStatus(const bool bStatus)
{
	this->m_bStatus = bStatus;
}

bool CProjectile::GetStatus()
{
	return m_bStatus;
}

void CProjectile::SetSpeed(float speed)
{
	this->speed = speed;
}

float CProjectile::GetSpeed()
{
	return speed;
}

void CProjectile::SetLifetime(float lifetime)
{
	this->lifetime = lifetime;
}

float CProjectile::GetLifetime()
{
	return lifetime;
}

void CProjectile::SetDirection(Vector3 direction)
{
	this->direction = direction;
}

Vector3 CProjectile::GetDirection()
{
	return direction;
}

void CProjectile::Update(const double dt)
{
	// Check if the projectile is active
	if (GetStatus())
	{
		// Update the position of the projectile
		SetTranslate(GetTranslate() + GetDirection() * GetSpeed() * dt);
	}

	// Update lifetime
	SetLifetime(GetLifetime() - dt);

	// Check if lifetime is over
	if (GetLifetime() <= 0)
	{
		SetStatus(false);
	}
}