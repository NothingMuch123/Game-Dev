#include "Camera3.h"
#include "Application.h"
#include "Mtx44.h"

float Camera3::TERRAIN_OFFSET = 30.f;
float Camera3::CAMERA_SPEED = 100.f;
float Camera3::CROUCH_SPEED = 50.f;
float Camera3::CROUCH_OFFSET = 10.f;

Camera3::Camera3()
{
	for (int i = 0; i < 255; ++i)
	{
		myKeys[i] = false;
	}
}

Camera3::~Camera3()
{
}

void Camera3::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->sprint = false;
	this->m_bJumping = false;
	this->JumpVel = 0;
	this->JUMPMAXSPEED = 200.f;
	this->JUMPACCEL = 100.f;
	this->GRAVITY = -150.f;
	this->type = LAND_CAM;
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();
}

void Camera3::Update(double dt, std::vector<unsigned char> &heightMap, const Vector3 &terrainSize)
{
	if (myKeys[1])	// Sprint
	{
		sprint = true;
		CAMERA_SPEED = 200.f;
		myKeys[1] = false;
	}
	else
	{
		sprint = false;
		CAMERA_SPEED = 100.f;
	}
	if (myKeys[2])	// Crouch
	{
		crouch = true;
		Crouch(dt, heightMap, terrainSize);
		myKeys[2] = false;
	}
	else
	{
		crouch = false;
		Crouch(-dt, heightMap, terrainSize);
	}
	if(myKeys['a'])
	{
		Strafe(dt, heightMap, terrainSize);
		myKeys['a'] = false;
	}
	if(myKeys['d'])
	{
		Strafe(-dt, heightMap, terrainSize);
		myKeys['d'] = false;
	}
	if(myKeys['w'])
	{
		Walk(dt, heightMap, terrainSize);
		myKeys['w'] = false;
	}
	if(myKeys['s'])
	{
		Walk(-dt, heightMap, terrainSize);
		myKeys['s'] = false;
	}
	if (myKeys['q'])
	{
		MoveUp_Down(dt, 0, heightMap, terrainSize);
		myKeys['q'] = false;
	}
	if (myKeys['e'])
	{
		MoveUp_Down(dt, 1, heightMap, terrainSize);
		myKeys['e'] = false;
	}
	if (myKeys['z'])
	{
		SpinCounterClockwise(dt);
		myKeys['z'] = false;
	}
	if (myKeys['c'])
	{
		SpinClockwise(dt);
		myKeys['c'] = false;
	}
	if (myKeys[' '])
	{
		Jump(dt);
		myKeys[' '] = false;
	}
	UpdateJump(dt, heightMap, terrainSize);

	//Update camera direction based on mouse movement
	if (Application::camera_yaw != 0)
	{
		Yaw(dt);
	}
	if (Application::camera_pitch != 0)
	{
		Pitch(dt);
	}

	/*if(Application::IsKeyPressed(VK_LEFT))
	{
		Vector3 view = (target - position).Normalized();
		float yaw = (float)(CAMERA_SPEED * (float)dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		view = rotation * view;
		target = position + view;
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
	}
	if(Application::IsKeyPressed(VK_RIGHT))
	{
		Vector3 view = (target - position).Normalized();
		float yaw = (float)(-CAMERA_SPEED * (float)dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		view = rotation * view;
		target = position + view;
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
	}
	if(Application::IsKeyPressed(VK_UP))
	{
		float pitch = (float)(CAMERA_SPEED * (float)dt);
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		view = rotation * view;
		target = position + view;
	}
	if(Application::IsKeyPressed(VK_DOWN))
	{
		float pitch = (float)(-CAMERA_SPEED * (float)dt);
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		view = rotation * view;
		target = position + view;
	}*/

	if(myKeys['t'])
	{
		Reset();
		myKeys['t'] = false;
	}
}

void Camera3::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}

void Camera3::SetCameraType(CAM_TYPE type)
{
	this->type = type;
}

Camera3::CAM_TYPE Camera3::GetCameraType()
{
	return type;
}

