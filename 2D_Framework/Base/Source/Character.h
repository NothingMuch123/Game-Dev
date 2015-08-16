#ifndef _CHARACTER_H
#define _CHARACTER_H

#include "Elemental.h"
#include "Collidable.h"
#include "IOwnerID.h"

#include "Vector2.h"
#include "Map.h"
#include "SpriteAnimation.h"
#include <iostream>
#include "Skill.h"

class CCharacter : public Elemental, public Collidable, public IOwnerID
{
public:
	static float WALK_SPEED, RUN_SPEED, GRAVITY;
	enum CHARACTER_ACTION
	{
		CA_NONE = 0,
		CA_WALK_LEFT,
		CA_WALK_RIGHT,
		CA_JUMP,
		CA_ATTACK,
		CA_SKILL,
		NUM_CA,
	};
	enum CHARACTER_ANIMATION
	{
		// Idle
		ANIM_IDLE = 0,

		// Walk
		ANIM_WALK,

		// Jump
		ANIM_JUMP,

		// Attack
		ANIM_ATTACK,

		// Skill
		ANIM_SKILL,

		NUM_ANIM,
	};
	CCharacter(Vector2 pos = Vector2(0,0), Vector2 scale = Vector2(1,1), CMap* map = NULL, bool defaultLookingLeft = false, float jumpSpeed = 0.f, bool midAir_Up = false, bool midAir_Down = false, bool dir = true);
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

	void SetAnimation(ELEMENT_TYPE eType, CHARACTER_ANIMATION caType, Animation* a);
	SpriteAnimation* GetSprite();

	void CheckReset(CHARACTER_ANIMATION type);

	void CalcBound(void);

	bool GetAction(CHARACTER_ACTION action);

	CHARACTER_ANIMATION GetCurrentAnim();

	void SetSprite(ELEMENT_TYPE type, SpriteAnimation* sprite);

	bool GetFlip(void) const;		// If true, flip the image

	Skill GetBasicSkill();
	Skill GetAdvancedSkill();

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
	SpriteAnimation* spriteList[NUM_ELEMENTS];
	Animation* animationList[NUM_ELEMENTS][NUM_ANIM];
	CHARACTER_ANIMATION currentAnim;

	void spriteSetToLeft(void);
	void spriteSetToRight(void);

	// Skills
	Skill m_basicSkill;
	Skill m_advancedSkill;
};

#endif