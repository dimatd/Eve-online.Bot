#pragma once
#include <functional>
#include <memory>
#include <map>
#include <chrono>
#include <utility>

namespace nm
{
	using FAction = std::function<void()>;
	using TTime = long long;

	inline TTime GetCurrentTime()
	{
		auto now = std::chrono::system_clock::now();
		auto duration = now.time_since_epoch();
		TTime millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

		return millis;
	}

	inline TTime GetEndTime(double Seconds)
	{
		TTime Sec = static_cast<TTime>(Seconds * 1000);
		TTime EndTime = GetCurrentTime() + Sec;
		return EndTime;
	}

	class TimeHandler
	{
	public:
		inline TimeHandler(const FAction& NewAction, double Seconds, bool NewIsLoop)
			: Action(NewAction)
			, EndTime(nm::GetEndTime(Seconds))
			, Seconds(Seconds)
			, IsLoop(NewIsLoop)
			{}

		inline void Execute() { Action(); }

		inline bool   GetIsLoop () const { return IsLoop; }
		inline double GetSeconds() const { return Seconds; }
		inline TTime  GetEndTime() const { return EndTime; }

		inline void UpdateTime()
		{
			EndTime = nm::GetEndTime(Seconds);
		}

	private:
		FAction Action;
		TTime   EndTime;
		double  Seconds;
		bool    IsLoop;
	};

	using TimeHandlerPtr = std::shared_ptr<TimeHandler>;

	class TimerManager;
	using TimerManagerPtr = std::shared_ptr<TimerManager>;

	class TimerManager
	{
	public:
		static const TimerManagerPtr& Get();

		void Tick();

		template<class TUserClass>
		inline TimeHandlerPtr MakeTimer(const double Seconds, bool IsLoop, const std::shared_ptr<TUserClass>& UserClass, void(TUserClass::* UserFunction)())
		{
			TimeHandlerPtr Timer = std::make_shared<TimeHandler>(std::bind(UserFunction, UserClass), Seconds, IsLoop);
			AddTimer(Timer);

			return Timer;
		}

	protected:
		void AddTimer(const TimeHandlerPtr& Timer)
		{
			Timers.insert({Timer->GetEndTime(), Timer});
		}

	private:
		std::map<TTime, TimeHandlerPtr> Timers;
	};

	
}


