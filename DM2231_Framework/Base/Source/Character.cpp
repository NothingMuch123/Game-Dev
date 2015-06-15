#include "Character.h"


CCharacter::CCharacter(void) : weaponList(NULL)
{
}

CCharacter::CCharacter(int ID, Vector3 translate, Vector3 rotate, Vector3 scale, Vector3 size, int health, int currentWeapon, std::string name, Camera3 camera, bool render) : CObj(ID, translate, rotate, scale, size, render), health(health), currentWeapon(currentWeapon), name(name), camera(camera)
{
	CWeapon *weapon = new CWeapon(CWeapon::W_SMG, Vector3(55, -35, 0), Vector3(0,0,0), Vector3(80,80,80), Vector3(1,1,1), 0.1f, 10, 2.f, 24, 72, 24, 120, true);
	AddWeapon(weapon);
	weapon = new CWeapon(CWeapon::W_PISTOL, Vector3(40, -40, 0), Vector3(0,0,0), Vector3(80,80,80), Vector3(1,1,1), 0.3f, 20, 1.5f, 12, 36, 12, 60, true);
	AddWeapon(weapon);
	weapon = new CWeapon(CWeapon::W_ROCKET_LAUNCHER, Vector3(40, -20, 0), Vector3(0,0,0), Vector3(80,80,80), Vector3(1,1,1), 1.5f, 100, 3.f, 1, 3, 1, 5, true);
	AddWeapon(weapon);
	weapon = new CWeapon(CWeapon::W_SNIPER, Vector3(55, -30, 0), Vector3(0,0,0), Vector3(80,80,80), Vector3(1,1,1), 1.f, 10, 3.f, 10, 30, 10, 50, true);
	AddWeapon(weapon);
	calcBound();
}

CCharacter::~CCharacter(void)
{
	for (std::vector<CWeapon*>::iterator it = weaponList.begin(); it != weaponList.end(); ++it)
	{
		CWeapon *w = (CWeapon*)*it;
		if (w)
		{
			delete w;
		}
	}
	weaponList.clear();
}

void CCharacter::SetHealth(int health)
{
	this->health = health;
}

int CCharacter::GetHealth()
{
	return health;
}

void CCharacter::SetCurrentWeapon(int currentWeapon)
{
	this->currentWeapon = currentWeapon;
}

int CCharacter::GetCurrentWeapon()
{
	return currentWeapon;
}

void CCharacter::SetName(std::string name)
{
	this->name = name;
}

std::string CCharacter::GetName()
{
	return name;
}

void CCharacter::SetCamera(Camera3 camera)
{
	this->camera = camera;
}

Camera3 &CCharacter::GetCamera()
{
	return camera;
}

void CCharacter::AddWeapon(CWeapon *weapon)
{
	weaponList.push_back(weapon);
}

std::vector<CWeapon*> &CCharacter::GetWeaponList()
{
	return weaponList;
}

void CCharacter::Reset()
{
	weaponList[0]->SetActiveAmmo(24);
	weaponList[0]->SetExtraAmmo(72);

	weaponList[1]->SetActiveAmmo(12);
	weaponList[1]->SetExtraAmmo(36);

	weaponList[2]->SetActiveAmmo(1);
	weaponList[2]->SetExtraAmmo(3);

	weaponList[3]->SetActiveAmmo(10);
	weaponList[3]->SetExtraAmmo(30);

	for (std::vector<CWeapon*>::iterator it = weaponList.begin(); it != weaponList.end(); ++it)
	{
		CWeapon *weapon = (CWeapon*)*it;
		weapon->Reset();
	}

	calcBound();
}

void CCharacter::calcBound()
{
	maxBound.Set(camera.position.x + ((scale.x * size.x) / 2), (camera.position.y - Camera3::TERRAIN_OFFSET) + (scale.y * size.y), camera.position.z + ((scale.z * size.z) / 2));
	minBound.Set(camera.position.x - ((scale.x * size.x) / 2), camera.position.y - Camera3::TERRAIN_OFFSET, camera.position.z - (scale.z * size.z));
}