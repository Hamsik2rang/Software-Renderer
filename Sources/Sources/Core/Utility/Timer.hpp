#pragma once

#include <chrono>

class Timer
{
private:
	using clock_t = std::chrono::high_resolution_clock;
	using millisecond_t = std::chrono::duration<unsigned long long, std::ratio<1, 1000>>;	// count 1 per millisecond

	std::chrono::time_point<clock_t> m_startTime;
	std::chrono::time_point<clock_t> m_lastTime;
public:
	Timer()
		: m_startTime{ clock_t::now() }, m_lastTime{ clock_t::now() }
	{}

	void Start()
	{
		m_lastTime = clock_t::now();
	}

	unsigned long long End()
	{
		auto curTime = clock_t::now();
		unsigned long long dur = std::chrono::duration_cast<millisecond_t>(curTime - m_lastTime).count();

		return dur;
	}

	unsigned long long GetElapsedTime()
	{
		unsigned long long curElapsedTime = std::chrono::duration_cast<millisecond_t>(clock_t::now() - m_startTime).count();
		
		return curElapsedTime;
	}
};