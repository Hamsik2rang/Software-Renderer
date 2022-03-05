#include "Camera.h"
#include "../Utility/Timer.hpp"
#include <cmath>


void Camera::Orthonormalization()
{
	m_n = (m_eye - m_at).normalize();
	m_u = (m_worldUp ^ m_n).normalize();
	m_v = m_n ^ m_u;
}

Camera::Camera()
{
	Orthonormalization();
}

//void Camera::SetTimer(Timer* timer)
//{
//	m_pTimer = timer;
//}

void Camera::Rotate(float pitch, float yaw, float deltaTime)
{
	//NOTE: Is it work?
	auto direction = m_at - m_eye;
	direction.x *= std::sin(yaw)* std::cos(pitch);
	direction.y *= std::sin(pitch);
	direction.z *= std::cos(yaw) * std::cos(pitch);
	m_at = (m_eye + direction).normalize() * (m_at - m_eye).length();
	Orthonormalization();
}

void Camera::Move(int front, int right, float deltaTime)
{
	if (front)
	{
		auto direction = m_at - m_eye;
		direction.normalize();
		m_eye += direction * m_speed * deltaTime * (float)front;
		m_at += direction * m_speed * deltaTime * (float)front;
	}
	if (right)
	{
		// right-handed coordinate
		auto direction = (m_at - m_eye) ^ m_worldUp;
		direction.normalize();
		m_eye += direction * m_speed * deltaTime * (float)right;
		m_at += direction * m_speed * deltaTime * (float)right;
	}
}

// getter, setter
Vec3f Camera::GetEye() const
{
	return m_eye;
}

Vec3f Camera::GetAt() const
{
	return m_at;
}

Vec3f Camera::GetWorldUp() const
{
	return m_worldUp;
}

Vec3f Camera::GetRight() const
{
	return m_u;
}

Vec3f Camera::GetUp() const
{
	return m_v;
}

Vec3f Camera::GetFront() const
{
	return m_n;
}

float Camera::GetAspect() const
{
	return m_aspect;
}

float Camera::GetFovY() const
{
	return m_fovY;
}

float Camera::GetNear() const
{
	return m_near;
}

float Camera::GetFar() const
{
	return m_far;
}

void Camera::SetAspect(float aspect)
{
	m_aspect = aspect;
}

void Camera::SetAspect(DWORD width, DWORD height)
{
	m_aspect = (float)width / (float)height;
}

void Camera::SetFov(float fovY)
{
	m_fovY = fovY;
}