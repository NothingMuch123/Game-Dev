#ifndef CAMERA_3_H
#define CAMERA_3_H

#include "Camera.h"
#include "LoadHmap.h"

class Camera3 : public Camera
{
public:
	//Vector3 position;
	//Vector3 target;
	//Vector3 up;

	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;

	Camera3();
	~Camera3();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt, std::vector<unsigned char> &heightMap, const Vector3 &terrainSize);
	virtual void Reset();
	virtual void UpdateStatus(unsigned char key);

	virtual void MoveForward_Backward(double dt, bool dir, std::vector<unsigned char> &heightMap, const Vector3 &terrainSize);	// 0 - Forward	| 1 - Backwards
	virtual void MoveLeft_Right(double dt, bool dir, std::vector<unsigned char> &heightMap, const Vector3 &terrainSize);		// 0 - Left		| 1 - Right
	virtual void MoveUp_Down(double dt, bool dir, std::vector<unsigned char> &heightMap, const Vector3 &terrainSize);			// 0 - Up		| 1 - Down

	// Mouse
	virtual void TurnUp(double dt);
	virtual void TurnDown(double dt);
	virtual void TurnLeft(double dt);
	virtual void TurnRight(double dt);

private:
	bool myKeys[255];
};

#endif