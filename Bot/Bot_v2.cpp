#include "Bot_v2.h"
#include <windows.h>
#include <memory>
#include "Log.h"
#include <synchapi.h>

class Test
{
public:
	void OnEvent()
	{
		ILOG("OnEvent");
	}
};

using TestPtr = std::shared_ptr<Test>;

//using namespace nm;

void RunBot_v3()
{
	TestPtr Timer = std::make_shared<Test>();

	nm::TimerManager::Get()->MakeTimer(2, true, Timer, &Test::OnEvent);

	auto FixedDeltaTime = (1.f / 30.f) * 1000;

	//while(!IsEngineExitRequested())
	for(int i = 0; i < 1000; i++)
	{
		nm::TTime StartTime = GetCurrentTime();

		nm::TimerManager::Get()->Tick();

		nm::TTime CurrentDelta = GetCurrentTime() - StartTime;
		if(CurrentDelta < FixedDeltaTime)
		{
			Sleep(static_cast<DWORD>(FixedDeltaTime - CurrentDelta));
		}
	}
}

