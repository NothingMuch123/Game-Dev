#ifndef ELEMENTAL_H
#define ELEMENTAL_H

class Elemental
{
public:
	enum ELEMENT_TYPE
	{
		NORMAL_TYPE,
		FIRE_TYPE,
		WATER_TYPE,
		AIR_TYPE,
		NUM_ELEMENTS
	};

protected:
	ELEMENT_TYPE m_element;

public:
	Elemental();
	virtual ~Elemental();

	void SetElement(ELEMENT_TYPE element);
	ELEMENT_TYPE GetElement(void);
};

#endif