#ifndef CAMERA_H
#define CAMERA_H

#include "Vector3.h"
#include <vector>

class Camera
{
public:
	Vector3 position;
	Vector3 target;
	Vector3 up;

	Camera();
	~Camera();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Reset();
	virtual void Update(double dt);
	virtual void UpdateStatus(unsigned char key) = 0;

	virtual void MoveForward_Backward(double dt, bool dir, std::vector<unsigned char> &heightMap, const Vector3 &terrainSize) = 0;	// 0 - Forward	| 1 - Backwards
	virtual void MoveLeft_Right(double dt, bool dir, std::vector<unsigned char> &heightMap, const Vector3 &terrainSize) = 0;		// 0 - Left		| 1 - Right
	virtual void MoveUp_Down(double dt, bool dir, std::vector<unsigned char> &heightMap, const Vector3 &terrainSize) = 0;			// 0 - Up		| 1 - Down

	// Mouse
	virtual void TurnUp(double dt) = 0;
	virtual void TurnDown(double dt) = 0;
	virtual void TurnLeft(double dt) = 0;
	virtual void TurnRight(double dt) = 0;
};

#endif