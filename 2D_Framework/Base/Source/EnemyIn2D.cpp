#include "EnemyIn2D.h"


CEnemyIn2D::CEnemyIn2D(void) : theStrategy(NULL), Collidable()
{
}


CEnemyIn2D::~CEnemyIn2D(void)
{
	if(theStrategy != NULL)
	{
		delete theStrategy;
		theStrategy = NULL;
	}
}

// Initialise this class instance
void CEnemyIn2D::Init(Vector2 NewPosition, int gameLevel, Mesh* newMesh, ENEMY_TYPE newType, CMap* map)
{
	Collidable::Init(NewPosition, map, true);

	this->m_currentLevel = gameLevel;
	this->m_Mesh = newMesh;
	this->m_Type = newType;
}

// Set Mesh of the enemy
void CEnemyIn2D::SetMesh(Mesh* newMesh)
{
	this->m_Mesh = newMesh;
}

// Set Enemy type
void CEnemyIn2D::SetEnemyType(ENEMY_TYPE newType)
{
	this->m_Type = newType;
}

// Get gamelevel of the enemy	
int CEnemyIn2D::GetCurrentLevel()
{
	return m_currentLevel;
}

/********************************************************************************
Enemy Update
********************************************************************************/
void CEnemyIn2D::Update(CMap* m_cMap, double dt)
{
	if (theStrategy != NULL)
	{
		theStrategy->Update(m_cMap, &pos, dt);
	}
}

/********************************************************************************
Strategy
********************************************************************************/
void CEnemyIn2D::ChangeStrategy(CStrategy* theNewStrategy, bool bDelete)
{
	if (bDelete == true)
	{
		if (theStrategy != NULL)
		{
			delete theStrategy;
			theStrategy = NULL;
		}
	}
	
	theStrategy = theNewStrategy;
}

// Get CurrentStrategy
CStrategy* CEnemyIn2D::GetStrategy()
{
	return theStrategy;
}

// Get Mesh
Mesh* CEnemyIn2D::GetMesh()
{
	return m_Mesh;
}

// Get Facing
bool CEnemyIn2D::GetFacing()
{
	return m_FaceRight;
}

// Get Enemy type
CEnemyIn2D::ENEMY_TYPE CEnemyIn2D::GetEnemyType()
{
	return m_Type;
}
