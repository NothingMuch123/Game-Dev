#include "Character.h"

float CCharacter::WALK_SPEED = 250.f;
float CCharacter::RUN_SPEED = 400.f;
float CCharacter::GRAVITY = 1000.f;

CCharacter::CCharacter(Vector2 pos, Vector2 scale, CMap* map, bool defaultLookingLeft, float jumpSpeed, bool midAir_Up, bool midAir_Down, bool dir) : Collidable(pos, true, map->GetTileSize()), Elemental()
					,
					scale(scale),
					jumpSpeed(jumpSpeed),
					midAir_Up(midAir_Up),
					midAir_Down(midAir_Down),
					dir(dir),
					currentAnim(ANIM_IDLE),
					current_speed(WALK_SPEED),
					fallingThrough(false),
					fallingThroughDist(0.f),
					defaultLook(defaultLookingLeft),
					flipSprite(false)
{
	for (int i = 0; i < NUM_CA; ++i)
	{
		actions[i] = false;
	}
	m_owner = PLAYER_TYPE;
}

CCharacter::~CCharacter(void)
{
	for (int i = 0; i < NUM_ELEMENTS; ++i)
	{
		for (int j = 0; j < NUM_ANIM; ++j)
		{
			if (animationList[i][j])
			{
				delete animationList[i][j];
				animationList[i][j] = NULL;
			}
		}
	}
}

