#include "TimerMgr.h"

#include <list>
#include "zTime.h"

TimerMgr::TimerMgr() 
: m_dwCurSeq(1)
{
	zRTime currentTime;
    m_tCurTime = currentTime.local_sec();
}

void TimerMgr::Update(time_t localtime)
{
	m_tCurTime = localtime;

    std::list<int> lstDelTimers;
    std::list<int> lstTimers;

	for(auto itr=m_mapTimerItems.begin();itr!=m_mapTimerItems.end();itr++)
    {
        lstTimers.push_back(itr->first);
    }

    for(auto itr=lstTimers.begin(); itr!=lstTimers.end();itr++)
    {
    	const int nTimerId = *itr;
        TimerItemMap::iterator itrFind = m_mapTimerItems.find(nTimerId);
        if (itrFind != m_mapTimerItems.end())
        {
            S_TimerItem& sTimerItem = itrFind->second;

            //std::string strfunc = sTimerItem.m_strFunc;

            if( m_tCurTime >= sTimerItem.m_timer)
            {
                if (sTimerItem.m_bCircle)
                {
                    sTimerItem.m_timer += sTimerItem.m_dwInterval;
                    if(sTimerItem.m_timer > sTimerItem.m_end_time)
					{
						lstDelTimers.push_back(itrFind->first);
					}
                }
                else
                {
                    lstDelTimers.push_back(itrFind->first);
                }

                sTimerItem.m_func();
            }
        }
    }

    for(auto itr=lstDelTimers.begin();itr!=lstDelTimers.end();itr++)
    {
    	const int nTimerId = *itr;
        m_mapTimerItems.erase(nTimerId);
    }
}

void TimerMgr::CancelTimer(DWORD iTimer)
{
    m_mapTimerItems.erase(iTimer);
}

static DWORD DayStartOfTime(time_t localtime)
{
	DWORD sec = localtime %  DAY_SECS;
	return localtime - sec;
}

static DWORD HourStartOfTime(time_t localtime )
{
	DWORD sec = localtime %  HOUR;
	return localtime - sec;
}

static DWORD WeekStartOfTime(time_t localtime)
{
	struct tm tv1;
	gmtime_r(&localtime, &tv1);
	DWORD sec = tv1.tm_sec + tv1.tm_min* 60 + tv1.tm_hour * 3600 + ((tv1.tm_wday+6)%7) * DAY_SECS;
	if (localtime > sec)
	{
		return localtime - sec;
	}

	return 0;
}

DWORD TimerMgr::AddDayStaticTimer(DWORD tTrigger, TimerFunction func)
{
    time_t tStart = DayStartOfTime(m_tCurTime) + tTrigger;

    return AddCircleTimer(tStart, func, DAY_SECS);
}

DWORD TimerMgr::AddHourStaticTimer(DWORD tTrigger,TimerFunction func)
{
	time_t tStart = HourStartOfTime(m_tCurTime) + tTrigger;

	return AddCircleTimer(tStart, func, HOUR);
}

DWORD TimerMgr::AddWeekStaticTimer(DWORD tTrigger, TimerFunction func)
{
	time_t tStart = WeekStartOfTime(m_tCurTime) + tTrigger;

	return AddCircleTimer(tStart, func, WEEK);
}

time_t TimerMgr::default_end_time()
{
	return m_tCurTime + 15 * 365 * 24 * 3600; 
}

DWORD TimerMgr::AddOnceTimer(time_t tTrigger,TimerFunction func)
{
    if(m_tCurTime > tTrigger)
	{
		return 0;
	}
    
    S_TimerItem timer;
    timer.m_func = func;
    timer.m_timer = tTrigger;
    timer.m_bCircle = false;
    timer.m_dwInterval = 0;
    timer.m_dwSeq = m_dwCurSeq;
	timer.m_end_time = default_end_time();

    m_mapTimerItems[m_dwCurSeq] = timer;

    return m_dwCurSeq++;
}

DWORD TimerMgr::AddOnceTimerFromNow(time_t tTrigger, TimerFunction func)
{
    tTrigger += m_tCurTime;
    return AddOnceTimer(tTrigger, func);
}

DWORD TimerMgr::AddCircleTimer(time_t tStart,TimerFunction func, DWORD dwInterval)
{
	while(true)
	{
		if(tStart > m_tCurTime)
			break;

		tStart += dwInterval;
	}

	S_TimerItem timer;
	timer.m_timer = tStart;
	timer.m_bCircle = true;
	timer.m_dwInterval = dwInterval;
	timer.m_dwSeq = m_dwCurSeq;
	timer.m_end_time = default_end_time();
	timer.m_func = func;

	m_mapTimerItems[m_dwCurSeq] = timer;

	return m_dwCurSeq++;
}

DWORD TimerMgr::AddCircleTimerFromNow(DWORD dwInterval,TimerFunction func)
{
	time_t tStart = m_tCurTime + dwInterval;
	return AddCircleTimer(tStart,func,dwInterval);	
}

DWORD TimerMgr::AddCircleTimer(time_t tStart, time_t tEnd, TimerFunction func, DWORD dwInterval)
{
	while(true)
	{   
		if(tStart > m_tCurTime)
			break;

		tStart += dwInterval;
	}   

	if(tStart > tEnd)
	{
		return 0;
	}

	S_TimerItem timer;
	timer.m_timer = tStart;
	timer.m_bCircle = true;
	timer.m_dwInterval = dwInterval;
	timer.m_dwSeq = m_dwCurSeq;
	timer.m_end_time = tEnd;
	timer.m_func = func;

	m_mapTimerItems[m_dwCurSeq] = timer;

	return m_dwCurSeq++;
}

