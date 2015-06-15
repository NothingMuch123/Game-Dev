#ifndef AMMOCRATE_H
#define AMMOCRATE_H

#include "Obj.h"

class CAmmoCrate : public CObj
{
public:
	static float MAX_RESPAWN_TIMER;
	static float AMMO_CRATE_OFSET;
	static float ROTATE_SPEED;

	CAmmoCrate(int ID = NULL, Vector3 translate = Vector3(0,0,0), Vector3 rotate = Vector3(0,0,0), Vector3 scale = Vector3(1,1,1), Vector3 size = Vector3(1,1,1), bool active = false);
	virtual ~CAmmoCrate(void);

	virtual void Init(bool active);
	void Update(const double dt);

	void SetActive(bool active);
	bool GetActive();

	void SetRespawnTimer(float respawnTimer);
	float GetRespawnTimer();

	virtual void calcBound();

private:
	bool active;
	float respawnTimer;
};

#endif