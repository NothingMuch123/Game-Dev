#include "Target.h"


CTarget::CTarget(void)
{
	Init(CTarget::TARGET_NONE, Vector3(0,0,0), Vector3(-90,0,0), Vector3(8,8,8), Vector3(1,1,1), false);
}


CTarget::~CTarget(void)
{
}

void CTarget::Init(int ID, Vector3 translate, Vector3 rotate, Vector3 scale, Vector3 size, bool active, float lifetime, bool render)
{
	SetLifetime(lifetime);
	SetActive(active);
	SetHit(false);
	if (ID == TARGET_MOVING)
	{
		left_right = rand() % 2;
	}
	CObj::Init(ID, translate, rotate, scale, size, render);
	calcBound();
}

void CTarget::Update(const double dt)
{
	static const float ROTATE_SPEED = 50.f;
	static const float MOVE_SPEED = 50.f;
	lifetime -= dt;
	if (lifetime <= 0)
	{
		hit = true;
	}
	if (this->hit)
	{
		if (rotate.x > -90)
		{
			rotate.x -= ROTATE_SPEED * dt;
			if (rotate.x <= -90)
			{
				rotate.x = -90;
				active = false;
				hit = false;
			}
		}
	}
	else
	{
		if (rotate.x < 0)
		{
			rotate.x += ROTATE_SPEED * dt;
			if (rotate.x > 0)
			{
				rotate.x = 0;
			}
		}
		if (this->ID == TARGET_MOVING)
		{
			if (translate.x <= (-400 + (scale.x * size.x)) || translate.x >= 400 - (scale.x * size.x))
			{
				left_right = !left_right;
			}
			if (left_right && rotate.x == 0)	// Move right
			{
				translate.x += MOVE_SPEED * dt;
				calcBound();
			}
			else if (rotate.x == 0)			// Move left
			{
				translate.x -= MOVE_SPEED * dt;
				calcBound();
			}
		}
	}
}

void CTarget::calcBound()
{
	maxBound.Set(translate.x + ((scale.x * size.x) / 2), translate.y + (scale.y * size.y), translate.z + ((scale.z * size.z) / 2));
	minBound.Set(translate.x - ((scale.x * size.x) / 2), translate.y, translate.z - (scale.z * size.x));
}

void CTarget::SetActive(bool active)
{
	this->active = active;
}

bool CTarget::GetActive()
{
	return active;
}

void CTarget::SetHit(bool hit)
{
	this->hit = hit;
}

bool CTarget::GetHit()
{
	return hit;
}

void CTarget::SetLifetime(float lifetime)
{
	this->lifetime = lifetime;
}

float CTarget::GetLifetime()
{
	return lifetime;
}