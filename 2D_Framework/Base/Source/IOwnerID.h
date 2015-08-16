#ifndef IOWNER_ID_H
#define IOWNER_ID_H

class IOwnerID
{
	public:
		enum OWNER_TYPE
		{
			PLAYER_TYPE,
			ENEMY_TYPE,
			NUM_TYPES
		};

	protected:
		OWNER_TYPE m_owner;

	public:
		IOwnerID();
		virtual ~IOwnerID();

		void SetOwner(OWNER_TYPE owner);
		OWNER_TYPE GetOwner(void);
};

#endif