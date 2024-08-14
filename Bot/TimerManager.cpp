#include "TimerManager.h"
#include <vector>

namespace nm
{
	const TimerManagerPtr& TimerManager::Get()
	{
		static TimerManagerPtr Instance;
		if(!Instance)
		{
			Instance = std::make_shared<TimerManager>();
		}

		return Instance;
	}

	void TimerManager::Tick()
	{
		auto currentTime = GetCurrentTime();

		std::vector<TimeHandlerPtr> LoopTimers;
		for (auto it = Timers.begin(); it != Timers.end();)
		{
			if(it->first <= currentTime)
			{
				auto Timer = it->second;

				Timer->Execute();
				if(Timer->GetIsLoop())
				{
					LoopTimers.push_back(Timer);
				}

				it = Timers.erase(it);
			}
			else
			{
				++it;
			}

			if(it != Timers.end() && it->first > currentTime)
			{
				break;
			}
		}

		for(auto Timer : LoopTimers)
		{
			Timer->UpdateTime();
			AddTimer(Timer);
		}
	}
}
