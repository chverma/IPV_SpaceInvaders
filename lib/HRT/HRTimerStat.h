/**
 * HRTimerStat.h : Class header for the High Resolution Timers with statistical analysis

 * @Prephix = HRTST_

 * @Author: Ramón Mollá
 * @version: 2.0
 * @Date:	11/2013
 * Creative Commons: Attribution-NonCommercial 3.0 Unported (CC BY-NC 3.0) 2012-10 Valencia
 *
 * Update:
 * Date:
 * Version:
 * Changes:
 * Take into account: TotalCounts is better to be power of two since 
 */

#ifndef HRTIMERSTAT_H
#define HRTIMERSTAT_H

#include <HRTimer.h>

class HRT_API cHRTimerStat: public cHRTimer
{
	///Performance attributes
	
	HRT_Time			*RecentCounts,	///< Stores the last TotalCounts timing measures. Units: System Clock Ticks
						*RecentCountsSquared;	///< For variance computing purpouses only
	unsigned int		TotalCounts,	///< Amount of counts stored at RecentCounts. Typically powers of two: 4, 8, 16, 32 or HRTST_STAT_BUFFER_LENGTH
						LastCount;		///< Pointer to the last count
	HRT_Time			Average,		///< Summation of all counts divided by TotalCounts
						Variance;		///< Summation of the square difference between every count and the Average

	float				WeightingFactor,///< Coeficient that multiplies every value. Recent values weight more that older ones if it is lower than one and viceversa.
						WFN,			///< Weighting Factor raised to N elements in the array (TotalCounts)
						ISK,			///< Inverse of the summatory of 1/K^i  to calculate weighted average without dividing when weighting
						ITC;			///< Inverse of N samples (TotalCounts) to calculate average without dividing

	void ResetInternalConsts();			///< Recalculates the values of WFN, ISK and ITC

public:	
	
	cHRTimerStat ();
	~cHRTimerStat ()	{delete RecentCounts;};
	//Ticks management
	void			SetTotalCounts	(unsigned int C);
	void			ResetCounting	();
	inline void		ResetCounting	(HRT_Time *Counts, unsigned int Size) {for(unsigned int i = 0; i < Size; i++) Counts[i] = HRT_NOTIME;};

	void			UpdateStats		();
	
	///Accessing to private attributes for read only mode
	HRT_Time GetAverage	();
	HRT_Time GetVariance();

	void SetWeightingFactor		(float WF);
};
#endif