/**
 * HRTimerStat.cpp : Class declarations for the High Resolution Timers

 * @Prephix = HRTST_	Comes from High Resolution Timer

 * @Author: Ramón Mollá
 * Version: 2.0
 * Creative Commons: Attribution-NonCommercial 3.0 Unported (CC BY-NC 3.0) 2012-10 Valencia
 *
 * Update:
 * Date:
 * Changes:
 */

#include <HRTimerStat.h>

//All previous values are taken into account although ancient values have no weight at all
//in the accouting
//#define HRTST_WHOLE_HISTORY
//Only the last few previous values are taken into account. The rest are forgotten
#define HRTST_LAST_HISTORY

cHRTimerStat::cHRTimerStat ()
{
	RecentCounts	= RecentCountsSquared = NULL;
	TotalCounts		= LastCount = 0;
	Average			= HRT_NOTIME;
	Variance		= HRT_NOTIME;
	WeightingFactor	= WFN = ITC = ISK = 1.0;
}

/**
	* @fn ResetInternalConsts ()
	* Recalculates the values of the internal statistical constants 
	* This internal method has to be called every time the weighting factor or TotalCounts change
*/
void cHRTimerStat::ResetInternalConsts()
{
#ifdef HRTST_LAST_HISTORY
	WFN = pow(WeightingFactor,TotalCounts);
	ITC = 1.0f/TotalCounts;
	if (1.0f == WFN) ISK = ITC;
	else ISK = (1.0f - WeightingFactor)/(1.0f - WFN);
#endif
}

/**
	* @fn ResetCounting ()
	* Set every counting within the counting array to zero
	* Statistical values are set to zero also
*/
void cHRTimerStat::ResetCounting()
{
	unsigned int i;

	for(i = 0; i < TotalCounts; i++)
	{
		RecentCounts[i]			= HRT_NOTIME;
		RecentCountsSquared[i]	= HRT_NOTIME;
	}
	
	LastCount = 0; 
	Average = HRT_NOTIME; Variance = HRT_NOTIME;
};

/**
	* @fn SetTotalCounts (unsigned short int C)
	* Changes the counting array length. Non destructive conversion, if possible.
	* If the new length N is lower than the previous one P, then get the most recent N values of P and store it in N
	* If the new length N is higher than the previous one P, then get all the P values of P and store it in N
	* @param unsigned short int C: The amount of counts to redimension the array of countings
*/

void	cHRTimerStat::SetTotalCounts	(unsigned int C)
{
	HRT_Time		*NewCounts, *NewCountsSquared;
	unsigned int	i, OldCount;

	#ifdef HRTST_WHOLE_HISTORY
	//There is only a single value to compute
		return NULL;
	#endif

	if (RecentCounts)
	{
		if (C > TotalCounts)
		{
			NewCounts			= new HRT_Time[C];
			NewCountsSquared	= new HRT_Time[C];
			
			ResetCounting	(NewCounts, C);
			ResetCounting	(NewCountsSquared, C);

			for(i=0,OldCount=LastCount;i<TotalCounts;i++)
			{
				OldCount++;
				if (OldCount == TotalCounts )
					OldCount = 0;

				NewCounts[i]		= RecentCounts[OldCount];
				NewCountsSquared[i] = RecentCountsSquared[OldCount];
			}

			LastCount = TotalCounts-1;

			//Finally delete RecentCounts and assign the new values
			delete RecentCounts;
			RecentCounts = NewCounts;

			//Finally delete RecentCountsSquared and assign the new values
			delete RecentCountsSquared;
			RecentCountsSquared = NewCountsSquared;
		}
		else if (TotalCounts > C)
			{
				NewCounts			= new HRT_Time[C];
				NewCountsSquared	= new HRT_Time[C];
				
				for(i=C-1,OldCount=LastCount+1;i>=0;i--)
				{
					OldCount--;
					if (OldCount < 0 )
						OldCount = TotalCounts-1;

					NewCounts[i]		= RecentCounts[OldCount];
					NewCountsSquared[i] = RecentCountsSquared[OldCount];
				}

				LastCount = C-1;

				//Finally delete RecentCounts and assign the new values
				delete RecentCounts;
				RecentCounts = NewCounts;

				//Finally delete RecentCountsSquared and assign the new values
				delete RecentCountsSquared;
				RecentCountsSquared = NewCounts;
			}
			else return; //new length is the same as the previous one. So, nothing to do :-))

	}
	else //RecentCounts == NULL
	{
		RecentCounts		= new HRT_Time[C];
		RecentCountsSquared = new HRT_Time[C];
		TotalCounts = C;
		ResetCounting();
	}
	TotalCounts = C;
	ResetInternalConsts();
}

