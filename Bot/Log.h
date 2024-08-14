#pragma once
#include <string>
#include <iostream>
#include "TimerManager.h"


inline void ELOG(const std::string& message)
{
	auto currentTime = nm::GetCurrentTime() / 1000;
	std::cout << currentTime << " : Error : " << message << std::endl;
}

inline void WLOG(const std::string& message)
{
	auto currentTime = nm::GetCurrentTime() / 1000;
	std::cout << currentTime << " : Warning : " << message << std::endl;
}

inline void ILOG(const std::string& message)
{
	auto currentTime = nm::GetCurrentTime() / 1000;
	std::cout << currentTime << " : Info : " << message << std::endl;
}