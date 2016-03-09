/**
 * HRTimer.cpp : Class declarations for the High Resolution Timers

 * @Prephix = HRT_	Comes from High Resolution Timer

 * @Author: Ramón Mollá
 * Version: 2.0
 * Creative Commons: Attribution-NonCommercial 3.0 Unported (CC BY-NC 3.0) 2012-10 Valencia
 *
 * Update:
 * Date:
 * Changes:
 */

#include <HRTimer.h>

///Updates the statistical private attributes MaxPartial and MinPartial
void cHRTimer::UpdateMinMax()
{
	if(LastPeriod > MaxPartial)
		MaxPartial = LastPeriod;
	else if(LastPeriod < MinPartial)
			MinPartial = LastPeriod;
}

void cHRTimer::ResetSamplingFrequency(void)
{
	if (HRT_ACTIVE == State)
	{
		EndCounting();	//Careful. State is now HRT_INACTIVE
		Accumulate();	//Accumulation supervening. Not explicitly ordered by the user, so... 
		Periods--;		//... no accounting for another period has to be performed. Do not increment the amount of times the accumulation is performed.
		InitCounting();	//A change to the previous state is performed and a new accounting with the new value
		//Reset accumulated
		Accumulated = HRT_NOTIME;
	}
	ResetSF();
}
