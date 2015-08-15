#ifndef ENEMY_SPAWNER_H
#define ENEMY_SPAWNER_H

#include "Vector2.h"

class CEnemySpawner
{
public:
	CEnemySpawner(Vector2 pos = Vector2(0,0), float spawnTimer = 1.f);
	~CEnemySpawner(void);

	bool CheckSpawn(const double dt);

	Vector2 GetPos();

private:
	Vector2 pos;
	float spawnTimer, timer;
};

#endif