#include "Camera.h"
#include "../Utility/Timer.hpp"
#include <cmath>


void Camera::Orthonormalization()
{
	m_n = (m_eye - m_at).normalize();
	m_u = (m_up ^ m_n).normalize();
	m_v = m_n ^ m_u;
}

Camera::Camera()
{
	Orthonormalization();
}

void Camera::SetTimer(Timer* timer)
{
	m_pTimer = timer;
}

void Camera::Rotate(float pitch, float yaw)
{
	//NOTE: Is it work?
	auto direction = m_at - m_eye;
	direction.x *= std::sin(yaw);
	direction.y *= std::sin(pitch);
	direction.z *= std::cos(pitch) * std::cos(yaw);
	m_at = (m_eye + direction).normalize() * (m_at - m_eye).length();
	Orthonormalization();
}

void Camera::Move(int front, int right)
{
	auto deltaTime = m_pTimer->Elapsed();
	if (front)
	{
		auto direction = m_at - m_eye;
		direction.normalize();
		m_eye += direction * m_moveSpeed * deltaTime * (float)front;
		m_at += direction * m_moveSpeed * deltaTime * (float)front;
	}
	if (right)
	{
		// right-handed coordinate
		auto direction = (m_at - m_eye) ^ m_up;
		direction.normalize();
		m_eye += direction * m_moveSpeed * deltaTime * (float)right;
		m_at += direction * m_moveSpeed * deltaTime * (float)right;
	}\
}
