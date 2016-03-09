
/**
 * RRTimer.h : Class header for the Real Time Timers

 * @Prephix = RTT_

 * @Author: Ramón Mollá
 * @version: 2.1
 * @Date:	07/2014

 * Creative Commons: Attribution-NonCommercial 3.0 Unported (CC BY-NC 3.0) 2012-10 Valencia
 *
 * Update:
 * Date:
 * Version:
 * Changes:
 */

#ifndef RTTIMER_H
#define RTTIMER_H

#include <OS.h>

#ifdef	OS_MSWINDOWS
	#include <windows.h>
	#include <stdint.h>
	#define RTT_MAXTIME INT64_MAX
#elif	defined(OS_LINUX)
	#include <limits.h>
	#define RTT_MAXTIME LONG_MAX
#elif	defined(OS_OSX)
#elif	defined(OS_ANDROID)
#endif

#define RTT_Time long long
typedef unsigned char		RTT_Shorts	[8];
typedef unsigned short int	RTT_Ints	[4];
typedef unsigned long		RTT_Longs	[2];

//#define RTT_MM_COUNTERS
#define RTT_TIME_STAMP_ASM

class cRTTimer
{
	///Performance attributes

	///Processor sampling frequency for this given performance counter. Units: count/s
	///One count = System Clock Ticks / 1024. A internal 10 bits counter is plugged to the system clock. 
	///Everytime a carry of the highest bit is performed, this bit is counted by the HRTimer
	///So, SamplingFrequency = System clock frequency*2^-10
	///For instance. Let's suppose a system with a system clock frequency of 3GHz, SamplingFrequency = 2929687Hz, approx. 2.9MHz
	RTT_Time	SamplingFrequency,
				Time;
	double		SFms,				///Sampling frequency in ms
				msXTick;			///msXTick = 1/SFms

public:
	//Methods
	inline cRTTimer	()	{}
	inline ~cRTTimer()	{}

	///Gets the performance frequency and stores it in an private attribute.
	///The value is expressed in counts/s. One count is 1024 system clock ticks
	void ResetSF				();///<It stops the program at least one second to obtain the sampling frequency
	inline void SetSF			(RTT_Time	SF) {SamplingFrequency = SF; SFms = SF*0.001; msXTick = 1.0/SFms;};///<Sets the sampling frequency obtained outside the RTTimer. This is in order to avoid time penalties.

	inline RTT_Time GetSamplingFreq	()				{return SamplingFrequency;}

	///Time in ms
	inline double	Ticks2ms		(RTT_Time T)	{return T*msXTick;}
	inline RTT_Time ms2Ticks		(double ms)		{RTT_Time aux; aux = SFms*ms; return aux;}

	inline double GetSamplingFreqms	()				{return SFms;}
	//Sets in the T parameter the current real Time value in ticks
	inline RTT_Time GetRealTime()
	{
	#ifdef RTT_MM_COUNTERS
		#ifdef	OS_WINDOWS
			QueryPerformanceCounter((LARGE_INTEGER *)&Time);
			return Time;
		#elif	defined(OS_LINUX)
		#elif	defined(OS_OSX)
		#elif	defined(OS_ANDROID)
		#endif
	#elif defined(RTT_TIME_STAMP_ASM)
		RTT_Longs *Aux = (RTT_Longs*) &Time;
		//Intel or AMD assembler
		_asm
		{
			rdtsc
			mov ebx, Aux
			mov [ebx], eax
			mov [ebx+4], edx
		}
	#endif
	}
};
#endif