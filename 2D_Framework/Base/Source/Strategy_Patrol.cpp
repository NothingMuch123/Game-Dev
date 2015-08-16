#include "Strategy_Patrol.h"


CStrategy_Patrol::CStrategy_Patrol(void)
{
	this->FacingNormal.Set(1, 0);
	this->ChangeFacingTimer = 1.f;
	this->ChangeFacing = false;
}


CStrategy_Patrol::~CStrategy_Patrol(void)
{
}

void CStrategy_Patrol::Update(CMap *m_cMap, Vector2* Position, double dt)
{
	// m_cMap is a map containing all the bound location

	ChangeFacingTimer -= dt;

	if(ChangeFacingTimer <= 0)
	{
		ChangeFacingTimer = 1.f;
		ChangeFacing = !ChangeFacing;
		FacingNormal = -FacingNormal;
	}


	/*int checkPosition_X = (int) ((m_cMap->GetMapOffset().x + Position->x) / m_cMap->GetTileSize());
	int checkPosition_Y = m_cMap->GetNumOfTiles_Height() - (int) ceil( (float)(Position->y) / m_cMap->GetTileSize());*/
	
	Vector2 Velocity = FacingNormal * 200;
	//Check going left or right
	if(FacingNormal.x > 0)
	{
		Vector2 tilePos( (int)ceil((float)(m_cMap->GetMapOffset().x + ((Position->x + m_cMap->GetTileSize()) + (Velocity.x * dt))) / m_cMap->GetTileSize()) , m_cMap->GetNumOfTiles_Height() - ((int) ceil( Position->y / m_cMap->GetTileSize()) + 1) ); // New position if move
		if(m_cMap->theScreenMap[tilePos.y][tilePos.x+1] == CMap::TILE_NONE)
		{
			*Position =  *Position + Velocity * dt;
		}
	}
	else if(FacingNormal.x < 0)
	{
		Vector2 tilePos( (m_cMap->GetMapOffset().x + (Position->x - (Velocity.x * dt))) / m_cMap->GetTileSize() , m_cMap->GetNumOfTiles_Height() - (int) (ceil( Position->y / m_cMap->GetTileSize()) + 1) ); // New position if move
		if(m_cMap->theScreenMap[tilePos.y][tilePos.x] == CMap::TILE_NONE)
		{
			*Position = *Position + Velocity * dt;
		}
	}
}