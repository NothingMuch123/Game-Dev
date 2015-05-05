#include "Camera3.h"
#include "Application.h"
#include "Mtx44.h"

Camera3::Camera3()
{
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

void Camera3::Update(double dt, bool *keypressed, std::vector<unsigned char> &heightMap, const Vector3 &terrainSize)
{
	Vector3 positionYTarget = target;
	positionYTarget.y = position.y;
	static const float TERRAINOFFSET = 10.f;

	static const float CAMERA_SPEED = 100.f;
	if(keypressed[Application::K_A])
	{
		Vector3 view = (positionYTarget - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		position -= right * CAMERA_SPEED * (float)dt;
		target -= right * CAMERA_SPEED * (float)dt;
		float yDifference = target.y - position.y;
		position.y = TERRAINOFFSET + terrainSize.y * ReadHeightMap(heightMap, position.x/terrainSize.x, position.z/terrainSize.z);
		target.y = position.y + yDifference;
	}
	if(keypressed[Application::K_D])
	{
		Vector3 view = (positionYTarget - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		position += right * CAMERA_SPEED * (float)dt;
		target += right * CAMERA_SPEED * (float)dt;
		float yDifference = target.y - position.y;
		position.y = TERRAINOFFSET + terrainSize.y * ReadHeightMap(heightMap, position.x/terrainSize.x, position.z/terrainSize.z);
		target.y = position.y + yDifference;
	}
	if(keypressed[Application::K_W])
	{
		Vector3 view = (positionYTarget - position).Normalized();
		position += view * CAMERA_SPEED * (float)dt;
		target += view * CAMERA_SPEED * (float)dt;
		float yDifference = target.y - position.y;
		position.y = TERRAINOFFSET + terrainSize.y * ReadHeightMap(heightMap, position.x/terrainSize.x, position.z/terrainSize.z);
		target.y = position.y + yDifference;
	}
	if(keypressed[Application::K_S])
	{
		Vector3 view = (positionYTarget - position).Normalized();
		position -= view * CAMERA_SPEED * (float)dt;
		target -= view * CAMERA_SPEED * (float)dt;
		float yDifference = target.y - position.y;
		position.y = TERRAINOFFSET + terrainSize.y * ReadHeightMap(heightMap, position.x/terrainSize.x, position.z/terrainSize.z);
		target.y = position.y + yDifference;
	}
	if (Application::IsKeyPressed('Q'))
	{
		float upMovement = (float)(CAMERA_SPEED * (float)dt);
		position.y += upMovement;
		target.y += upMovement;
	}
	if (Application::IsKeyPressed('E'))
	{
		float upMovement = (float)(-CAMERA_SPEED * (float)dt);
		position.y += upMovement;
		target.y += upMovement;
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

	//Update camera direction based on mouse movement
	// Left-right rotate
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
	// Up-down rotate
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