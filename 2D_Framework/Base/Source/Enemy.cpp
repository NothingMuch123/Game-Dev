#include "Enemy.h"

float CEnemy::WALK_SPEED = 250.f;
float CEnemy::GRAVITY = 1000.f;

CEnemy::CEnemy(Vector2 pos, Vector2 scale, bool midAir_Down, bool dir, bool active) : pos(pos), scale(scale), midAir_Down(midAir_Down), dir(dir), active(active), jumpSpeed(0.f), currentAnim(E_ANIM_WALK_LEFT)
{
	for (int i = 0; i < NUM_E_ANIM; ++i)
	{
		animationList[i] = NULL;
	}
}

CEnemy::~CEnemy(void)
{
	for (int i = 0; i < NUM_E_ANIM; ++i)
	{
		if (animationList[i])
		{
			delete animationList[i];
			animationList[i] = NULL;
		}
	}
}

void CEnemy::Init(Vector2 pos, Vector2 scale, bool midAir_Down, bool dir, bool active)
{
	this->pos = pos;
	this->scale = scale;
	this->midAir_Down = midAir_Down;
	this->dir = dir;
	this->active = active;
	this->currentAnim = E_ANIM_WALK_LEFT;
}

void CEnemy::Update(double dt, CMap* map)
{
	if (pos.x + (map->GetTileSize() * scale.x) < 0 || pos.x > map->GetMap_Width() || pos.y < 0 || pos.y > map->GetMap_Height())
	{
		Reset();
	}
	// Animation update
	if (animationList[currentAnim])
	{
		animationList[currentAnim]->Update(dt);
	}

	MoveLeftRight(true, dt, map);
	currentAnim = E_ANIM_WALK_LEFT;

	if (midAir_Down == false) // Check gravity
	{
		Vector2 tilePos( (int)((map->GetMapOffset().x + pos.x) / map->GetTileSize()) , map->GetNumOfTiles_Height() - (int) ceil( (pos.y - GRAVITY * dt) / map->GetTileSize()) );
		if (tilePos.x <= 0 || tilePos.x >= map->GetNumOfTiles_Width() || tilePos.y < 0 || tilePos.y >= map->GetNumOfTiles_Height())
		{
			return;
		}
		if (map->theScreenMap[tilePos.y][tilePos.x] == CMap::TILE_NONE && map->theScreenMap[tilePos.y][tilePos.x + scale.x] == CMap::TILE_NONE)
		{
			midAir_Down = true;
		}
	}
	else if (midAir_Down == true) // Falling downwards
	{ 
		// Character tile position
		Vector2 tilePos( (int)((pos.x + map->GetMapOffset().x) / map->GetTileSize()) , map->GetNumOfTiles_Height() - (int) ceil( (pos.y - jumpSpeed * dt) / map->GetTileSize()) );
		if (tilePos.x <= 0 || tilePos.x > map->GetNumOfTiles_Width() || tilePos.y < 0 || tilePos.y > map->GetNumOfTiles_Height())
		{
			return;
		}

		// Constrain player within the entire map
		if (tilePos.x < 0)
		{
			tilePos.x = 0;
		}
		if (tilePos.x > map->GetNumOfTiles_Width())
		{
			tilePos.x = map->GetNumOfTiles_Width();
		}
		if (tilePos.y < 0)
		{
			tilePos.y = 0;
		}
		if (tilePos.y > map->GetNumOfTiles_Height())
		{
			tilePos.y = map->GetNumOfTiles_Height();
		}
		if (map->theScreenMap[tilePos.y][tilePos.x] == CMap::TILE_DIRT || map->theScreenMap[tilePos.y][tilePos.x + scale.x] == CMap::TILE_DIRT || 
			map->theScreenMap[tilePos.y][tilePos.x] == CMap::TILE_GRASS || map->theScreenMap[tilePos.y][tilePos.x + scale.x] == CMap::TILE_GRASS || 
			map->theScreenMap[tilePos.y][tilePos.x] == CMap::TILE_STONE || map->theScreenMap[tilePos.y][tilePos.x + scale.x] == CMap::TILE_STONE || 
			map->theScreenMap[tilePos.y][tilePos.x] == CMap::TILE_SLAB || map->theScreenMap[tilePos.y][tilePos.x + scale.x] == CMap::TILE_SLAB || 
			map->theScreenMap[tilePos.y][tilePos.x] == CMap::TILE_FLOATING || map->theScreenMap[tilePos.y][tilePos.x + scale.x] == CMap::TILE_FLOATING) // Landed
		{
			// Since the new position does not allow the hero to move into, then go back to the old position
			pos.y = ((int) (pos.y / map->GetTileSize())) * map->GetTileSize();
			CalcBound(map);
			midAir_Down = false;
			jumpSpeed = 0;
		}
		else
		{
			pos.y -= jumpSpeed * dt;
			jumpSpeed += GRAVITY * dt;
			CalcBound(map);
		}
	}
}

