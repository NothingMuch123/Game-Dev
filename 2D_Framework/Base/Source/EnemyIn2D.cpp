#include "EnemyIn2D.h"


CEnemyIn2D::CEnemyIn2D(void) : theStrategy(NULL)
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
void CEnemyIn2D::Init(Vector3 NewPosition, int gameLevel, Mesh* newMesh, ENEMY_TYPE newType)
{
	this->m_Position = NewPosition;
	this->m_currentLevel = gameLevel;
	this->m_Mesh = newMesh;
	this->m_Type = newType;
	this->m_Active = true;
}

// Set position of the enemy
void CEnemyIn2D::SetPos(Vector3 NewPosition)
{
	m_Position = NewPosition;
}

// Set Mesh of the enemy
void CEnemyIn2D::SetMesh(Mesh* newMesh)
{
	this->m_Mesh = newMesh;
}

// Set Active
void CEnemyIn2D::SetActive(bool newActive)
{
	this->m_Active = newActive;
}

// Set Enemy type
void CEnemyIn2D::SetEnemyType(ENEMY_TYPE newType)
{
	this->m_Type = newType;
}

// Get position of the enemy
Vector3 CEnemyIn2D::GetPos()
{
	return m_Position;
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
		theStrategy->Update(m_cMap, &m_Position, dt);
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

// Get Active
bool CEnemyIn2D::GetActive()
{
	return m_Active;
}

// Get Enemy type
CEnemyIn2D::ENEMY_TYPE CEnemyIn2D::GetEnemyType()
{
	return m_Type;
}
