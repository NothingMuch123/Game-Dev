#ifndef CHARACTER_H
#define CHARACTER_H

#include "Weapon.h"

class CCharacter : public CObj
{
public:
	enum CHAR_TYPE
	{
		CHAR_PLAYER = 0,
		CHAR_ENEMY,
	};

	CCharacter(void);
	CCharacter(int ID, Vector3 translate, Vector3 rotate, Vector3 scale, Vector3 size, int health, int currentWeapon, std::string name, Camera3 camera, bool render = true);
	virtual ~CCharacter(void);

	void SetHealth(int health);
	int GetHealth();

	void SetCurrentWeapon(int currentWeapon);
	int GetCurrentWeapon();

	void SetName(std::string name);
	std::string GetName();

	void SetCamera(Camera3 camera);
	Camera3 &GetCamera();

	void AddWeapon(CWeapon *weapon);
	std::vector<CWeapon*> &GetWeaponList();

	void Reset();
	virtual void calcBound();

protected:
	int health, currentWeapon;
	std::string name;
	Camera3 camera;
	std::vector<CWeapon*> weaponList;
};

#endif