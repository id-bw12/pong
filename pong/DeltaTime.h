#include <chrono>

using namespace std; 
using namespace chrono;

typedef system_clock::time_point TimePoint;
typedef system_clock Clock;
namespace timer
{
	class DeltaTime
	{
	public:
		DeltaTime(){};
		DeltaTime(bool isOn) : m_startTime(Clock::now()), m_isOn(isOn){};
		~DeltaTime(){};
		void setTimeLimit(float timeLimit);//{m_timeLimit = duration<int>(timeLimit);}
		void resetStartTime(){ m_startTime = Clock::now(); }
		void offAndOnSwitch(bool isOn){ m_isOn = isOn; };
		bool checkDelta();
		bool getPowerSwitch(){ return m_isOn; }

	private:
		TimePoint m_startTime, m_elapseTime;
		milliseconds m_deltaTime, m_timeLimit;
		bool m_isOn = false;
	};

	typedef DeltaTime Timer; 
}

