#pragma once

#include <chrono>

class Timer
{
private:
	using clock_t = std::chrono::high_resolution_clock;
	using millisecond_t = std::chrono::duration<float, std::ratio<1, 1000>>;	// count 1 per millisecond
	
	std::chrono::time_point<clock_t> StartTime;

public:
	void Start()
	{
		StartTime = clock_t::now();
	}

	float Elapsed()
	{
		float curElapsedTime = std::chrono::duration_cast<millisecond_t>(clock_t::now() - StartTime).count();

		return curElapsedTime;
	}
};