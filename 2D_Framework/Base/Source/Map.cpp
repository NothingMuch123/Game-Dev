#include "Map.h"

#include "PowerUp.h"

CMap::CMap(void)
: theScreen_Height(0)
, theScreen_Width(0)
, theNumOfTiles_Height(0)
, theNumOfTiles_Width(0)
, theMap_Height(0)
, theMap_Width(0)
, theNumOfTiles_MapHeight(0)
, theNumOfTiles_MapWidth(0)
, theTileSize(0)
, mapOffset(0,0)
, tileOffset(0,0)
, mapFineOffset(0,0)
{
	theScreenMap.clear();
}

CMap::~CMap(void)
{
	theScreenMap.clear();
}

void CMap::Init(const int theScreen_Height, const int theScreen_Width, const int theNumOfTiles_Height, const int theNumOfTiles_Width, const int theMap_Height, const int theMap_Width, int theTileSize)
{
	this->theScreen_Height		= theScreen_Height;
	this->theScreen_Width		= theScreen_Width;
	this->theNumOfTiles_Height	= theNumOfTiles_Height;
	this->theNumOfTiles_Width	= theNumOfTiles_Width;
	this->theMap_Height			= theMap_Height;
	this->theMap_Width			= theMap_Width;
	this->theTileSize			= theTileSize;

	theNumOfTiles_MapHeight = (int)(theMap_Height / theTileSize);
	theNumOfTiles_MapWidth = (int)(theMap_Width / theTileSize);

	theScreenMap.resize(theNumOfTiles_MapHeight);
	for (int i = 0; i < theNumOfTiles_MapHeight; ++i)
		theScreenMap[i].resize(theNumOfTiles_MapWidth);
}

bool CMap::LoadMap(const string mapName)
{
	if (LoadFile(mapName) == true)
	{
		printf("Map (%s) has been successfully loaded!\n", mapName.c_str());
		return true;
	}

	return false;
}

