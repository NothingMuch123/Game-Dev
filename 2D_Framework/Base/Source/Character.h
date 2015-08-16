#ifndef _CHARACTER_H
#define _CHARACTER_H

#include "Elemental.h"
#include "Collidable.h"

#include "Vector2.h"
#include "Map.h"
#include "SpriteAnimation.h"
#include <iostream>

class CCharacter : public Elemental, public Collidable
{
public:
	static float WALK_SPEED, RUN_SPEED, GRAVITY;
	enum CHARACTER_ACTION
	{
		CA_NONE = 0,
		CA_LOOK_UP,
		CA_LOOK_DOWN,
		CA_LEFT,
		CA_RIGHT,
		CA_JUMP,
		CA_RUN,
		CA_LIE,
		CA_SHOOT,
		NUM_CA,
	};
	enum CHARACTER_ANIMATION
	{
		// Idle
		ANIM_IDLE_LEFT = 0,
		ANIM_IDLE_RIGHT,

		// Run
		ANIM_RUN_LEFT,
		ANIM_RUN_RIGHT,

		// Move and shoot
		ANIM_MOVE_LEFT_SHOOT,
		ANIM_MOVE_RIGHT_SHOOT,

		// Jump
		ANIM_JUMP_LEFT,
		ANIM_JUMP_RIGHT,

		// Lie down
		ANIM_LIE_LEFT,
		ANIM_LIE_RIGHT,

		// Look up
		ANIM_LOOK_UP_LEFT,
		ANIM_LOOK_UP_RIGHT,

		// Look down
		ANIM_LOOK_DOWN_LEFT,
		ANIM_LOOK_DOWN_RIGHT,

		// Move left shoot diagonal
		ANIM_MOVE_LEFT_SHOOT_TOP,
		ANIM_MOVE_LEFT_SHOOT_DOWN,

		// Move right shoot diagonal
		ANIM_MOVE_RIGHT_SHOOT_TOP,
		ANIM_MOVE_RIGHT_SHOOT_DOWN,

		// Fall
		ANIM_FALL_LEFT,
		ANIM_FALL_RIGHT,

		NUM_ANIM,
	};
	CCharacter(Vector2 pos = Vector2(0,0), Vector2 scale = Vector2(1,1), CMap* map = NULL, bool defaultLookingLeft, float jumpSpeed = 0.f, bool midAir_Up = false, bool midAir_Down = false, bool dir = true);
	virtual ~CCharacter(void);

	virtual void Update(const double dt, CMap *m_cMap);
	void Jump();
	void MoveUpDown(const bool mode, const float timeDiff);
	void MoveLeftRight(const bool mode, const float timeDiff, CMap *map);
	void Constrain(float timeDiff, CMap *m_cMap);

	Vector2 GetScale();

	void SetMidAir_Up(bool midAir_Up);
	bool GetMidAir_Up();

	void SetMidAir_Down(bool midAir_Down);
	bool GetMidAir_Down();

	void SetJumpSpeed(int jumpSpeed);
	int GetJumpSpeed();

	void SetDir(bool dir);
	bool GetDir();

	void SetActions(const CHARACTER_ACTION action, const bool status);

	void SetAnimation(CHARACTER_ANIMATION type, Animation* a);
	SpriteAnimation* GetSprite();

	void CheckReset(CHARACTER_ANIMATION type);

	void CalcBound(void);

	bool GetAction(CHARACTER_ACTION action);

	CHARACTER_ANIMATION GetCurrentAnim();

	void SetSprite(SpriteAnimation* sprite);

	bool GetFlip(void);		// If true, flip the image

protected:
	Vector2 scale; // Within screen size
	bool midAir_Up, midAir_Down;
	float jumpSpeed;
	bool actions[NUM_CA];
	bool dir; // Left = False | Right = True
	float current_speed;
	Vector2 minBound, maxBound; // Bounds for AABB collision
	bool fallingThrough;
	float fallingThroughDist;
	bool defaultLook;				// The default direction that the character is facing in the sprites; True == Left
	bool flipSprite;				// Controls if the sprite should be flipped
	
	// Sprite animation
	SpriteAnimation *sprite;
	Animation* animationList[NUM_ANIM];
	CHARACTER_ANIMATION currentAnim;

	void spriteSetToLeft(void);
	void spriteSetToRight(void);
};

#endif