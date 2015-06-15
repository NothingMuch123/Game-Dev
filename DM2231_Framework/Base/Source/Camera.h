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
	float total_yaw, total_pitch;

	Camera();
	~Camera();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Reset();
	virtual void Update(double dt);
	virtual void UpdateStatus(unsigned char key) = 0;
};

#endif