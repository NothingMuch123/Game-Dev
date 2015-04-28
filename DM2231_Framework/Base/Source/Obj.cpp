#include "Obj.h"

/******************************************************************************/
/*!
\brief
Default constructor
*/
/******************************************************************************/
CObj::CObj(void) : ID(-1), translate(0, 0, 0), rotate(0, 0, 0), scale(1, 1, 1), maxBound(0, 0, 0), minBound(0, 0, 0), size(1, 1, 1), render(true)
{
}

/******************************************************************************/
/*!
\brief
Overloaded constructor
*/
/******************************************************************************/
CObj::CObj(int _ID, Vector3 _translate, Vector3 _rotate, Vector3 _scale, Vector3 _size) : ID(_ID), translate(_translate), rotate(_rotate), scale(_scale), size(_size), render(true)
{
}

/******************************************************************************/
/*!
\brief
Destructor
*/
/******************************************************************************/
CObj::~CObj(void)
{
}

/******************************************************************************/
/*!
\brief
return translate values
\return
translate values
*/
/******************************************************************************/
Vector3 CObj::getTranslate()
{
	return translate;
}

/******************************************************************************/
/*!
\brief
return rotate values
\return
rotate values
*/
/******************************************************************************/
Vector3 CObj::getRotate()
{
	return rotate;
}

/******************************************************************************/
/*!
\brief
return scale values
\return
scale values
*/
/******************************************************************************/
Vector3 CObj::getScale()
{
	return scale;
}

/******************************************************************************/
/*!
\brief
return maxBound values
\return
maxBound values
*/
/******************************************************************************/
Vector3 CObj::getMaxBound()
{
	return maxBound;
}

/******************************************************************************/
/*!
\brief
return minBound values
\return
minBound values
*/
/******************************************************************************/
Vector3 CObj::getMinBound()
{
	return minBound;
}

/******************************************************************************/
/*!
\brief
return size values
\return
size values
*/
/******************************************************************************/
Vector3 CObj::getSize()
{
	return size;
}

/******************************************************************************/
/*!
\brief
return ID
\return
ID similar to SP2::GEOMETRY_TYPE
*/
/******************************************************************************/
int CObj::getID()
{
	return ID;
}

/******************************************************************************/
/*!
\brief
Calculates bound based on translate, size and scale
*/
/******************************************************************************/
void CObj::calcBound()
{
	float offset = 3;
	maxBound.Set(translate.x + ((size.x * scale.x) / 2) + offset, translate.y + (size.y * scale.y) + offset, translate.z + ((size.z * scale.z) / 2) + offset);
	minBound.Set(translate.x - ((size.x * scale.x) / 2) - offset, translate.y - (size.y * scale.y) - offset , translate.z - ((size.z * scale.z) / 2) - offset);
}

/******************************************************************************/
/*!
\brief
return render state
\return
render state
*/
/******************************************************************************/
bool CObj::getRender()
{
	return render;
}

/******************************************************************************/
/*!
\brief
set render state
*/
/******************************************************************************/
void CObj::setRender(bool _render)
{
	render = _render;
}