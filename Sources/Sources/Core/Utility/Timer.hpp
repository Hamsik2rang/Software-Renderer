#pragma once

#include <chrono>

class Timer
{
private:
	using clock_t = std::chrono::high_resolution_clock;
	using millisecond_t = std::chrono::duration<float, std::ratio<1, 1000>>;	// count 1 per millisecond
	
public:
	static float Elapsed()
	{
		static std::chrono::time_point<clock_t> startTime = clock_t::now();
		float curElapsedTime = std::chrono::duration_cast<millisecond_t>(clock_t::now() - startTime).count();

		return curElapsedTime;
	}
};