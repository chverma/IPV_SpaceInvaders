/**
 * HRTimerManager.h : Class header for the High Resolution Timers Manager

 * @Prephix = HRTM_	Comes from High Resolution Timer Manager

 * @Author: Ramón Mollá
 * @version: 2.0
 * @Date:	02/2013

 * Creative Commons: Attribution-NonCommercial 3.0 Unported (CC BY-NC 3.0) 2012-10 Valencia
 *
 * Update:	
 * Date:	
 * Changes:
 */

#ifndef HRTIMERM_H
#define HRTIMERM_H

#include <vector>
#include <string>
#include <HRTimer.h>

/**
	\enum HRTM_ERRORS
	\brief Different errors that mey happen when managing HR Timers
*/
typedef enum 
{
	HRTM_NO_ERROR,
	HRTM_NO_TIMERS_CREATED,
	HRTM_MAX_ERRORS
} HRTM_ERRORS;

/**
	\class cHRTimerManager
	\brief Manages an array of High Resolution Timers.
*/
class HRT_API cHRTimerManager
{
	std::string Name;	///<String that identifies the timer. The name of the timer

public:

	std::vector<cHRTimer>	Timers;

	//Methods
	inline cHRTimerManager(void)	{DestroyTimers();};
	inline ~cHRTimerManager()		{DestroyTimers();};


	inline void CreateTimers(unsigned int T) {Timers.resize(T);};

	void SetTimersName (char *TimerNames[]);
	inline char *ErrorMsg (HRTM_ERRORS E);

	inline void DestroyTimers()				{Timers.clear();Name.clear();};

	///When an ACPI command changes (up/down) the system clock frequency, a new QueryPerformanceFrequency has to be performed 
	///by every HRT.
	void ResetSamplingFrequency(void);
	inline void ResetAllTimers(void){for (unsigned int i = 0; i < Timers.size(); i++) {Timers[i].Reset();}};

	inline void			SetName		(char *N)		{Name.assign(N, strlen(N));};
	inline void			SetName		(std::string N)	{Name = N;};
	inline const char	*GetName	()				{return Name.c_str();};
	inline std::string	GetNameS	()				{return Name;};

};
#endif
