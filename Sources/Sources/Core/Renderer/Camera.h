#pragma once

#include "../Math/Vector.hpp"
#include "../Utility/Timer.hpp"

class Camera
{
private:
	Timer* m_pTimer;

	Vec3f m_eye = { 0.0f, 1.0f, 3.0f };
	Vec3f m_at = { 0.0f, 0.0f, 0.0f };
	Vec3f m_up = { 0.0f, 1.0f, 0.0f };

	Vec3f m_u;
	Vec3f m_v;
	Vec3f m_n;

	float m_fovY;
	float m_aspect;
	float m_near;
	float m_far;

	const float m_moveSpeed = 5.0f;
	const float m_rotateSpeed = 180.f;

	void Orthonormalization();
public:
	Camera();

	void SetTimer(Timer* timer);
	void Rotate(float pitch, float yaw);
	void Move(int front, int right);

};

