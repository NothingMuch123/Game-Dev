#include "Character.h"


CCharacter::CCharacter(Vector2 pos, int jumpSpeed, bool midAir_Up, bool midAir_Down, int animationCounter, bool animationInvert) :	
					pos(pos),
					jumpSpeed(jumpSpeed),
					midAir_Up(midAir_Up),
					midAir_Down(midAir_Down),
					animationCounter(animationCounter),
					animationInvert(animationInvert)
{
}

CCharacter::~CCharacter(void)
{
}

void CCharacter::Update(const double dt, CMap *m_cMap, CMap *m_cRearMap)
{
	// Update Hero's info 
	if (midAir_Up == false && midAir_Down == false) 
	{ 
		// Don't jump 
	} 
	else if (midAir_Up == true && midAir_Down == false) 
	{ 
		// Check if the hero can move up into mid air...
		int checkPosition_X = (int) ((m_cMap->GetMapOffset().x + pos.x) / m_cMap->GetTileSize()); 
		int checkPosition_Y = m_cMap->GetNumOfTiles_Height() - (int) ceil( (float) (pos.y + jumpSpeed) / m_cMap->GetTileSize()); 
		if (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == CMap::TILE_GROUND || m_cMap->theScreenMap[checkPosition_Y][checkPosition_X + 1] == CMap::TILE_GROUND) 
		{ 
			// Since the new position does not allow the hero to move into, then go back to the old position
			pos.y = ((int) (pos.y / m_cMap->GetTileSize())) * m_cMap->GetTileSize();
			midAir_Up = false;
			jumpSpeed = 0; 
		} 
		else 
		{ 
			pos.y += jumpSpeed;
			jumpSpeed -= 1; 
			if (jumpSpeed == 0)
			{ 
				midAir_Up = false; 
				midAir_Down = true; 
			}
		} 
	} 
	else if (midAir_Up == false && midAir_Down == true) 
	{ 
		// Check if the hero is still in mid air...
		int checkPosition_X = (int) ((m_cMap->GetMapOffset().x + pos.x) / m_cMap->GetTileSize());
		if (checkPosition_X < 0)
		{
			checkPosition_X = 0;
		}
		if (checkPosition_X > m_cMap->GetNumOfTiles_Width())
		{
			checkPosition_X = m_cMap->GetNumOfTiles_Width();
		}
		int checkPosition_Y = m_cMap->GetNumOfTiles_Height() - (int) ceil( (float) (pos.y - jumpSpeed) / m_cMap->GetTileSize()); 
		if (checkPosition_Y < 0)
		{
			checkPosition_Y = 0;
		}
		if (checkPosition_Y > m_cMap->GetNumOfTiles_Height())
		{
			checkPosition_Y = m_cMap->GetNumOfTiles_Height();
		}
		if (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == 1 || m_cMap->theScreenMap[checkPosition_Y][checkPosition_X + 1] == 1) 
		{ 
			// Since the new position does not allow the hero to move into, then go back to the old position
			pos.y = ((int) (pos.y / m_cMap->GetTileSize())) * m_cMap->GetTileSize();
			midAir_Down = false;
			jumpSpeed = 0; 
		}
		else
		{
			pos.y -= jumpSpeed;
			jumpSpeed += 1;
		}
	}

	Constrain(25, 750, 25, 575, 1.f, m_cMap);
	//m_cMap->GetTileOffset().x = (int) (m_cMap->GetMapOffset().x / m_cMap->GetTileSize());
	m_cMap->SetTileOffset( Vector2((int) (m_cMap->GetMapOffset().x / m_cMap->GetTileSize()) , m_cMap->GetTileOffset().y) );
	if (m_cMap->GetTileOffset().x + m_cMap->GetNumOfTiles_Width() > m_cMap->GetNumOfTiles_MapWidth())
	{
		//m_cMap->GetTileOffset().x = m_cMap->GetNumOfTiles_MapWidth() - m_cMap->GetNumOfTiles_Width();
		m_cMap->SetTileOffset( Vector2(m_cMap->GetNumOfTiles_MapWidth() - m_cMap->GetNumOfTiles_Width() , m_cMap->GetTileOffset().y) );
	}
}