void CCharacter::Update(const double dt, CMap *m_cMap)
{
	current_speed = WALK_SPEED;
	if (actions[CA_WALK_LEFT])
	{
		dir = false;
		MoveLeftRight(true, dt, m_cMap);
		spriteSetToLeft();
		if (!midAir_Up && !midAir_Down)
		{
			CheckReset(ANIM_WALK);
		}
		actions[CA_WALK_LEFT] = false;
	}
	else if (actions[CA_WALK_RIGHT])
	{
		dir = true;
		MoveLeftRight(false, dt, m_cMap);
		spriteSetToRight();
		if (!midAir_Up && !midAir_Down)
		{
			CheckReset(ANIM_WALK);
		}
		actions[CA_WALK_RIGHT] = false;
	}
	else
	{
		if (!midAir_Up && !midAir_Down && !actions[CA_SKILL])
		{
			CheckReset(ANIM_IDLE);
		}
	}
	if (actions[CA_JUMP])
	{
		Jump();
		CheckReset(ANIM_JUMP);
		actions[CA_JUMP] = false;
	}
	if (actions[CA_ATTACK])
	{
		//Attack();
		CheckReset(ANIM_ATTACK);
		actions[CA_ATTACK] = false;
	}
	if (actions[CA_SKILL] && m_element != NORMAL_TYPE)
	{
		//Skill();
		CheckReset(ANIM_SKILL);
		actions[CA_SKILL] = false;
	}
	// Actions
	/*if (midAir_Up || midAir_Down)
	{
		actions[CA_LIE] = false;
	}
	if(actions[CA_LOOK_UP]) // Looking up
	{
		if (actions[CA_LEFT] && !actions[CA_LIE]) // Moving left while looking up
		{
			if (!midAir_Up)
			{
				CheckReset(ANIM_MOVE_LEFT_SHOOT_TOP);
			}
			MoveLeftRight(true, dt, m_cMap);
			actions[CA_LEFT] = false;
		}
		else if (actions[CA_RIGHT] && !actions[CA_LIE]) // Moving right while looking up
		{
			if (!midAir_Up)
			{
				CheckReset(ANIM_MOVE_RIGHT_SHOOT_TOP);
			}
			MoveLeftRight(false, dt, m_cMap);
			actions[CA_RIGHT] = false;
		}
		else // Not moving while looking up
		{
			if (dir && !midAir_Up) // Facing right
			{
				CheckReset(ANIM_LOOK_UP_RIGHT);
			}
			else if (!midAir_Up)
			{
				CheckReset(ANIM_LOOK_UP_LEFT);
			}
		}
		actions[CA_LOOK_UP] = false;
	}
	else if (actions[CA_LOOK_DOWN]) // Looking down
	{
		if (actions[CA_LEFT] && !actions[CA_LIE])
		{
			if (!midAir_Up)
			{
				CheckReset(ANIM_MOVE_LEFT_SHOOT_DOWN);
			}
			MoveLeftRight(true, dt, m_cMap);
			actions[CA_LEFT] = false;
		}
		else if (actions[CA_RIGHT] && !actions[CA_LIE]) // Move right while looking down
		{
			if (!midAir_Up)
			{
				CheckReset(ANIM_MOVE_RIGHT_SHOOT_DOWN);
			}
			MoveLeftRight(false, dt, m_cMap);
			actions[CA_RIGHT] = false;
		}
		else // Not moving while looking down
		{
			if (dir && !midAir_Up) // Facing right
			{
				CheckReset(ANIM_LOOK_DOWN_RIGHT);
			}
			else if (!midAir_Up)
			{
				CheckReset(ANIM_LOOK_DOWN_LEFT);
			}
			if (actions[CA_JUMP] && !midAir_Up && !midAir_Down) // Drop down if jump
			{
				Vector2 tilePos( (int)((m_cMap->GetMapOffset().x + pos.x) / m_cMap->GetTileSize()) , m_cMap->GetNumOfTiles_Height() - (int) ceil( (pos.y - (GRAVITY * dt)) / m_cMap->GetTileSize()) );
				if (m_cMap->theScreenMap[tilePos.y][tilePos.x] == CMap::TILE_FLOATING || m_cMap->theScreenMap[tilePos.y][tilePos.x + 1] == CMap::TILE_FLOATING)
				{
					fallingThrough = true;
					midAir_Down = true;
					actions[CA_JUMP] = false;
				}
			}
		}
		actions[CA_LOOK_DOWN] = false;
	}
	else // Not looking up or down
	{
		if (actions[CA_LEFT] && !actions[CA_LIE]) // Moving left
		{
			if (actions[CA_RUN]) // Running left
			{
				current_speed = RUN_SPEED;
				if (!midAir_Up)
				{
					CheckReset(ANIM_RUN_LEFT);
				}
			}
			else // Walking left
			{
				current_speed = WALK_SPEED;
				if (!midAir_Up)
				{
					CheckReset(ANIM_MOVE_LEFT_SHOOT);
				}
			}
			MoveLeftRight(true, dt, m_cMap);
			actions[CA_LEFT] = false;
		}
		else if (actions[CA_RIGHT] && !actions[CA_LIE]) // Moving right
		{
			if (actions[CA_RUN]) // Running right
			{
				current_speed = RUN_SPEED;
				if (!midAir_Up)
				{
					CheckReset(ANIM_RUN_RIGHT);
				}
			}
			else // Walking right
			{
				current_speed = WALK_SPEED;
				if (!midAir_Up)
				{
					CheckReset(ANIM_MOVE_LEFT_SHOOT);
				}
			}
			MoveLeftRight(false, dt, m_cMap);
			actions[CA_RIGHT] = false;
		}
		else if (!midAir_Up && !actions[CA_LIE]) // Not moving
		{
			if (dir) // Idle right
			{
				CheckReset(ANIM_IDLE_RIGHT);
			}
			else
			{
				CheckReset(ANIM_IDLE_LEFT);
			}
		}
	}

	if (actions[CA_JUMP])
	{
		Jump();
		actions[CA_JUMP] = false;
	}

	if (midAir_Down)
	{
		if (dir)
		{
			CheckReset(ANIM_FALL_RIGHT);
		}
		else
		{
			CheckReset(ANIM_FALL_LEFT);
		}
	}
	else
	{
		if (actions[CA_LIE] && !midAir_Up && !actions[CA_JUMP])
		{
			if (dir)
			{
				CheckReset(ANIM_LIE_RIGHT);
			}
			else
			{
				CheckReset(ANIM_LIE_LEFT);
			}
			actions[CA_LIE] = false;
		}
	}*/

	for (int i = 0; i < NUM_CA; ++i)
	{
		actions[i] = false;
	}

	// Update animation
	spriteList[m_element]->Update(dt);

	if (fallingThroughDist >= m_cMap->GetTileSize()  * scale.x)
	{
		fallingThrough = false;
		fallingThroughDist = 0.f;
	}
	
	// Update Hero's info
	if (midAir_Up == false && midAir_Down == false) // Check gravity
	{
		Vector2 tilePos( (int)((m_cMap->GetMapOffset().x + pos.x) / m_cMap->GetTileSize()) , m_cMap->GetNumOfTiles_Height() - (int) ceil( pos.y / m_cMap->GetTileSize()) );
		if (m_cMap->theScreenMap[tilePos.y][tilePos.x] == CMap::TILE_NONE && m_cMap->theScreenMap[tilePos.y][tilePos.x + scale.x] == CMap::TILE_NONE || 
			(fallingThrough && (m_cMap->theScreenMap[tilePos.y][tilePos.x] == CMap::TILE_FLOATING && m_cMap->theScreenMap[tilePos.y][tilePos.x + scale.x] == CMap::TILE_FLOATING)))
		{
			midAir_Down = true;
		}
	}
	else if (midAir_Up == true && midAir_Down == false) // Jumping upwards
	{
		// Character tile position from map
		Vector2 tilePos( (int)((m_cMap->GetMapOffset().x + pos.x) / m_cMap->GetTileSize()) , m_cMap->GetNumOfTiles_Height() - (int) (ceil( (pos.y + jumpSpeed * dt) / m_cMap->GetTileSize()) + 1) );
		if (tilePos.y < 0) // Ensure that tilePos does not go out of map
		{
			tilePos.y = 0;
		}
		if (m_cMap->theScreenMap[tilePos.y][tilePos.x] == CMap::TILE_DIRT || m_cMap->theScreenMap[tilePos.y][tilePos.x + scale.x] == CMap::TILE_DIRT || 
			m_cMap->theScreenMap[tilePos.y][tilePos.x] == CMap::TILE_GRASS || m_cMap->theScreenMap[tilePos.y][tilePos.x + scale.x] == CMap::TILE_GRASS || 
			m_cMap->theScreenMap[tilePos.y][tilePos.x] == CMap::TILE_STONE || m_cMap->theScreenMap[tilePos.y][tilePos.x + scale.x] == CMap::TILE_STONE || 
			m_cMap->theScreenMap[tilePos.y][tilePos.x] == CMap::TILE_SLAB || m_cMap->theScreenMap[tilePos.y][tilePos.x + scale.x] == CMap::TILE_SLAB || 
			pos.y >= m_cMap->GetScreen_Height() - m_cMap->GetTileSize()) // Hit object while jumping
		{ 
			// Since the new position does not allow the hero to move into, then go back to the old position
			//pos.y = ((int) (pos.y / m_cMap->GetTileSize())) * m_cMap->GetTileSize();
			midAir_Up = false;
			midAir_Down = true;
			jumpSpeed = 0; 
		} 
		else // Continue jumping
		{ 
			pos.y += jumpSpeed * dt;
			jumpSpeed -= GRAVITY * dt;
			CalcBound();
			if (jumpSpeed <= 0)
			{
				midAir_Up = false; 
				midAir_Down = true; 
			}
		} 
	} 
	else if (midAir_Up == false && midAir_Down == true) // Falling downwards
	{ 
		// Character tile position
		Vector2 tilePos( (int)((m_cMap->GetMapOffset().x + pos.x) / m_cMap->GetTileSize()) , m_cMap->GetNumOfTiles_Height() - (int) ceil( (pos.y - jumpSpeed * dt) / m_cMap->GetTileSize()) );

		// Constrain player within the entire map
		if (tilePos.x < 0)
		{
			tilePos.x = 0;
		}
		if (tilePos.x > m_cMap->GetNumOfTiles_MapWidth())
		{
			tilePos.x = m_cMap->GetNumOfTiles_MapWidth();
		}
		if (tilePos.y < 0)
		{
			tilePos.y = 0;
		}
		if (tilePos.y > m_cMap->GetNumOfTiles_MapHeight())
		{
			tilePos.y = m_cMap->GetNumOfTiles_MapHeight();
		}
		if (m_cMap->theScreenMap[tilePos.y][tilePos.x] == CMap::TILE_DIRT || m_cMap->theScreenMap[tilePos.y][tilePos.x + scale.x] == CMap::TILE_DIRT || 
			m_cMap->theScreenMap[tilePos.y][tilePos.x] == CMap::TILE_GRASS || m_cMap->theScreenMap[tilePos.y][tilePos.x + scale.x] == CMap::TILE_GRASS || 
			m_cMap->theScreenMap[tilePos.y][tilePos.x] == CMap::TILE_STONE || m_cMap->theScreenMap[tilePos.y][tilePos.x + scale.x] == CMap::TILE_STONE || 
			m_cMap->theScreenMap[tilePos.y][tilePos.x] == CMap::TILE_SLAB || m_cMap->theScreenMap[tilePos.y][tilePos.x + scale.x] == CMap::TILE_SLAB || 
			(!fallingThrough && (m_cMap->theScreenMap[tilePos.y][tilePos.x] == CMap::TILE_FLOATING || m_cMap->theScreenMap[tilePos.y][tilePos.x + scale.x] == CMap::TILE_FLOATING))) // Landed
		{
			// Since the new position does not allow the hero to move into, then go back to the old position
			if (!fallingThrough)
			{
				pos.y = ((int) (pos.y / m_cMap->GetTileSize())) * m_cMap->GetTileSize();
			}
			CalcBound();
			if (fallingThrough)
			{
				if (fallingThroughDist >= m_cMap->GetTileSize())
				{
					midAir_Down = false;
				}
			}
			else
			{
				midAir_Down = false;
			}
			jumpSpeed = 0;
		}
		else
		{
			pos.y -= jumpSpeed * dt;
			if (fallingThrough)
			{
				fallingThroughDist += jumpSpeed * dt;
			}
			jumpSpeed += GRAVITY * dt;
			CalcBound();
		}
	}

	Constrain(dt, m_cMap);
	//m_cMap->GetTileOffset().x = (int) (m_cMap->GetMapOffset().x / m_cMap->GetTileSize());
	m_cMap->SetTileOffset( Vector2((int) (m_cMap->GetMapOffset().x / m_cMap->GetTileSize()) , m_cMap->GetTileOffset().y) );
	if (m_cMap->GetTileOffset().x + m_cMap->GetNumOfTiles_Width() > m_cMap->GetNumOfTiles_MapWidth()) // Remove the extra column of tile when player reached the end
	{
		//m_cMap->GetTileOffset().x = m_cMap->GetNumOfTiles_MapWidth() - m_cMap->GetNumOfTiles_Width();
		m_cMap->SetTileOffset( Vector2(m_cMap->GetNumOfTiles_MapWidth() - m_cMap->GetNumOfTiles_Width() , m_cMap->GetTileOffset().y) );
	}
}

