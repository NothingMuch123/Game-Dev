#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Obj.h"

class CProjectile : public CObj
{
public:
	CProjectile(void);
	virtual ~CProjectile(void);

protected:
	Vector3 target;
};

#endif