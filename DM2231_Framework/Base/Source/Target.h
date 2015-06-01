#ifndef TARGET_H
#define TARGET_H

#include "Obj.h"

class CTarget : public CObj
{
public:
	enum TARGET_TYPE
	{
		TARGET_NONE = 0,
		TARGET_STANDING,
		TARGET_MOVING,
		NUM_TARGET,
	};
	CTarget(void);
	virtual ~CTarget(void);

	void Init(int ID, Vector3 translate, Vector3 rotate, Vector3 scale, Vector3 size, bool active = true, float lifetime = 0.f, bool render = true);
	void Update(const double dt);
	virtual void calcBound();

	void SetActive(bool active);
	bool GetActive();

	void SetHit(bool hit);
	bool GetHit();

	void SetLifetime(float lifetime);
	float GetLifetime();

private:
	bool active, hit, left_right;
	float lifetime;
};

#endif