#include "DeltaTime.h"

namespace timer
{
	void DeltaTime::setTimeLimit(float timeLimit)
	{
		m_timeLimit = duration_cast<milliseconds>(duration<float>(timeLimit));
	}

	 bool DeltaTime::checkDelta()
	{
		bool limitReach = false;
		if (m_isOn)
		{
			m_elapseTime = Clock::now();
			m_deltaTime = duration_cast<milliseconds>(m_elapseTime - m_startTime);
			if (m_deltaTime > m_timeLimit)
				limitReach = true;
		}

		return limitReach;
	}

}