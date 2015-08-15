#ifndef ENEMY_H
#define ENEMY_H

#include "Map.h"
#include "Vector2.h"
#include "SpriteAnimation.h"

class CEnemy
{
public:
	static float WALK_SPEED, GRAVITY;
	enum ENEMY_ACTION
	{
		EA_NONE = 0,
		EA_LEFT,
		NUM_EA,
	};
	enum ENEMY_ANIMATION
	{
		E_ANIM_NONE = 0,
		E_ANIM_WALK_LEFT,
		NUM_E_ANIM,
	};
	CEnemy(Vector2 pos = Vector2(0,0), Vector2 scale = Vector2(1,1), bool midAir_Down = false, bool dir = false, bool active = false);
	~CEnemy(void);

	// Initialise this class instance
	void Init(Vector2 pos, Vector2 scale = Vector2(1,1), bool midAir_Down = false, bool dir = false, bool active = true);
	virtual void Update(double dt, CMap* map); // Enemy Update

	void MoveLeftRight(const bool mode, const float timeDiff, CMap *map);

	void SetPos(Vector2 pos);
	Vector2 GetPos();
	Vector2 GetScale();
	Vector2 GetMinBound();
	Vector2 GetMaxBound();

	void SetMidAir_Down(bool midAir_Down);
	bool GetMidAir_Down();

	void SetDir(bool dir);
	bool GetDir();

	void CalcBound(CMap *map);
	bool GetActive();

	void Reset();

	void SetAnimation(ENEMY_ANIMATION type, SpriteAnimation* sa);
	SpriteAnimation* GetCurrentAnimation();

private:
	Vector2 pos, scale; // Within screen size
	float jumpSpeed;
	bool midAir_Down;
	bool dir; // Left = False | Right = True
	Vector2 minBound, maxBound; // Bounds for AABB collision
	bool active;
	
	// Sprite animation
	SpriteAnimation* animationList[NUM_E_ANIM];
	ENEMY_ANIMATION currentAnim;
};

#endif