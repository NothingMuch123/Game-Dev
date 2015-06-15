#ifndef ENTITY_H
#define ENTITY_H

#include "Vector2.h"

class CEntity
{
public:
	CEntity(int ID = -1, Vector2 position = Vector2(0,0), bool active = false);
	virtual ~CEntity(void);

	void Init(int ID, Vector2 position, bool active = true);

	void SetActive(bool active);
	bool GetActive();

	void SetPosition(Vector2 position);
	Vector2 GetPosition();

	void SetID(int ID);
	int GetID();

private:
	bool active;
	Vector2 position;
	int ID;
};

#endif