/**
	* @fn UpdateCounting ()
	* Pull out the oldest counting value
	* Push in the newest counting value
	* Update statistical values
*/
void	cHRTimerStat::UpdateStats()
{
	HRT_Time LastPeriodSquared;

#ifdef HRTST_LAST_HISTORY
	LastCount++;
	if (LastCount == TotalCounts )
		LastCount = 0;
	//LastCount now is indexing to the oldest values in the array

	if (1>WeightingFactor)
	{
		////Take out the oldest value and update variance
		////Variance = Variance + squared(X0)/Sk - (K^n)*squared(Xn)/Sk
		LastPeriodSquared				= LastPeriod*LastPeriod;
		RecentCountsSquared[LastCount]	*= WFN;
		Variance						= Variance*WeightingFactor + (LastPeriodSquared - RecentCountsSquared[LastCount])*ISK;

		RecentCountsSquared[LastCount]	= LastPeriodSquared;
		////Variance = Variance + squared(previous Xaverage)
		Variance						+= Average*Average;

		//LastPeriod contains the newest value to incorporate to the accounting and substitute the oldest one
		//Calculate the new value for the average
		RecentCounts[LastCount]			*= WFN;
		Average							= Average*WeightingFactor + (LastPeriod - RecentCounts[LastCount])*ISK;
		RecentCounts[LastCount]			= LastPeriod;		
		Variance						-= Average*Average;
	}
	else	//1==WeightingFactor
	{
		//Take out the oldest value and update variance
		//Variance = Variance + squared(Xn+1)/n - squared(X1)/n
		LastPeriodSquared				= LastPeriod*LastPeriod;
		Variance						+= (LastPeriodSquared - RecentCountsSquared[LastCount])*ITC;

		RecentCountsSquared[LastCount]	= LastPeriodSquared;
		//Variance = Variance + squared(previous Xaverage)
		Variance						+= Average*Average;

		//LastPeriod contains the newest value to incorporate to the accounting and substitute the oldest one
		//Calculate the new value for the average
		Average							+= (LastPeriod - RecentCounts[LastCount])*ITC;
		RecentCounts[LastCount]			= LastPeriod;
		Variance						-= Average*Average;
	}
#elif HRTST_WHOLE_HISTORY
	if (1>WeightingFactor)
	{
		*RecentCountsSquared *= WeightingFactor;
		*RecentCounts		 *= WeightingFactor;
	}

	*RecentCountsSquared += LastPeriod*LastPeriod;
	*RecentCounts		 += LastPeriod;
	TotalCounts++;
#endif
}

void cHRTimerStat::SetWeightingFactor(float WF)
{
	if (1.0<WF) 
		WeightingFactor = 1.0f;
	else if (0>WF)
		WeightingFactor = 0;
	else WeightingFactor = WF; 
	
	WFN = 1.0/pow(WeightingFactor,TotalCounts);
}

HRT_Time cHRTimerStat::GetAverage()
{
#ifdef HRTST_LAST_HISTORY
	return Average;
#elif HRTST_WHOLE_HISTORY
	return RecentCounts / TotalCounts;
#endif
};

HRT_Time cHRTimerStat::GetVariance()
{
#ifdef HRTST_LAST_HISTORY
	return Variance;
#elif HRTST_WHOLE_HISTORY
	return RecentCountsSquared / TotalCounts - (RecentCounts*RecentCounts)/(TotalCounts*TotalCounts);
#endif
};