void CCharacter::Jump()
{
	if (midAir_Up == false && midAir_Down == false && !fallingThrough) 
	{
		midAir_Up = true;
		jumpSpeed = 500.f;
	}
}

void CCharacter::MoveUpDown(const bool mode, const float timeDiff)
{
	if (mode)
	{ 
		pos.y = pos.y + (current_speed * timeDiff);
	}
	else 
	{ 
		pos.y = pos.y - (current_speed * timeDiff); 
	} 
}

void CCharacter::MoveLeftRight(const bool mode, const float timeDiff, CMap *map)
{
	if (mode)		// Left
	{
		Vector2 tilePos( (map->GetMapOffset().x + (pos.x - (current_speed * timeDiff))) / map->GetTileSize() , map->GetNumOfTiles_Height() - (int) (ceil( pos.y / map->GetTileSize()) + 1) ); // New position if move
		if (map->theScreenMap[tilePos.y][tilePos.x] == CMap::TILE_NONE ||
			(midAir_Up && map->theScreenMap[tilePos.y][tilePos.x] == CMap::TILE_FLOATING) ||
			(fallingThrough && map->theScreenMap[tilePos.y][tilePos.x] == CMap::TILE_FLOATING)) // Check collision when walking left
		{
			pos.x = pos.x - (current_speed * timeDiff);
			dir = false;
			spriteSetToLeft();
		}
	}
	else 
	{
		Vector2 tilePos( (int)ceil((float)(map->GetMapOffset().x + ((pos.x + map->GetTileSize() * (scale.x - 1)) + (current_speed * timeDiff))) / map->GetTileSize()) , map->GetNumOfTiles_Height() - ((int) ceil( pos.y / map->GetTileSize()) + 1) ); // New position if move
		if (map->theScreenMap[tilePos.y][tilePos.x] == CMap::TILE_NONE || 
			(midAir_Up && map->theScreenMap[tilePos.y][tilePos.x] == CMap::TILE_FLOATING) ||
			(fallingThrough && map->theScreenMap[tilePos.y][tilePos.x] == CMap::TILE_FLOATING)) // Check collision when walking right
		{
			pos.x = pos.x + (current_speed * timeDiff);
			dir = true;
			spriteSetToRight();
		}
	}
}