void Camera3::MoveForward_Backward(double dt, bool dir, std::vector<unsigned char> &heightMap, const Vector3 &terrainSize) // 0 - Forward, 1 - Backwards
{
	Vector3 positionYTarget = target;
	positionYTarget.y = position.y;
	float speed = CAMERA_SPEED;
	
	if (dir)
	{
		speed = -speed;
	}

	Vector3 view = (positionYTarget - position).Normalized();
	position += view * speed * (float)dt;
	target += view * speed * (float)dt;

	if (!m_bJumping)
	{
		// Terrain y-axis
		float yDifference = target.y - position.y;
		if (crouch)
		{
			position.y = (TERRAIN_OFFSET - CROUCH_OFFSET) + terrainSize.y * ReadHeightMap(heightMap, position.x/terrainSize.x, position.z/terrainSize.z);
		}
		else
		{
			position.y = TERRAIN_OFFSET + terrainSize.y * ReadHeightMap(heightMap, position.x/terrainSize.x, position.z/terrainSize.z);
		}
		target.y = position.y + yDifference;
	}
}

void Camera3::MoveLeft_Right(double dt, bool dir, std::vector<unsigned char> &heightMap, const Vector3 &terrainSize)		// 0 - Left, 1 - Right
{
	Vector3 positionYTarget = target;
	positionYTarget.y = position.y;
	float speed = CAMERA_SPEED;

	if (dir)
	{
		speed = -speed;
	}

	// Moving forward or backwards
	Vector3 view = (positionYTarget - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	position += right * speed * (float)dt;
	target += right * speed * (float)dt;

	if (!m_bJumping)
	{
		// Terrain y-axis
		float yDifference = target.y - position.y;
		if (crouch)
		{
			position.y = (TERRAIN_OFFSET - CROUCH_OFFSET) + terrainSize.y * ReadHeightMap(heightMap, position.x/terrainSize.x, position.z/terrainSize.z);
		}
		else
		{
			position.y = TERRAIN_OFFSET + terrainSize.y * ReadHeightMap(heightMap, position.x/terrainSize.x, position.z/terrainSize.z);
		}
		target.y = position.y + yDifference;
	}
}

void Camera3::MoveUp_Down(double dt, bool dir, std::vector<unsigned char> &heightMap, const Vector3 &terrainSize)
{
	float speed = CAMERA_SPEED;

	if (dir)
	{
		speed = -speed;
	}

	float up_downMovement = (float)(speed * (float)dt);
	position.y += up_downMovement;
	target.y += up_downMovement;
}

void Camera3::UpdateStatus(unsigned char key)
{
	myKeys[key] = true;
}

void Camera3::LookUp(const double dt)
{
	float pitch = (float)(-CAMERA_SPEED * Application::camera_pitch * (float)dt);
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	up = right.Cross(view).Normalized();
	Mtx44 rotation;
	rotation.SetToRotation(pitch, right.x, right.y, right.z);
	view = rotation * view;
	target = position + view;
}

void Camera3::LookDown(const double dt)
{
	float pitch = (float)(-CAMERA_SPEED * Application::camera_pitch * (float)dt);
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	up = right.Cross(view).Normalized();
	Mtx44 rotation;
	rotation.SetToRotation(pitch, right.x, right.y, right.z);
	view = rotation * view;
	target = position + view;
}

void Camera3::TurnLeft(const double dt)
{
	Vector3 view = (target - position).Normalized();
	float yaw = (float)(-CAMERA_SPEED * Application::camera_yaw * (float)dt);
	Mtx44 rotation;
	rotation.SetToRotation(yaw, 0,1,0);
	view = rotation * view;
	target = position + view;
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	up = right.Cross(view).Normalized();
}

void Camera3::TurnRight(const double dt)
{
	Vector3 view = (target - position).Normalized();
	float yaw = (float)(-CAMERA_SPEED * Application::camera_yaw * (float)dt);
	Mtx44 rotation;
	rotation.SetToRotation(yaw, 0,1,0);
	view = rotation * view;
	target = position + view;
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	up = right.Cross(view).Normalized();
}

void Camera3::SpinClockwise(const double dt)
{
	Mtx44 rotate;
	Vector3 view = target - position;
	rotate.SetToRotation(-dt * CAMERA_SPEED, view.x, view.y, view.z);
	up = rotate * up;
}

void Camera3::SpinCounterClockwise(const double dt)
{
	Mtx44 rotate;
	Vector3 view = target - position;
	rotate.SetToRotation(dt * CAMERA_SPEED, view.x, view.y, view.z);
	up = rotate * up;
}

void Camera3::Crouch(const double dt, std::vector<unsigned char> &heightMap, const Vector3 &terrainSize)
{
	if (dt > 0)
	{
		CAMERA_SPEED = 50.f;
		float crouchY = ((TERRAIN_OFFSET + terrainSize.y * ReadHeightMap(heightMap, position.x/terrainSize.x, position.z/terrainSize.z)) - CROUCH_OFFSET);
		if (position.y > crouchY)
		{
			position.y -= CROUCH_SPEED * dt;
			target.y -= CROUCH_SPEED * dt;
			if (position.y < crouchY)
			{
				float yDifference = target.y - position.y;
				position.y = crouchY;
				target.y = crouchY + yDifference;
			}
		}
	}
	else if (dt < 0)
	{
		if (!sprint)
		{
			CAMERA_SPEED = 100.f;
		}
		float normalY = (TERRAIN_OFFSET + terrainSize.y * ReadHeightMap(heightMap, position.x/terrainSize.x, position.z/terrainSize.z));
		if (position.y < normalY)
		{
			position.y += CROUCH_SPEED * -dt;
			target.y += CROUCH_SPEED * -dt;
			if (position.y > normalY)
			{
				float yDifference = target.y - position.y;
				position.y = normalY;
				target.y = normalY + yDifference;
			}
		}
	}
}

void Camera3::Pitch(const double dt)
{
	if (Application::camera_pitch > 0.0)
	{
		LookUp(dt);
	}
	else if (Application::camera_pitch < 0.0)
	{
		LookDown(dt);
	}
}

void Camera3::Yaw(const double dt)
{
	if (Application::camera_yaw > 0.0)
	{
		TurnRight(dt);
	}
	else if (Application::camera_yaw < 0.0)
	{
		TurnLeft(dt);
	}
}

void Camera3::Roll(const double dt)
{

}

void Camera3::Walk(const double dt, std::vector<unsigned char> &heightMap, const Vector3 &terrainSize)
{
	if (dt > 0)
	{
		MoveForward_Backward(dt, false, heightMap, terrainSize);
	}
	else if (dt < 0)
	{
		MoveForward_Backward(abs(dt), true, heightMap, terrainSize);
	}
}

void Camera3::Strafe(const double dt, std::vector<unsigned char> &heightMap, const Vector3 &terrainSize)
{
	if (dt > 0)
	{
		MoveLeft_Right(dt, true, heightMap, terrainSize);
	}
	else if (dt < 0)
	{
		MoveLeft_Right(abs(dt), false, heightMap, terrainSize);
	}
}

void Camera3::Jump(const double dt)
{
	if (!m_bJumping)
	{
		m_bJumping = true;

		// Calculate the jump velocity
		JumpVel = JUMPACCEL;

		// Factor in maximum speed limit
		if (JumpVel > JUMPMAXSPEED)
		{
			JumpVel = JUMPMAXSPEED;
		}
	}
}

void Camera3::UpdateJump(const double dt, std::vector<unsigned char> &heightMap, const Vector3 &terrainSize)
{
	if(m_bJumping)
	{
		// Factor in gravity
		JumpVel += GRAVITY * dt;

		// Update the camera and target position
		position.y += JumpVel * (float)dt;
		target.y += JumpVel * (float)dt;

		// Check if the camera has reached the ground
		float yDifference = target.y - position.y;
		float yValue = TERRAIN_OFFSET + terrainSize.y * ReadHeightMap(heightMap, position.x/terrainSize.x, position.z/terrainSize.z);
		if (position.y < yValue)
		{
			position.y = yValue;
			target.y = position.y + yDifference;
			JumpVel = 0.f;
			m_bJumping = false;
		}
	}
}