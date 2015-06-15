#include "AmmoCrate.h"

float CAmmoCrate::MAX_RESPAWN_TIMER = 5.f;
float CAmmoCrate::AMMO_CRATE_OFSET = 5.f;
float CAmmoCrate::ROTATE_SPEED = 30.f;

CAmmoCrate::CAmmoCrate(int ID, Vector3 translate, Vector3 rotate, Vector3 scale, Vector3 size, bool active) : CObj(ID, translate, rotate, scale, size), active(active), respawnTimer(0.f)
{
	calcBound();
}


CAmmoCrate::~CAmmoCrate(void)
{
}

void CAmmoCrate::Init(bool active)
{
	this->active = active;
	this->rotate.y = 0;
	calcBound();
}

void CAmmoCrate::Update(const double dt)
{
	if (active)
	{
		this->rotate.y += (ROTATE_SPEED * dt);
		if (rotate.y >= 360)
		{
			rotate.y -= 360;
		}
	}
	else
	{
		if (respawnTimer < MAX_RESPAWN_TIMER)
		{
			respawnTimer += dt;
		}
		else
		{
			Init(true);
			respawnTimer = 0.f;
		}
	}
}

void CAmmoCrate::SetActive(bool active)
{
	this->active = active;
}

bool CAmmoCrate::GetActive()
{
	return active;
}

void CAmmoCrate::SetRespawnTimer(float respawnTimer)
{
	this->respawnTimer = respawnTimer;
}

float CAmmoCrate::GetRespawnTimer()
{
	return respawnTimer;
}

void CAmmoCrate::calcBound()
{
	maxBound.Set(translate.x + ((scale.x * size.x) / 2), translate.y + (scale.y * size.y), translate.z + ((scale.z * size.z) / 2));
	minBound.Set(translate.x - ((scale.x * size.x) / 2), translate.y, translate.z - (scale.z * size.z));
}