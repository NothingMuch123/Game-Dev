#include "IOwnerID.h"

IOwnerID::IOwnerID()
	: m_owner(PLAYER_TYPE)
{
}


IOwnerID::~IOwnerID()
{
}

void IOwnerID::SetOwner(OWNER_TYPE owner)
{
	m_owner = owner;
}

IOwnerID::OWNER_TYPE IOwnerID::GetOwner(void)
{
	return m_owner;
}