void CEnemy::MoveLeftRight(const bool mode, const float timeDiff, CMap *map)
{
	if (mode)
	{
		Vector2 tilePos( (map->GetMapOffset().x + (pos.x - (WALK_SPEED * timeDiff))) / map->GetTileSize() , map->GetNumOfTiles_Height() - (int) (ceil( pos.y / map->GetTileSize()) + 1) ); // New position if move
		if (tilePos.x < 0 || tilePos.x > map->GetNumOfTiles_Width() || tilePos.y < 0 || tilePos.y > map->GetNumOfTiles_Height())
		{
			if (pos.x + (map->GetTileSize() * scale.x) > 0)
			{
				pos.x = pos.x - (WALK_SPEED * timeDiff);
				dir = false;
				CalcBound(map);
			}
			else
			{
				return;
			}
		}
		else
		{
			if (map->theScreenMap[tilePos.y][tilePos.x] == CMap::TILE_NONE ||
				map->theScreenMap[tilePos.y][tilePos.x] == CMap::TILE_TARGET_RED ||
				map->theScreenMap[tilePos.y][tilePos.x] == CMap::TILE_TARGET_LIGHT_BLUE ||
				map->theScreenMap[tilePos.y][tilePos.x] == CMap::TILE_TARGET_DARK_BLUE ||
				map->theScreenMap[tilePos.y][tilePos.x] == CMap::TILE_TARGET_GREEN ||
				map->theScreenMap[tilePos.y][tilePos.x] == CMap::TILE_TARGET_YELLOW ||
				map->theScreenMap[tilePos.y][tilePos.x] == CMap::TILE_TARGET_WHITE ||
				map->theScreenMap[tilePos.y][tilePos.x] == CMap::TILE_ENEMY_SPANWER) // Check collision when walking left
			{
				pos.x = pos.x - (WALK_SPEED * timeDiff);
				dir = false;
				CalcBound(map);
			}
		}
	}
	else 
	{
		Vector2 tilePos( (int)ceil((float)(map->GetMapOffset().x + ((pos.x + map->GetTileSize() * (scale.x - 1)) + (WALK_SPEED * timeDiff))) / map->GetTileSize()) , map->GetNumOfTiles_Height() - ((int) ceil( pos.y / map->GetTileSize()) + 1) ); // New position if move
		if (tilePos.x < 0 || tilePos.x > map->GetNumOfTiles_Width() || tilePos.y < 0 || tilePos.y > map->GetNumOfTiles_Height())
		{
			pos.x = pos.x + (WALK_SPEED * timeDiff);
			dir = true;
			CalcBound(map);
		}
		else
		{
			if (map->theScreenMap[tilePos.y][tilePos.x] == CMap::TILE_NONE ||
				map->theScreenMap[tilePos.y][tilePos.x] == CMap::TILE_TARGET_RED ||
				map->theScreenMap[tilePos.y][tilePos.x] == CMap::TILE_TARGET_LIGHT_BLUE ||
				map->theScreenMap[tilePos.y][tilePos.x] == CMap::TILE_TARGET_DARK_BLUE ||
				map->theScreenMap[tilePos.y][tilePos.x] == CMap::TILE_TARGET_GREEN ||
				map->theScreenMap[tilePos.y][tilePos.x] == CMap::TILE_TARGET_YELLOW ||
				map->theScreenMap[tilePos.y][tilePos.x] == CMap::TILE_TARGET_WHITE) // Check collision when walking right
			{
				pos.x = pos.x + (WALK_SPEED * timeDiff);
				dir = true;
				CalcBound(map);
			}
		}
	}
}

void CEnemy::SetPos(Vector2 pos)
{
	this->pos = pos;
}

Vector2 CEnemy::GetPos()
{
	return pos;
}

Vector2 CEnemy::GetScale()
{
	return scale;
}

Vector2 CEnemy::GetMinBound()
{
	return minBound;
}

Vector2 CEnemy::GetMaxBound()
{
	return maxBound;
}

void CEnemy::SetMidAir_Down(bool midAir_Down)
{
	this->midAir_Down = midAir_Down;
}

bool CEnemy::GetMidAir_Down()
{
	return midAir_Down;
}

void CEnemy::SetDir(bool dir)
{
	this->dir = dir;
}

bool CEnemy::GetDir()
{
	return dir;
}

void CEnemy::CalcBound(CMap *map)
{
	minBound.Set(pos.x, pos.y);
	maxBound.Set(pos.x + map->GetTileSize() * scale.x, pos.y + map->GetTileSize() * scale.y);
}

bool CEnemy::GetActive()
{
	return active;
}

void CEnemy::Reset()
{
	pos = minBound = maxBound = Vector2(0,0);
	active = midAir_Down = dir = false;
	jumpSpeed = 0.f;
}

void CEnemy::SetAnimation(ENEMY_ANIMATION type, SpriteAnimation* sa)
{
	animationList[type] = sa;
}

SpriteAnimation* CEnemy::GetCurrentAnimation()
{
	return animationList[currentAnim];
}