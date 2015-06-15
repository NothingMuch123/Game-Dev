#ifndef LOAD_HMAP_H
#define LOAD_HMAP_H

#include <vector>
#include "Vector3.h"
#include "Vector2.h"

bool LoadHeightMap(const char *file_path, std::vector<unsigned char> &heightMap);

float barryCentric(Vector3 p1, Vector3 p2, Vector3 p3, Vector2 pos);
float ReadHeightMap(std::vector<unsigned char> &heightMap, float x, float z);

static float TerrainHeights[256][256];

#endif