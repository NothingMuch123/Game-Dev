#ifndef TARGET_H
#define TARGET_H

#include "Vector2.h"
#include "SpriteAnimation.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"

class CTarget
{
public:
	enum TARGET_TYPE
	{
		TARGET_NONE = 0,
		TARGET_RED,
		TARGET_LIGHT_BLUE,
		TARGET_DARK_BLUE,
		TARGET_GREEN,
		TARGET_YELLOW,
		TARGET_WHITE,
		TARGET_DESTROY,
		NUM_TARGET,
	};
	CTarget(TARGET_TYPE type = TARGET_NONE, Vector2 pos = Vector2(0,0), Vector2 scale = Vector2(1,1), float openTime = 1.f, bool open = false, bool active = false);
	~CTarget(void);

	void Init(TARGET_TYPE type, Vector2 pos, Vector2 scale, float openTime, bool open = false, bool active = true);
	void Update(const double dt);

	void SetType(TARGET_TYPE type);
	void SetActive(bool active);
	TARGET_TYPE GetType();
	bool GetActive();
	bool GetOpen();
	float GetOpenTime();
	Vector2 GetPos();
	Vector2 GetScale();
	Vector2 GetMinBound();
	Vector2 GetMaxBound();
	SpriteAnimation* GetSA();

	void CalcBound();

private:
	Vector2 pos, scale, minBound, maxBound;
	bool active, open;
	float openTime, timer;
	TARGET_TYPE type;
	SpriteAnimation *sa;
};

#endif