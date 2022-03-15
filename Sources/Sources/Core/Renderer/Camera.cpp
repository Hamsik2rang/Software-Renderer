#include "Camera.h"
#include "../Math/Matrix.hpp"

// Re-calculate Camera basis(n, u, v) when it rotate
void Camera::Orthonormalization()
{
	Vec3f front;
	front.x = std::sinf(DegreeToRadian(m_yaw)) * std::cosf(DegreeToRadian(m_pitch));
	front.y = std::sinf(DegreeToRadian(m_pitch));
	front.z = std::cosf(DegreeToRadian(m_yaw)) * std::cosf(DegreeToRadian(m_pitch));

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
	if (!m_bCanMove)
	{
		return;
	}
	m_pitch += yOffset * m_rotateSensitivity;
	m_yaw -= xOffset * m_rotateSensitivity;

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

void Camera::Move(int front, int right, int up, float alpha)
{
	if (!m_bCanMove)
	{
		return;
	}
	m_eye += (-m_n.normalize() * (float)front + m_u.normalize() * (float)right + m_v.normalize() * (float)up) * m_speed * alpha;
}

// getter, setter
Vec3f Camera::GetEye() const
{
	return m_eye;
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

void Camera::SetEye(Vec3f pos)
{
	m_eye = pos;
}

void Camera::SetIgnoreInput()
{
	m_bCanMove = false;
}
 
void Camera::UnsetIgnoreInput()
{
	m_bCanMove = true;
}

bool Camera::CanMove()
{
	return m_bCanMove;
}
