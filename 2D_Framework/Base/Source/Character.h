#ifndef _CHARACTER_H
#define _CHARACTER_H

#include "Vector2.h"
#include "Map.h"

class CCharacter
{
public:
	CCharacter(Vector2 pos = Vector2(0,0), int jumpSpeed = 0, bool midAir_Up = false, bool midAir_Down = false, int animationCounter = 0, bool animationInvert = false);
	virtual ~CCharacter(void);

	void Update(const double dt);
	void Jump();
	void MoveUpDown(const bool mode, const float timeDiff);
	void MoveLeftRight(const bool mode, const float timeDiff);
	void Constrain(const int leftBorder, const int rightBorder, const int topBorder, const int bottomBorder, float timeDiff);

	void SetPos(Vector2 pos);
	Vector2 GetPos();

	void SetMidAir_Up(bool midAir_Up);
	bool GetMidAir_Up();

	void SetMidAir_Down(bool midAir_Down);
	bool GetMidAir_Down();

	void SetJumpSpeed(int jumpSpeed);
	int GetJumpSpeed();

	void SetAnimationInvert(bool animationInvert);
	bool GetAnimationInvert();

	void SetAnimationCounter(int animationCounter);
	int GetAnimationCounter();

private:
	Vector2 pos;
	bool midAir_Up, midAir_Down;
	int jumpSpeed;
	
	// Sprite animation
	// Change to sprite animation class
	bool animationInvert;
	int animationCounter;
};

#endif