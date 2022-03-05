#pragma once

constexpr float PI{ 3.141592f };

float DegreeToRadian(float degree)
{
	// 180' = pi radian
	return PI * degree / 180.0f;
}

float RadianToDegree(float radian)
{
	return radian / PI * 180.0f;
}