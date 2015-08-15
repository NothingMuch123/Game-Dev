#ifndef ENEMY_IN_2D_H
#define ENEMY_IN_2D_H

#include "Strategy.h"

#include "Mesh.h"
#include "Collidable.h"

class CEnemyIn2D : public Collidable
{
public:
	CEnemyIn2D(void);
	~CEnemyIn2D(void);

	enum ENEMY_TYPE
	{
		ENEMY_NONE = 0,
		ENEMY_GROUND,
		ENEMY_AIR,

		NUM_ENEMY_TYPE,
	};

	// Initialise this class instance
	void Init(Vector2 NewPosition, int gameLevel, Mesh* newMesh, ENEMY_TYPE newType, CMap* map);
	// Set Mesh of the enemy
	void SetMesh(Mesh* newMesh);
	// Set Enemy type
	void SetEnemyType(ENEMY_TYPE newtype);
	// Get gamelevel of the enemy
	int GetCurrentLevel();
	// ENEMY Update
	void Update(CMap* m_cMap, double dt);
	// Change Strategy
	void ChangeStrategy(CStrategy* theNewStrategy, bool bDelete=true);
	// Get CurrentStrategy
	CStrategy* GetStrategy();
	// Get Mesh
	Mesh* GetMesh();
	// Get Facing
	bool GetFacing();
	// Get Enemy type
	ENEMY_TYPE GetEnemyType();

private:
	// Enemy's gamelevel
	int m_currentLevel;
	// The Destination is the position of the Hero
	CStrategy* theStrategy;
	// Enemy's Mesh
	Mesh* m_Mesh;
	// Enemy's Facing;
	bool m_FaceRight;
	// Enemy type
	ENEMY_TYPE m_Type;
};

#endif