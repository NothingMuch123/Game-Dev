#ifndef CHARACTER_H
#define CHARACTER_H

#include "Obj.h"

class CCharacter : public CObj
{
public:
	CCharacter(void);
	virtual ~CCharacter(void);

protected:
	int health;
};

#endif