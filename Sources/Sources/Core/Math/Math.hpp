#pragma once

const float PI = std::acosf(-1);

inline float DegreeToRadian(float degree)
{
	// 180' = pi radian
	return degree * (PI / 180.0f);
}

inline float RadianToDegree(float radian)
{
	return radian * (180.0f / PI);
}

