#ifndef ENEMY_H
#define ENEMY_H

#include "Character.h"
#include "Vector2.h"
#include "Strategy.h"

class CEnemy : public CCharacter
{
public:
	CEnemy(Vector2 pos = Vector2(0,0), int jumpSpeed = 0, bool midAir_Up = false, bool midAir_Down = false, int animationCounter = 0, bool animationInvert = false, CStrategy *strategy = NULL);
	~CEnemy(void);

	// Initialise this class instance
	void Init(void);
	virtual void Update(double dt, CMap* map); // Enemy Update

	//void ChangeStrategy(CStrategy* strategy, bool bDelete=true); // Strategy
	
	/*void SetPos(Vector2 pos); // Set position of this enemy
	Vector2 GetPos(); // Get position of this enemy

	void SetDestination(const Vector2 destination); // Set the destination of this enemy
	Vector2 GetDestination(void); // Get the destination of this enemy
	
	void SetAnimationInvert(bool ENEMYAnimationInvert); // Set Animation Invert status of the enemy
	bool GetAnimationInvert(void); // Get Animation Invert status of the enemy
	
	void SetAnimationCounter(int animationCounter); // Set Animation Counter of the enemy
	int GetAnimationCounter(void); // Get Animation Counter of the enemy*/

private:
	//CStrategy* strategy;
};

#endif