void CCharacter::Jump()
{
	if (midAir_Up == false && midAir_Down == false) 
	{
		midAir_Up = true;
		jumpSpeed = 10;
	}
}

void CCharacter::MoveUpDown(const bool mode, const float timeDiff)
{
	if (mode)
	{ 
		pos.y = pos.y + (int) (5.0f * timeDiff); 
	}
	else 
	{ 
		pos.y = pos.y - (int) (5.0f * timeDiff); 
	} 
}

void CCharacter::MoveLeftRight(const bool mode, const float timeDiff)
{
	if (mode)
	{
		pos.x = pos.x - (int) (5.0f * timeDiff);
		animationInvert = true;
		--animationCounter;
		if (animationCounter < 0)
		{
			animationCounter = 3;
		}
	}
	else 
	{
		pos.x = pos.x + (int) (5.0f * timeDiff);
		animationInvert = false;
		++animationCounter;
		if (animationCounter > 3)
		{
			animationCounter = 0;
		}
	}
}

void CCharacter::Constrain(const int leftBorder, const int rightBorder, const int topBorder, const int bottomBorder, float timeDiff, CMap *m_cMap)
{
	if (pos.x < leftBorder)
	{
		pos.x = leftBorder;
		//m_cMap->GetMapOffset().x = m_cMap->GetMapOffset().x - (int)(5.f * timeDiff);
		m_cMap->SetMapOffset( Vector2(m_cMap->GetMapOffset().x - (int)(5.f * timeDiff) , m_cMap->GetMapOffset().y) );
		if (m_cMap->GetMapOffset().x < 0)
		{
			//m_cMap->GetMapOffset().x = 0;
			m_cMap->SetMapOffset( Vector2(0, m_cMap->GetMapOffset().y) );
		}
	}
	else if (pos.x > rightBorder)
	{
		pos.x = rightBorder;
		//m_cMap->GetMapOffset().x = m_cMap->GetMapOffset().x + (int)(5.f * timeDiff);
		m_cMap->SetMapOffset( Vector2(m_cMap->GetMapOffset().x + (int)(5.f * timeDiff), m_cMap->GetMapOffset().y) );
		if (m_cMap->GetMapOffset().x > (m_cMap->GetMap_Width() - m_cMap->GetScreen_Width()))
		{
			//m_cMap->GetMapOffset().x = m_cMap->GetMap_Width() - m_cMap->GetScreen_Width();
			m_cMap->SetMapOffset( Vector2(m_cMap->GetMap_Width() - m_cMap->GetScreen_Width(), m_cMap->GetMapOffset().y) );
		}
	}

	if (pos.y < topBorder)
	{
		pos.y = topBorder;
	}
	else if (pos.y > bottomBorder)
	{
		pos.y = bottomBorder;
	}
}

void CCharacter::SetPos(Vector2 pos)
{
	this->pos = pos;
}

Vector2 CCharacter::GetPos()
{
	return pos;
}

void CCharacter::SetMidAir_Up(bool midAir_Up)
{
	this->midAir_Up = midAir_Up;
}

bool CCharacter::GetMidAir_Up()
{
	return midAir_Up;
}

void CCharacter::SetMidAir_Down(bool midAir_Down)
{
	this->midAir_Down = midAir_Down;
}

bool CCharacter::GetMidAir_Down()
{
	return midAir_Down;
}

void CCharacter::SetJumpSpeed(int jumpSpeed)
{
	this->jumpSpeed = jumpSpeed;
}

int CCharacter::GetJumpSpeed()
{
	return jumpSpeed;
}

void CCharacter::SetAnimationInvert(bool animationInvert)
{
	this->animationInvert = animationInvert;
}

bool CCharacter::GetAnimationInvert()
{
	return animationInvert;
}

void CCharacter::SetAnimationCounter(int animationCounter)
{
	this->animationCounter = animationCounter;
}

int CCharacter::GetAnimationCounter()
{
	return animationCounter;
}