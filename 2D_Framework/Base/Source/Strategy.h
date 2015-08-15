#ifndef STRATEGY_H
#define STRATEGY_H

#include "Vector3.h"
#include "Map.h"

class CStrategy
{
public:
	CStrategy(void);
	~CStrategy(void);
	virtual void Update(CMap*, Vector3*, double)=0;
};

#endif