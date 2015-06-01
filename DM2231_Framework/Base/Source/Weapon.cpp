#include "Weapon.h"


CWeapon::CWeapon(void)
{
}

CWeapon::CWeapon(int ID, Vector3 translate, Vector3 rotate, Vector3 scale, Vector3 size, float firerate, float damage, float reloadSpeed, int activeAmmo, int extraAmmo, int clipSize, int maxAmmo, bool render) : CObj(ID, translate, rotate, scale, size, render), firerate(firerate), damage(damage), reloadSpeed(reloadSpeed), activeAmmo(activeAmmo), extraAmmo(extraAmmo), clipSize(clipSize), maxAmmo(maxAmmo)
{
}

CWeapon::~CWeapon(void)
{
}

void CWeapon::SetFirerate(float firerate)
{
	this->firerate = firerate;
}

float CWeapon::GetFirerate()
{
	return firerate;
}

void CWeapon::SetDamage(float damage)
{
	this->damage = damage;
}

float CWeapon::GetDamage()
{
	return damage;
}

void CWeapon::SetReloadSpeed(float reloadSpeed)
{
	this->reloadSpeed = reloadSpeed;
}

float CWeapon::GetReloadSpeed()
{
	return reloadSpeed;
}

void CWeapon::SetActiveAmmo(int activeAmmo)
{
	this->activeAmmo = activeAmmo;
}

int CWeapon::GetActiveAmmo()
{
	return activeAmmo;
}

void CWeapon::SetExtraAmmo(int extraAmmo)
{
	this->extraAmmo = extraAmmo;
}

int CWeapon::GetExtraAmmo()
{
	return extraAmmo;
}

void CWeapon::SetClipSize(int clipSize)
{
	this->clipSize = clipSize;
}

int CWeapon::GetClipSize()
{
	return clipSize;
}

void CWeapon::SetMaxAmmo(int maxAmmo)
{
	this->maxAmmo = maxAmmo;
}

int CWeapon::GetMaxAmmo()
{
	return maxAmmo;
}

bool CWeapon::Fire(CProjectile *p, Camera3 *shooterCamera)
{
	if (this->activeAmmo <= 0) // No active ammo
	{
		return false;
	}
	else
	{
		if (this->GetID() == W_PISTOL)
		{
			p->Init(CProjectile::PROJ_BULLET, shooterCamera->target, Vector3(0, 0, 0), Vector3(1, 1, 1), Vector3(1, 1, 1), 500, 2, true, (shooterCamera->target - shooterCamera->position).Normalized());
			--(this->activeAmmo);
			return true;
		}
		else if (this->GetID() == W_ROCKET_LAUNCHER)
		{
			Vector3 dir = (shooterCamera->target - shooterCamera->position).Normalized();
			p->Init(CProjectile::PROJ_ROCKET, shooterCamera->target, Vector3(0, 0, 0), Vector3(1, 1, 1), Vector3(1, 1, 1), 500, 2, true, dir);
			--(this->activeAmmo);
			return true;
		}
		else if (this->GetID() == W_SNIPER)
		{
			p->Init(CProjectile::PROJ_BULLET, shooterCamera->target, Vector3(0, 0, 0), Vector3(1, 1, 1), Vector3(1, 1, 1), 500, 2, true, (shooterCamera->target - shooterCamera->position).Normalized());
			--(this->activeAmmo);
			return true;
		}
		else if (this->GetID() == W_MELEE)
		{
			return true;
		}
	}
}

bool CWeapon::Reload()
{
	if (this->reloadSpeed == -1)
	{
		return false;
	}
	if (this->extraAmmo > 0)
	{
		if (this->activeAmmo < this->clipSize)
		{
			int reloadAmmo = (this->clipSize - this->activeAmmo);
			if (this->extraAmmo >= reloadAmmo)
			{
				this->extraAmmo -= reloadAmmo;
				this->activeAmmo = this->clipSize;
				return true;
			}
			else
			{
				this->activeAmmo += this->extraAmmo;
				this->extraAmmo = 0;
				return true;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}