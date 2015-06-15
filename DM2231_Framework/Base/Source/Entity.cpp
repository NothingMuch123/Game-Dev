#include "Entity.h"

CEntity::CEntity(int ID, Vector2 position, bool active) : ID(ID), position(position), active(active)
{
}

CEntity::~CEntity(void)
{
}

void CEntity::Init(int ID, Vector2 position, bool active)
{
	SetActive(active);
	SetPosition(position);
	SetID(ID);
}

void CEntity::SetActive(bool active)
{
	this->active = active;
}

bool CEntity::GetActive()
{
	return active;
}

void CEntity::SetPosition(Vector2 position)
{
	this->position = position;
}

Vector2 CEntity::GetPosition()
{
	return position;
}

void CEntity::SetID(int ID)
{
	this->ID = ID;
}

int CEntity::GetID()
{
	return ID;
}