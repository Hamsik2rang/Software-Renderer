#pragma once

#include "../Math/Vector.hpp"
#include "../Utility/Timer.hpp"

#include <wtypes.h>

class Camera
{
private:
	Vec3f m_worldUp = { 0.0f, 1.0f, 0.0f };	// Actually it is const.
	Vec3f m_eye = { 0.0f, 1.0f, 3.0f };
	Vec3f m_at = { 0.0f, 0.0f, -1.0f };

	Vec3f m_u;
	Vec3f m_v;
	Vec3f m_n;

	float m_fovY = 45.0f;
	float m_aspect = 16.0f / 9.0f;
	float m_near = 1.0f;
	float m_far = 100.0f;

	const float m_speed = 0.005f;

	void Orthonormalization();
public:
	Camera();

	void Rotate(float pitch, float yaw, float deltaTime);
	void Move(int front, int right, float deltaTime);

	// getter, setter
	Vec3f GetEye() const;
	Vec3f GetAt() const;
	Vec3f GetWorldUp() const;

	Vec3f GetRight() const;
	Vec3f GetUp() const;
	Vec3f GetFront() const;

	float GetAspect() const;
	float GetFovY() const;
	float GetNear() const;
	float GetFar() const;

	void SetAspect(float aspect);
	void SetAspect(DWORD width, DWORD height);
	void SetFov(float fovY);
};

