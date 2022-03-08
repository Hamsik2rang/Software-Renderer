#include "Camera.h"

// Re-calculate Camera basis(n, u, v) when it rotate
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

void Camera::Rotate(float xOffset, float yOffset, float alpha)
{
	xOffset *= alpha * m_rotateSensitivity * -1.0f;
	yOffset *= alpha * m_rotateSensitivity * -1.0f;

	m_at = (GetTransform(m_eye.x, m_eye.y, m_eye.z)
		* GetRotate(xOffset, yOffset, 0.0f) 
		* GetTransform(-m_eye.x, -m_eye.y, -m_eye.z) 
		* m_at.CartesianToAffine()).AffineToCartesian();

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