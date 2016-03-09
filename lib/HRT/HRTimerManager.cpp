/**
 * HRTimerManager.cpp : Class methods declaration for the High Resolution Timers Manager

 * @Prephix = HRTM_	Comes from High Resolution Timer Manager

 * @Author: Ramón Mollá
 * Version: 2.0
 * Creative Commons: Attribution-NonCommercial 3.0 Unported (CC BY-NC 3.0) 2012-10 Valencia
 *
 * Update:	
 * Date:	

 * Changes:
 */

#include <HRTimerManager.h>

//Mind the order in which HRTM errors are defined on the HRTM_ERRORS enum type
char *HRTM_ErrorMsg[HRTM_MAX_ERRORS] =
{
	"No error",									//Corresponds to HRTM_NO_ERROR error
	"No timer has been possible to be created"	//Corresponds to HRTM_NO_TIMERS_CREATED error
};

void cHRTimerManager::ResetSamplingFrequency(void)
{
	unsigned int i, 
				 size;
	RTT_Time	 SF;

	size = Timers.size();

	if (size)
	{
		Timers[0].ResetSF();
		SF = Timers[0].GetSamplingFreq();
		for (i = 0; i < size; i++) {Timers[i].SetSF(SF);}
	}
}

void cHRTimerManager::SetTimersName (char *TimerNames[])
{
	unsigned int  i;

	for (i=0;i<Timers.size();i = i+1)
		Timers[i].SetName(TimerNames[i]);
}

inline char *ErrorMsg (HRTM_ERRORS E)	{return HRTM_ErrorMsg[E];};
