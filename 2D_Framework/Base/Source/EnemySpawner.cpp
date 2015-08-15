#include "EnemySpawner.h"


CEnemySpawner::CEnemySpawner(Vector2 pos, float spawnTimer) : pos(pos), spawnTimer(spawnTimer), timer(0.f)
{
}

CEnemySpawner::~CEnemySpawner(void)
{
}

bool CEnemySpawner::CheckSpawn(const double dt)
{
	if (timer < spawnTimer)
	{
		timer += dt;
		return false;
	}
	else
	{
		timer = 0.f;
		return true;
	}
}

Vector2 CEnemySpawner::GetPos()
{
	return pos;
}