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
CObj::CObj(int _ID, Vector3 _translate, Vector3 _rotate, Vector3 _scale, Vector3 _size, bool render) : ID(_ID), translate(_translate), rotate(_rotate), scale(_scale), size(_size), render(render)
{
	calcBound();
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

void CObj::SetTranslate(Vector3 translate)
{
	this->translate = translate;
}

/******************************************************************************/
/*!
\brief
return translate values
\return
translate values
*/
/******************************************************************************/
Vector3 CObj::GetTranslate()
{
	return translate;
}

void CObj::SetRotate(Vector3 rotate)
{
	this->rotate = rotate;
}

/******************************************************************************/
/*!
\brief
return rotate values
\return
rotate values
*/
/******************************************************************************/
Vector3 CObj::GetRotate()
{
	return rotate;
}

void CObj::SetScale(Vector3 scale)
{
	this->scale = scale;
}

/******************************************************************************/
/*!
\brief
return scale values
\return
scale values
*/
/******************************************************************************/
Vector3 CObj::GetScale()
{
	return scale;
}

void CObj::SetSize(Vector3 size)
{
	this->size = size;
}

/******************************************************************************/
/*!
\brief
return maxBound values
\return
maxBound values
*/
/******************************************************************************/
Vector3 CObj::GetMaxBound()
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
Vector3 CObj::GetMinBound()
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
Vector3 CObj::GetSize()
{
	return size;
}

void CObj::SetID(int ID)
{
	this->ID = ID;
}

/******************************************************************************/
/*!
\brief
return ID
\return
ID similar to SP2::GEOMETRY_TYPE
*/
/******************************************************************************/
int CObj::GetID()
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
	float offset = 0;
	maxBound.Set(translate.x + ((size.x * scale.x) / 2) + offset, translate.y + ((size.y * scale.y) / 2) + offset, translate.z + ((size.z * scale.z) / 2) + offset);
	minBound.Set(translate.x - ((size.x * scale.x) / 2) - offset, translate.y - ((size.y * scale.y) / 2) - offset , translate.z - ((size.z * scale.z) / 2) - offset);
}

/******************************************************************************/
/*!
\brief
return render state
\return
render state
*/
/******************************************************************************/
bool CObj::GetRender()
{
	return render;
}

/******************************************************************************/
/*!
\brief
set render state
*/
/******************************************************************************/
void CObj::SetRender(bool render)
{
	this->render = render;
}

void CObj::Init(int ID, Vector3 translate, Vector3 rotate, Vector3 scale, Vector3 size, bool render)
{
	SetID(ID);
	SetTranslate(translate);
	SetRotate(rotate);
	SetScale(scale);
	SetSize(size);
	SetRender(render);
	calcBound();
}