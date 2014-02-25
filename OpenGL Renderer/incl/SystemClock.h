#ifndef _SYSCLOCK_H_
#define _SYSCLOCK_H_

#include <Windows.h>

class SystemClock
{
public:

        SystemClock() : m_timeScale(1.0f),
                        m_isPaused(false),
                        m_recordedTime(timeGetTime())
        {

        }

        inline UINT32   getTime()
        {
            return static_cast<UINT32>( m_timeScale * m_recordedTime );
        }

        inline void     setTime()
        {
            m_recordedTime = timeGetTime();
        }

        inline UINT32   getTimeDelta()
        {
            return static_cast<UINT32>(m_timeScale * (timeGetTime() - m_recordedTime));
        }

        inline void setTimeScale(float scale)
        {
            m_timeScale = scale;
        }

private:

        DWORD       m_recordedTime;
	
        float       m_timeScale;

        bool        m_isPaused;
};

#endif