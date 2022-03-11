#pragma once

#include "../Math/Vector.hpp"
#include "../Math/Math.hpp"

#include <wtypes.h>
#include <cmath>
#include <iostream>

class Camera
{
private:
	Vec3f m_worldUp = { 0.0f, 1.0f, 0.0f };	// Actually it is const.
	Vec3f m_eye = { 0.0f, 4.0f, -9.0f };

	// Basis of camera space
	Vec3f m_u;
	Vec3f m_v;
	Vec3f m_n;

	float m_fovY = 45.0f;
	float m_aspect = 16.0f / 9.0f;
	float m_near = 1.5f;
	float m_far = 100.0f;


	const float m_speed = 0.05f;
	const float m_rotateSensitivity = 0.1f;

	void Orthonormalization();
public:
	float m_pitch = 25.0f;
	float m_yaw = 180.0f;
	Vec3f camRotation = { 0.0f, 0.0f, 0.0f };
	Camera();

	void Rotate(float xOffset, float yOffset, float alpha);
	void Move(int front, int right, int up, float alpha);

	// getter, setter
	Vec3f GetEye() const;
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
	void SetEye(Vec3f pos);
};