void CCharacter::Constrain(float timeDiff, CMap *m_cMap)
{
	// Tile point at bottom left
	const int leftBorder = m_cMap->GetTileSize(), topBorder = m_cMap->GetTileSize();
	const int rightBorder = m_cMap->GetScreen_Width() - (m_cMap->GetTileSize() * (scale.x + 1)); // Minus 2 tiles because width is at the most right while tile starts at its left
	const int bottomBorder = m_cMap->GetScreen_Height() - m_cMap->GetTileSize();
	if (pos.x < leftBorder)
	{
		pos.x = leftBorder;
		//m_cMap->GetMapOffset().x = m_cMap->GetMapOffset().x - (int)(5.f * timeDiff);
		m_cMap->SetMapOffset( Vector2(m_cMap->GetMapOffset().x - (current_speed * timeDiff) , m_cMap->GetMapOffset().y) );
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
		m_cMap->SetMapOffset( Vector2(m_cMap->GetMapOffset().x + (current_speed * timeDiff), m_cMap->GetMapOffset().y) );
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
	
	CalcBound();
}

Vector2 CCharacter::GetScale()
{
	return scale;
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

void CCharacter::SetDir(bool dir)
{
	this->dir = dir;
}

bool CCharacter::GetDir()
{
	return dir;
}

void CCharacter::SetActions(const CHARACTER_ACTION action, const bool status)
{
	actions[action] = status;
}

void CCharacter::SetAnimation(ELEMENT_TYPE eType, CHARACTER_ANIMATION caType, Animation* a)
{
	animationList[eType][caType] = a;
}

SpriteAnimation* CCharacter::GetSprite()
{
	return spriteList[m_element];
}

void CCharacter::CheckReset(CHARACTER_ANIMATION type)
{
	if (currentAnim != type || ((currentAnim == ANIM_ATTACK || currentAnim == ANIM_SKILL) && spriteList[m_element]->m_anim->ended))
	{
		currentAnim = type;
		spriteList[m_element]->m_anim = animationList[m_element][currentAnim];
		spriteList[m_element]->Reset();
	}
}

void CCharacter::CalcBound()
{
	minBound.Set(pos.x, pos.y);
	maxBound.Set(pos.x + tileSize * scale.x, pos.y + tileSize * scale.y);
}

bool CCharacter::GetAction(CHARACTER_ACTION action)
{
	return actions[action];
}

CCharacter::CHARACTER_ANIMATION CCharacter::GetCurrentAnim()
{
	return currentAnim;
}

void CCharacter::SetSprite(ELEMENT_TYPE type, SpriteAnimation *sprite)
{
	this->spriteList[type] = sprite;
}

bool CCharacter::GetFlip(void) const
{
	return flipSprite;
}

void CCharacter::spriteSetToLeft(void)
{
	flipSprite = !defaultLook;
}

void CCharacter::spriteSetToRight(void)
{
	flipSprite = defaultLook;
}

Skill CCharacter::GetBasicSkill()
{
	return m_basicSkill;
}

Skill CCharacter::GetAdvancedSkill()
{
	return m_advancedSkill;
}
