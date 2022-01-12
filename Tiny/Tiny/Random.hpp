#pragma once

#include <random>

class Random
{
private:
	std::random_device rd;
	std::mt19937 mersenne;
public:
	Random()
		: mersenne(rd())
	{}

	int generate(int min, int max)
	{
		std::uniform_int_distribution<> generator(min, max);
		int gen = generator(mersenne);

		return gen;
	}

	float generate(float min, float max)
	{
		std::uniform_real_distribution<> generator(min, max);
		float gen = generator(mersenne);

		return gen;
	}
};