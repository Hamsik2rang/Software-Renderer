#pragma once

#include <random>

class Random
{
private:

public:
	static int GetRandomInteger(int min, int max);
	static float GetRandomReal(float min, float max);
};

int Random::GetRandomInteger(int min, int max)
{
	static std::random_device rd;
	static std::mt19937 mersenne(rd());

	std::uniform_int_distribution<> dis(min, max);
	int generated = dis(mersenne);
	return generated;
}

float Random::GetRandomReal(float min, float max)
{
	static std::random_device rd;
	static std::mt19937 mersenne(rd());

	std::uniform_real_distribution<> dis(min, max);
	float generated = (float)dis(mersenne);
	return generated;
}