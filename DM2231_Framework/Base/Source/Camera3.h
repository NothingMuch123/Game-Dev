#ifndef CAMERA_3_H
#define CAMERA_3_H

#include "Camera.h"
#include "LoadHmap.h"

class Camera3 : public Camera
{
public:
	static float TERRAIN_OFFSET;
	static float CAMERA_SPEED;
	static float CROUCH_SPEED;
	static float CROUCH_OFFSET;
	enum CAM_TYPE
	{
		LAND_CAM,
		AIR_CAM,
		NUM_CAM_TYPE,
	};

	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;

	Camera3();
	~Camera3();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt, std::vector<unsigned char> &heightMap, const Vector3 &terrainSize);
	virtual void Reset();
	virtual void UpdateStatus(unsigned char key);

	virtual void SetCameraType(CAM_TYPE type);
	virtual CAM_TYPE GetCameraType();

	virtual void MoveForward_Backward(double dt, bool dir, std::vector<unsigned char> &heightMap, const Vector3 &terrainSize);	// 0 - Forward	| 1 - Backwards
	virtual void MoveLeft_Right(double dt, bool dir, std::vector<unsigned char> &heightMap, const Vector3 &terrainSize);		// 0 - Left		| 1 - Right
	virtual void MoveUp_Down(double dt, bool dir, std::vector<unsigned char> &heightMap, const Vector3 &terrainSize);			// 0 - Up		| 1 - Down

	// Basic methods
	virtual void LookUp(const double dt);
	virtual void LookDown(const double dt);
	virtual void TurnLeft(const double dt);
	virtual void TurnRight(const double dt);
	virtual void SpinClockwise(const double dt);
	virtual void SpinCounterClockwise(const double dt);
	virtual void Crouch(const double dt, std::vector<unsigned char> &heightMap, const Vector3 &terrainSize);

	// Applied methods
	virtual void Pitch(const double dt);
	virtual void Yaw(const double dt);
	virtual void Roll(const double dt);
	virtual void Walk(const double dt, std::vector<unsigned char> &heightMap, const Vector3 &terrainSize);
	virtual void Strafe(const double dt, std::vector<unsigned char> &heightMap, const Vector3 &terrainSize);
	virtual void Jump(const double dt);

	virtual void UpdateJump(const double dt, std::vector<unsigned char> &heightMap, const Vector3 &terrainSize);

private:
	bool myKeys[255];
	CAM_TYPE type;

	// Jump
	bool m_bJumping;
	float GRAVITY;
	float JumpVel;
	float JUMPMAXSPEED, JUMPACCEL;

	// Sprint
	bool sprint;
	bool crouch;
};

#endif