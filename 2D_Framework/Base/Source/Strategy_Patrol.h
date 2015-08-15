#ifndef STRATEGY_PATROL_H
#define STRATEGY_PATROL_H

#include "Strategy.h"
#include "Mesh.h"

class CStrategy_Patrol : public CStrategy
{
public:
	CStrategy_Patrol(void);
	~CStrategy_Patrol(void);

	virtual void Update(CMap *m_cMap, Vector3* Position, double dt);
	
private:
	// The Destination is the position of the target
	Vector3 FacingNormal;
	float ChangeFacingTimer;
	bool ChangeFacing;
};

#endif