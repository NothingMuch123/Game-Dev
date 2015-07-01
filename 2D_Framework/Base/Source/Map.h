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

	void Init(const int theScreen_Height, const int theScreen_Width, const int theNumOfTiles_Height, const int theNumOfTiles_Width, const int theTileSize=25);
	bool LoadMap(const string mapName);
	int GetNumOfTiles_Height(void);
	int GetNumOfTiles_Width(void);
	int GetTileSize(void);

	vector<vector<int> > theScreenMap;

private:
	int theScreen_Height;
	int theScreen_Width;
	int theNumOfTiles_Height;
	int theNumOfTiles_Width;
	int theTileSize;

	bool LoadFile(const string mapName);
};
