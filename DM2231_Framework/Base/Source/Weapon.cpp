#include "Weapon.h"
#include "Application.h"

CWeapon::CWeapon(void)
{
}

CWeapon::CWeapon(int ID, Vector3 translate, Vector3 rotate, Vector3 scale, Vector3 size, float firerate, float damage, float reloadSpeed, int activeAmmo, int extraAmmo, int clipSize, int maxAmmo, bool render) : CObj(ID, translate, rotate, scale, size, render), firerate(firerate), damage(damage), reloadSpeed(reloadSpeed), activeAmmo(activeAmmo), extraAmmo(extraAmmo), clipSize(clipSize), maxAmmo(maxAmmo), defaultTranslateY(translate.y)
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

bool CWeapon::Fire(CProjectile *p, Camera3 *shooterCamera, const double dt, double &recoil, irrklang::ISoundEngine *sound)
{
	if (this->activeAmmo <= 0) // No active ammo
	{
		return false;
	}
	else
	{
		Vector3 dir = (shooterCamera->target - shooterCamera->position).Normalized();
		if (this->GetID() == W_PISTOL)
		{
			p->Init(CProjectile::PROJ_BULLET, shooterCamera->position, Vector3(shooterCamera->total_pitch, shooterCamera->total_yaw, 0), Vector3(1, 1, 1), Vector3(1, 1, 1), 1200, 3, true, dir);
			--(this->activeAmmo);
			Application::camera_pitch -= 10 * dt;
			recoil += 10 * dt;
			sound->play2D("Sound//pistol_shoot.wav");
			return true;
		}
		else if (this->GetID() == W_ROCKET_LAUNCHER)
		{
			p->Init(CProjectile::PROJ_ROCKET, shooterCamera->position, Vector3(shooterCamera->total_pitch, shooterCamera->total_yaw, 0), Vector3(4, 4, 4), Vector3(1, 1, 1), 1000, 10, true, dir);
			--(this->activeAmmo);
			Application::camera_pitch -= 100 * dt;
			recoil += 100 * dt;
			sound->play2D("Sound//rocket_launcher_shoot.wav");
			return true;
		}
		else if (this->GetID() == W_SNIPER)
		{
			p->Init(CProjectile::PROJ_BULLET, shooterCamera->position, Vector3(shooterCamera->total_pitch, shooterCamera->total_yaw, 0), Vector3(1, 1, 1), Vector3(1, 1, 1), 1500, 5, true, dir);
			--(this->activeAmmo);
			Application::camera_pitch -= 50 * dt;
			recoil += 50 * dt;
			sound->play2D("Sound//sniper_shoot.wav");
			return true;
		}
		else if (this->GetID() == W_SMG)
		{
			p->Init(CProjectile::PROJ_BULLET, shooterCamera->position, Vector3(shooterCamera->total_pitch, shooterCamera->total_yaw, 0), Vector3(1, 1, 1), Vector3(1, 1, 1), 1500, 5, true, dir);
			--(this->activeAmmo);
			Application::camera_pitch -= 5 * dt;
			recoil += 5 * dt;
			sound->play2D("Sound//smg_shoot.wav");
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

void CWeapon::SetDefaultTranslateY(float defaultTranslateY)
{
	this->defaultTranslateY = defaultTranslateY;
}

float CWeapon::GetDefaultTranslateY()
{
	return defaultTranslateY;
}

void CWeapon::Reset()
{
	translate.y = defaultTranslateY;
}