#include "Elemental.h"



Elemental::Elemental() : m_element(NORMAL_TYPE)
{
}


Elemental::~Elemental()
{
}

void Elemental::SetElement(ELEMENT_TYPE element)
{
	m_element = element;
}

Elemental::ELEMENT_TYPE Elemental::GetElement(void)
{
	return m_element;
}