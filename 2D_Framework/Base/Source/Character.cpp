#include "Character.h"


CCharacter::CCharacter(Vector2 pos, int jumpSpeed, bool midAir_Up, bool midAir_Down, int animationCounter, bool animationInvert) :	pos(pos),
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

void CCharacter::Update(const double dt)
{
	/*// Update Hero's info 
	if (midAir_Up == false && midAir_Down == false) 
	{ 
		// Don't jump 
	} 
	else if (midAir_Up == true && midAir_Down == false) 
	{ 
		// Check if the hero can move up into mid air...
		int checkPosition_X = (int) ((mapOffset.x + HeroPosition.x) / m_cMap->GetTileSize()); 
		int checkPosition_Y = m_cMap->GetNumOfTiles_Height() - (int) ceil( (float) (HeroPosition.y + jumpspeed) / m_cMap->GetTileSize()); 
		if (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == TILE_GROUND || m_cMap->theScreenMap[checkPosition_Y][checkPosition_X + 1] == TILE_GROUND) 
		{ 
			// Since the new position does not allow the hero to move into, then go back to the old position
			HeroPosition.y = ((int) (HeroPosition.y / m_cMap->GetTileSize())) * m_cMap->GetTileSize();
			midAir_Up = false;
			jumpspeed = 0; 
		} 
		else 
		{ 
			HeroPosition.y += jumpspeed;
			jumpspeed -= 1; 
			if (jumpspeed == 0)
			{ 
				midAir_Up = false; 
				midAir_Down = true; 
			}
		} 
	} 
	else if (midAir_Up == false && midAir_Down == true) 
	{ 
		// Check if the hero is still in mid air...
		int checkPosition_X = (int) ((mapOffset.x + HeroPosition.x) / m_cMap->GetTileSize());
		if (checkPosition_X < 0)
		{
			checkPosition_X = 0;
		}
		if (checkPosition_X > m_cMap->GetNumOfTiles_Width())
		{
			checkPosition_X = m_cMap->GetNumOfTiles_Width();
		}
		int checkPosition_Y = m_cMap->GetNumOfTiles_Height() - (int) ceil( (float) (HeroPosition.y - jumpspeed) / m_cMap->GetTileSize()); 
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
			HeroPosition.y = ((int) (HeroPosition.y / m_cMap->GetTileSize())) * m_cMap->GetTileSize();
			midAir_Down = false;
			jumpspeed = 0; 
		}
		else
		{
			HeroPosition.y -= jumpspeed;
			jumpspeed += 1;
		}
	}

	ConstrainHero(25, 750, 25, 575, 1.f);
	tileOffset.x = (int) (mapOffset.x / m_cMap->GetTileSize());
	if (tileOffset.x + m_cMap->GetNumOfTiles_Width() > m_cMap->GetNumOfTiles_MapWidth())
	{
		tileOffset.x = m_cMap->GetNumOfTiles_MapWidth() - m_cMap->GetNumOfTiles_Width();
	}*/
}

void CCharacter::Jump()
{

}

void CCharacter::MoveUpDown(const bool mode, const float timeDiff)
{

}

void CCharacter::MoveLeftRight(const bool mode, const float timeDiff)
{

}

void CCharacter::Constrain(const int leftBorder, const int rightBorder, const int topBorder, const int bottomBorder, float timeDiff)
{

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