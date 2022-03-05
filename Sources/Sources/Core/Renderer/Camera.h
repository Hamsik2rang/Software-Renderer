#pragma once

#include "../Math/Vector.hpp"
#include "../Utility/Timer.hpp"

#include <wtypes.h>

class Camera
{
private:
	Timer* m_pTimer;

	Vec3f m_eye = { 0.0f, 1.0f, 3.0f };
	Vec3f m_at = { 0.0f, 0.0f, -1.0f };
	Vec3f m_worldUp = { 0.0f, 1.0f, 0.0f };

	Vec3f m_u;
	Vec3f m_v;
	Vec3f m_n;

	float m_fovY = 45.0f;
	float m_aspect = 16.0f / 9.0f;
	float m_near = 1.0f;
	float m_far = 101.0f;

	const float m_speed = 5.0f;

	void Orthonormalization();
public:
	Camera();

	void SetTimer(Timer* timer);
	void Rotate(float pitch, float yaw);
	void Move(int front, int right);

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

