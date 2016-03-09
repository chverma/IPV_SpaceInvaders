/**
 * HRTimer.h : Class header for the High Resolution Timers

 * @Prephix = HRT_

 * @Author: Ramón Mollá
 * @version: 2.0
 * @Date:	12/2013
 * Creative Commons: Attribution-NonCommercial 3.0 Unported (CC BY-NC 3.0) 2012-10 Valencia
 *
 * Update:
 * Date:
 * Version:
 * Changes:
 */

#ifndef HRTIMER_H
#define HRTIMER_H

#include <string>
#include <RTTimer.h>

#ifdef	OS_MSWINDOWS
	//#define DLL_EXPORTS
	#ifdef DLL_EXPORTS
	#define HRT_API __declspec(dllexport)
	#else
	#define HRT_API
	#endif

	#define HRT_Time RTT_Time

#elif	defined(OS_LINUX)
#elif	defined(OS_OSX)
#elif	defined(OS_ANDROID)
#endif

//64 bits constants
#define HRT_TIME_INVALID	-1LL
#define HRT_NOTIME			 0LL
#define HRT_INMEDIATELY		 0LL

typedef enum
{
	HRT_INACTIVE,	///The HRT is not counting now. State reached just after EndCounting(); is invoked
	HRT_ACTIVE,		///The HRT is counting now. State reached just after InitCounting(); is invoked
	HRT_MAX_STATES	///Amount of different states a HRT can reach
} HRT_STATES;

class HRT_API cHRTimer: public cRTTimer
{
protected:
	std::string Name;

	HRT_STATES State;

	///Performance attributes

	HRT_Time	InitialCounter,	///<Initial performance counter value at the very beginning of the measured process. Units: System Clock Ticks
				FinalCounter,	///<Final performance counter value at the very end of the measured process. Units: System Clock Ticks

	///Statistical derived values
				Accumulated,	///<Total performance counter values accumulated during every measured process. Units: System Clock Ticks
								///The same use as Accumulatedms but in ticks. Take into account that the amount of ticks used to perform a computational task 
				AccumulatedACPI,///<remains always equal even if the system clock frequency changes, but not time consumed.
				LastPeriod,		///<Performance counter values accumulated during the last measured process. Units: System Clock Ticks
				MaxPartial,		///<Maximun partial period detected during every measured process. Units: System Clock Ticks
				MinPartial,		///<Maximun partial period detected during every measured process. Units: System Clock Ticks
				Alarm;			///<Total Amount of time to wait from now on until the alarm sounds

	unsigned int	Periods;		///<Amount of times a period has been accumulated

public:
	//Methods
	cHRTimer	() {Name.clear(); Reset();}
	~cHRTimer	() {}

	//Ticks management
	void			ResetSamplingFrequency	();
	inline void		EndCounting				()	{FinalCounter = GetRealTime(); LastPeriod = FinalCounter - InitialCounter;}
	inline HRT_Time TicksUntilNow			()	{EndCounting(); return LastPeriod;}
	inline HRT_Time TotalTicksUntilNow		()	{HRT_Time T; TicksUntilNow(); T = Accumulated + LastPeriod; return T;}
	inline void		Accumulate				()	{Accumulated += LastPeriod; Periods++;}
	inline void		AccumulateUntilNow		()	{Accumulated += TicksUntilNow(); Periods++;InitCounting	();}
	inline void		InitCounting			()	{InitialCounter = GetRealTime();FinalCounter = InitialCounter;}
	inline void		Start					()	{InitCounting();State	= HRT_ACTIVE;}
	inline void		ResetName				()	{Name.clear();}

	inline void		Reset()
	{
		AccumulatedACPI	= HRT_NOTIME;
		InitialCounter	= HRT_NOTIME;
		FinalCounter	= HRT_NOTIME;
		Accumulated		= HRT_NOTIME;
		LastPeriod		= HRT_NOTIME;
		MaxPartial		= HRT_NOTIME;
		Alarm			= HRT_NOTIME;

		Periods			= 0;
		MinPartial		= RTT_MAXTIME;
		Start	();
	}

	void UpdateMinMax();	///<Updates the statistical private attributes MaxPartial and MinPartial

	///Accessing to private attributes for read only mode
	inline HRT_Time GetAccumulated		()	{return Accumulated;}
	inline HRT_Time GetAccumulatedACPI	()	{return AccumulatedACPI;}
	inline HRT_Time GetLastPeriod		()	{return LastPeriod;}
	inline HRT_Time GetMaxPartial		()	{return MaxPartial;}
	inline HRT_Time GetMinPartial		()	{return MinPartial;}
	inline unsigned int	 GetPeriods		()	{return Periods;}

	///Next computations are multiplied by 1000 ms in a second
	inline double GetAccumulatedms	()	{return Ticks2ms(Accumulated);}
	inline double GetLastPeriodms	()	{return Ticks2ms(LastPeriod);}
	inline double GetMaxPartialms	()	{return Ticks2ms(MaxPartial);}
	inline double GetMinPartialms	()	{return Ticks2ms(MinPartial);}

	//Alarms
	inline void			SetAlarm	(HRT_Time A)	{Alarm = A; InitCounting();}
	inline void			SetAlarm	(double A)		{SetAlarm(ms2Ticks(A));}
	inline bool			IsSounding	()				{EndCounting();return Alarm < LastPeriod;}
	inline void			ResetAlarm	()				{InitCounting();}

	//Miscelanea
	inline void			SetName		(char *N)		{Name.assign(N, strlen(N));}
	inline void			SetName		(std::string N)	{Name = N;}
	inline const char	*GetName	()				{return Name.c_str();}
	inline std::string	GetNameS	()				{return Name;}
};
#endif