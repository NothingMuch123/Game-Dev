#ifndef WEAPON_H
#define WEAPON_H

#include "Obj.h"
#include "Projectile.h"
#include "Camera3.h"

class CWeapon : public CObj
{
public:
	enum WEAPON_TYPE
	{
		W_MELEE = 0,
		W_PISTOL,
		W_ROCKET_LAUNCHER,
		W_SNIPER,
		W_TOTAL,
	};

	CWeapon(void);
	CWeapon(int ID, Vector3 translate, Vector3 rotate, Vector3 scale, Vector3 size, float firerate, float damage, float reloadSpeed, int activeAmmo, int extraAmmo, int clipSize, int maxAmmo, bool render = true);
	virtual ~CWeapon(void);

	void SetFirerate(float firerate);
	float GetFirerate();

	void SetDamage(float damage);
	float GetDamage();

	void SetReloadSpeed(float reloadSpeed);
	float GetReloadSpeed();

	void SetActiveAmmo(int activeAmmo);
	int GetActiveAmmo();

	void SetExtraAmmo(int extraAmmo);
	int GetExtraAmmo();

	void SetClipSize(int clipSize);
	int GetClipSize();

	void SetMaxAmmo(int maxAmmo);
	int GetMaxAmmo();

	bool Fire(CProjectile *p, Camera3 *shooterCamera);
	bool Reload();

protected:
	float firerate, damage, reloadSpeed;
	int activeAmmo, extraAmmo, clipSize, maxAmmo;
};

#endif