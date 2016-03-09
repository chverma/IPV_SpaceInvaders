/**
 * RTTimer.cpp : Class declarations for the Real Time Timers

 * @Prephix = RTT_	Comes from Real Time Timer

 * @Author: Ramón Mollá
 * Version: 2.0
 * @Date:	07/2014

 * Creative Commons: Attribution-NonCommercial 3.0 Unported (CC BY-NC 3.0) 2012-10 Valencia
 *
 * Update:
 * Date:
 * Changes:
 */

#include <RTTimer.h>

void cRTTimer::ResetSF(void)
{
#ifdef	OS_MSWINDOWS
		QueryPerformanceFrequency((LARGE_INTEGER *)&SamplingFrequency);
	#ifdef RTT_TIME_STAMP_ASM
		LARGE_INTEGER	TS, TS1S;	//Time Stamp and TS plus one second
		RTT_Time RT;

		RT = GetRealTime();
		QueryPerformanceCounter(&TS);
		for( TS1S.QuadPart = TS.QuadPart + GetSamplingFreq(); TS.QuadPart < TS1S.QuadPart; QueryPerformanceCounter(&TS));
		SamplingFrequency = GetRealTime() - RT;
	#endif
#elif	defined(OS_LINUX)
#elif	defined(OS_OSX)
#elif	defined(OS_ANDROID)
#endif

	SetSF(SamplingFrequency);
}
