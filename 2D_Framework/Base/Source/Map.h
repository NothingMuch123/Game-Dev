#pragma once

#include <vector>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

class CMap
{
public:
	CMap(void);
	~CMap(void);

	void Init(const int theScreen_Height, const int theScreen_Width, const int theNumOfTiles_Height, const int theNumOfTiles_Width, const int theMap_Height, const int theMap_Width, const int theTileSize=25);
	bool LoadMap(const string mapName);
	int GetNumOfTiles_Height(void);	// Get the number of tiles for height of screen
	int GetNumOfTiles_Width(void); // Get the number of tiles for width of screen
	int GetTileSize(void);

	int GetNumOfTiles_MapHeight();	// Get the number of tiles for height of map
	int GetNumOfTiles_MapWidth(); // Get the number of tiles for width of map

	int GetScreen_Width();
	int GetScreen_Height();
	int GetMap_Width();
	int GetMap_Height();

	vector<vector<int> > theScreenMap;

private:
	int theScreen_Height;
	int theScreen_Width;
	int theNumOfTiles_Height; // Number of tiles in the screen's height
	int theNumOfTiles_Width; // Number of tiles in the screen's width
	int theTileSize;

	int theMap_Height, theMap_Width; // Map's width and height
	int theNumOfTiles_MapHeight, theNumOfTiles_MapWidth; //Number of tiles in the map's width and height

	bool LoadFile(const string mapName);
};
