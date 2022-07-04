#pragma once
#include <iostream>
#include <chrono>
// Scope-based Timer
class Timer
{
public:
	Timer()
	{
		mStartTimepoint = std::chrono::high_resolution_clock::now();
	}

	double GetElapsedTime()
	{
		auto EndTimepoint = std::chrono::high_resolution_clock::now();

		auto start = std::chrono::time_point_cast<std::chrono::microseconds>(mStartTimepoint).time_since_epoch().count();
		auto end = std::chrono::time_point_cast<std::chrono::microseconds>(EndTimepoint).time_since_epoch().count();

		auto duration = end - start;
		double ms = duration * 0.001;

		return ms;
	}
private:
	std::chrono::time_point< std::chrono::high_resolution_clock> mStartTimepoint;
};