bool CMap::LoadFile(const string mapName)
{
	// Clear the spawn list for adding
	spawnList.clear();

	int theLineCounter = 0;
	int theMaxNumOfColumns = 0;

	ifstream file(mapName.c_str());
	if(file.is_open())
	{
		int i = 0, k = 0;
		while(file.good())
		{
			string aLineOfText = "";
			getline(file, aLineOfText);

			if  (theLineCounter>=theNumOfTiles_Height)
				break;

			// If this line is not a comment line, then process it
			if(!(aLineOfText.find("//*") == NULL) && aLineOfText != "")
			{
				int theColumnCounter = 0;

				string token;
				istringstream iss(aLineOfText);
				while(getline(iss, token, ',') && (theColumnCounter<theNumOfTiles_MapWidth))
				{
					int tile = atoi(token.c_str());
					theScreenMap[theLineCounter][theColumnCounter++] = tile;
					//static const float minTime = 1.f, maxTime = 3.f;
					switch (tile)
					{
					/*case TILE_TARGET_RED :
						{
							CTarget *t = new CTarget();
							t->Init(CTarget::TARGET_RED, Vector2((theColumnCounter - 1) * theTileSize, theScreen_Height - (theLineCounter + 1) * theTileSize), Vector2(1,1), Math::RandFloatMinMax(minTime, maxTime), false, true);
							targetList.push_back(t);
						}
						break;
					case TILE_TARGET_LIGHT_BLUE :
						{
							CTarget *t = new CTarget();
							t->Init(CTarget::TARGET_LIGHT_BLUE, Vector2((theColumnCounter - 1) * theTileSize, theScreen_Height - (theLineCounter + 1) * theTileSize), Vector2(1,1), Math::RandFloatMinMax(minTime, maxTime), false, true);
							targetList.push_back(t);
						}
						break;
					case TILE_TARGET_DARK_BLUE :
						{
							CTarget *t = new CTarget();
							t->Init(CTarget::TARGET_DARK_BLUE, Vector2((theColumnCounter - 1) * theTileSize, theScreen_Height - (theLineCounter + 1) * theTileSize), Vector2(1,1), Math::RandFloatMinMax(minTime, maxTime), false, true);
							targetList.push_back(t);
						}
						break;
					case TILE_TARGET_GREEN :
						{
							CTarget *t = new CTarget();
							t->Init(CTarget::TARGET_GREEN, Vector2((theColumnCounter - 1) * theTileSize, theScreen_Height - (theLineCounter + 1) * theTileSize), Vector2(1,1), Math::RandFloatMinMax(minTime, maxTime), false, true);
							targetList.push_back(t);
						}
						break;
					case TILE_TARGET_YELLOW :
						{
							CTarget *t = new CTarget();
							t->Init(CTarget::TARGET_YELLOW, Vector2((theColumnCounter - 1) * theTileSize, theScreen_Height - (theLineCounter + 1) * theTileSize), Vector2(1,1), Math::RandFloatMinMax(minTime, maxTime), false, true);
							targetList.push_back(t);
						}
						break;
					case TILE_TARGET_WHITE :
						{
							CTarget *t = new CTarget();
							t->Init(CTarget::TARGET_WHITE, Vector2((theColumnCounter - 1) * theTileSize, theScreen_Height - (theLineCounter + 1) * theTileSize), Vector2(1,1), Math::RandFloatMinMax(minTime, maxTime), false, true);
							targetList.push_back(t);
						}
						break;
					case TILE_ENEMY_SPANWER :
						{
							CEnemySpawner *spawner = new CEnemySpawner(Vector2((theColumnCounter - 1) * theTileSize, theScreen_Height - (theLineCounter + 1) * theTileSize), Math::RandFloatMinMax(1.f, 4.f));
							enemySpawnerList.push_back(spawner);
						}
						break;*/
					case TILE_FIRE_POWER:
						Vector2 pos(theColumnCounter * theTileSize, theScreen_Height - ((theLineCounter)* theTileSize));
						spawnList.push_back(new PowerUp(pos, true, Elemental::FIRE_TYPE, this));
						break;
					case TILE_WATER_POWER:
						Vector2 pos(theColumnCounter * theTileSize, theScreen_Height - ((theLineCounter)* theTileSize));
						spawnList.push_back(new PowerUp(pos, true, Elemental::WATER_TYPE, this));
						break;
					case TILE_AIR_POWER:
						Vector2 pos(theColumnCounter * theTileSize, theScreen_Height - ((theLineCounter)* theTileSize));
						spawnList.push_back(new PowerUp(pos, true, Elemental::AIR_TYPE, this));
						break;
					}
				}
				theLineCounter++;
			}
			else
			{
				// This is the first line of the map data file
				string token;
				istringstream iss(aLineOfText);
				while(getline(iss, token, ','))
				{
					// Count the number of columns
					theMaxNumOfColumns = atoi(token.c_str());
				}
				if ( theMaxNumOfColumns != theNumOfTiles_MapWidth)
					return false;
			}
		}
	}
	return true;
}

int CMap::GetNumOfTiles_Height(void)
{
	return theNumOfTiles_Height;
}

int CMap::GetNumOfTiles_Width(void)
{
	return theNumOfTiles_Width;
}

int CMap::GetTileSize(void)
{
	return theTileSize;
}

int CMap::GetNumOfTiles_MapHeight()
{
	return theNumOfTiles_MapHeight;
}

int CMap::GetNumOfTiles_MapWidth()
{
	return theNumOfTiles_MapWidth;
}

int CMap::GetScreen_Width()
{
	return theScreen_Width;
}

int CMap::GetScreen_Height()
{
	return theScreen_Height;
}

int CMap::GetMap_Width()
{
	return theMap_Width;
}

int CMap::GetMap_Height()
{
	return theMap_Height;
}

void CMap::SetMapOffset(Vector2 mapOffset)
{
	this->mapOffset = mapOffset;
}

Vector2 CMap::GetMapOffset()
{
	return mapOffset;
}

void CMap::SetTileOffset(Vector2 tileOffset)
{
	this->tileOffset = tileOffset;
}

Vector2 CMap::GetTileOffset()
{
	return tileOffset;
}

void CMap::SetMapFineOffset(Vector2 mapFineOffset)
{
	this->mapFineOffset = mapFineOffset;
}

Vector2 CMap::GetMapFineOffset()
{
	return mapFineOffset;
}

vector<Collidable*> CMap::GetObjsToSpawn(void)
{
	return spawnList;
}
