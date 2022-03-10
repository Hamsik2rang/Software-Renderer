#include "Camera.h"
#include "../Math/Math.hpp"
#include "../Math/Matrix.hpp"

// Re-calculate Camera basis(n, u, v) when it rotate
void Camera::Orthonormalization()
{
	Vec3f front;
	front.x = std::cosf(DegreeToRadian(m_yaw)) * std::cosf(DegreeToRadian(m_pitch));
	front.y = std::sinf(DegreeToRadian(m_pitch));
	front.z = std::sinf(DegreeToRadian(m_yaw)) * std::cosf(DegreeToRadian(m_pitch));

	m_n = front.normalize();
	m_u = (m_worldUp ^ m_n).normalize();
	m_v = m_n ^ m_u;
}

Camera::Camera()
{
	Orthonormalization();
}

void Camera::Rotate(float xOffset, float yOffset, float alpha)
{
	m_pitch += xOffset * m_rotateSensitivity;
	m_yaw += yOffset * m_rotateSensitivity;

	if (m_pitch > 89.0f)
	{
		m_pitch = 89.0f;
	}
	else if (m_pitch < -89.0f)
	{
		m_pitch = -89.0f;
	}

	Orthonormalization();
}

void Camera::Move(int front, int right, float alpha)
{
	if (front)
	{
		// right-handed coordinate
		Vec3f direction = m_n * -1.0f;
		
		direction.normalize();
		m_eye += direction * m_speed * alpha * (float)front;
		m_at += direction * m_speed * alpha * (float)front;
	}
	if (right)
	{
		Vec3f direction = m_u;
		direction.normalize();
		m_eye += direction * m_speed * alpha * (float)right;
		m_at += direction * m_speed * alpha * (float)right;
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