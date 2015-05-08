#include "Camera3.h"
#include "Application.h"
#include "Mtx44.h"

static const float TERRAIN_OFFSET = 20.f;
static const float CAMERA_SPEED = 100.f;

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
	if(myKeys['a'])
	{
		MoveLeft_Right(dt, 0, heightMap, terrainSize);
		myKeys['a'] = false;
	}
	if(myKeys['d'])
	{
		MoveLeft_Right(dt, 1, heightMap, terrainSize);
		myKeys['d'] = false;
	}
	if(myKeys['w'])
	{
		MoveForward_Backward(dt, 0, heightMap, terrainSize);
		myKeys['w'] = false;
	}
	if(myKeys['s'])
	{
		MoveForward_Backward(dt, 1, heightMap, terrainSize);
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

	//Update camera direction based on mouse movement
	if (myKeys['i'])
	{
		TurnUp(dt);
		myKeys['i'] = false;
	}
	if (myKeys['k'])
	{
		TurnDown(dt);
		myKeys['k'] = false;
	}
	if (myKeys['j'])
	{
		TurnLeft(dt);
		myKeys['j'] = false;
	}
	if (myKeys['l'])
	{
		TurnRight(dt);
		myKeys['l'] = false;
	}

	if(Application::IsKeyPressed(VK_LEFT))
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
	}

	if(Application::IsKeyPressed('R'))
	{
		Reset();
	}
}

void Camera3::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
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

	// Terrain y-axis
	float yDifference = target.y - position.y;
	position.y = TERRAIN_OFFSET + terrainSize.y * ReadHeightMap(heightMap, position.x/terrainSize.x, position.z/terrainSize.z);
	target.y = position.y + yDifference;
}

void Camera3::MoveLeft_Right(double dt, bool dir, std::vector<unsigned char> &heightMap, const Vector3 &terrainSize)		// 0 - Left, 1 - Right
{
	Vector3 positionYTarget = target;
	positionYTarget.y = position.y;
	float speed = CAMERA_SPEED;

	if (!dir)
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

	// Terrain y-axis
	float yDifference = target.y - position.y;
	position.y = TERRAIN_OFFSET + terrainSize.y * ReadHeightMap(heightMap, position.x/terrainSize.x, position.z/terrainSize.z);
	target.y = position.y + yDifference;
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

void Camera3::TurnUp(double dt)
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

void Camera3::TurnDown(double dt)
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

void Camera3::TurnLeft(double dt)
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

void Camera3::TurnRight(double dt)
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