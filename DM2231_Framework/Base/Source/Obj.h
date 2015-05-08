/******************************************************************************/
/*!
\file	Obj.h
\author Koh Fang Shu
\par	
\brief
Header file for Obj Class
*/
/******************************************************************************/
#ifndef OBJ_H
#define OBJ_H

#include "Vector3.h"
#include "Mesh.h"

/******************************************************************************/
/*!
		Class CObj:
\brief	Stores Item Data 
*/
/******************************************************************************/

class CObj
{
public:
	CObj(void);
	CObj(int _ID, Vector3 _translate, Vector3 _rotate, Vector3 _scale, Vector3 _size);
	void Init(int ID, Vector3 translate, Vector3 rotate, Vector3 scale, Vector3 size, bool render = true);

	void SetID(int ID);
	int GetID();
	
	void SetTranslate(Vector3 translate);
	Vector3 GetTranslate();

	void SetRotate(Vector3 rotate);
	Vector3 GetRotate();

	void SetScale(Vector3 scale);
	Vector3 GetScale();

	void SetSize(Vector3 size);
	Vector3 GetSize();

	void SetRender(bool render);
	bool GetRender();

	Vector3 GetMaxBound();
	Vector3 GetMinBound();

	virtual void calcBound();	// AABB bound calculation
	virtual ~CObj(void);

protected:
	int ID;
	Mesh *mesh;
	Vector3 translate, rotate, scale;
	Vector3 maxBound, minBound;
	Vector3 size;
	bool render;
};

#endif