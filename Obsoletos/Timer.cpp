/** 
//
// Time declarations
//
	@author Ramon Molla
	@version 2011-09-11
*/

#include <Timer.h>

CTimeManager::CTimeManager() {Init();}

void CTimeManager::Init()
{
	fpsoldtime		= 0;		// Used to store previous time, to calculate ms taken to draw each frame
	totalFrameCount	= 0;		// The number of frames that have been rendered since app started
	fpsTotal		= 0;		// Totalled fps (Use for average calculation
	ClockMS = ClockS = ClockM = ClockHR = 0;	// Variables used to calculate clock

	// FPS METHODE N°2
	Fps2_firstpass	= true;
	sec_start		= 0;
	sec_end			= 0;
	sec_frame		= 0;
}

/// TimerInit //////////////////////////////////////////////////////////////////////////////////////
// Initialization of timer.
void CTimeManager::TimerInit(void)
{
	// Clear Our Timer Structure
	memset(&timer, 0, sizeof(timer));

	// Performance counter (if available)
	if (QueryPerformanceFrequency((LARGE_INTEGER *) &timer.frequency))
	{
		// Performance Counter Is Available, Use It Instead Of The Multimedia Timer
		// Get The Current Time And Store It In performance_timer_start
		QueryPerformanceCounter((LARGE_INTEGER *) &timer.performance_timer_start);
		timer.performance_timer         = TRUE;             // Set Performance Timer To TRUE
		// Calculate The Timer Resolution Using The Timer Frequency
		timer.resolution                = (double) (((double)1.0f)/((double)timer.frequency));
		// Set The Elapsed Time To The Current Time
		timer.performance_timer_elapsed = timer.performance_timer_start;
	}

	// Standard counter
	else
	{    
		timer.performance_timer = FALSE;                    // Set Performance Timer To FALSE
		timer.mm_timer_start    = timeGetTime();            // Use timeGetTime() To Get Current Time
		timer.resolution        = 1.0f/1000.0f;             // Set Our Timer Resolution To .001f
		timer.frequency         = 1000;                     // Set Our Timer Frequency To 1000
		timer.mm_timer_elapsed  = timer.mm_timer_start;     // Set The Elapsed Time To The Current Time
	}
}

// TimerGetTime //////////////////////////////////////////////////////////////////////////////////////
// Gets the elapsed time in milliseconds since we started the timer.
double CTimeManager::TimerGetTime()
{
	__int64 time;

	if (timer.performance_timer)							// Are We Using The Performance Timer ?
	{
		QueryPerformanceCounter((LARGE_INTEGER *) &time);	// Grab The Current Performance Time

		// Return The Current Time Minus The Start Time Multiplied By The Resolution And 1000 (To Get MS)
		return ( (double) ( time - timer.performance_timer_start) * timer.resolution)*1000.0f;
	}
	else
		// Return The Current Time Minus The Start Time Multiplied By The Resolution And 1000 (To Get MS)
		return( (double) ( timeGetTime() - timer.mm_timer_start) * timer.resolution)*1000.0f;
}



// Fps_Init ///////////////////////////////////////////////////////////////////////////////////////
// Initalizes the fps variables (Must be called once on startup)
void CTimeManager::Fps_Init()
{    
	TimerInit();
	fpsoldtime=(long)TimerGetTime();
	msTotal=0;
}



// Time.Time.Fps_Update ///////////////////////////////////////////////////////////////////////////////////
// Updates the variables (Must be called each program loop).
void CTimeManager::Fps_Update()
{
	CalculateMS();
	CalculateFPS();
	CalculateAverageFPS();
	CalculateClock();
}

// Avoids the delay taken by the game loading. That does not affects the accounting
void CTimeManager::FPSReset()
{
	ms = 1;								// make FPS believe that the last frame had taken 1 ms
	oldTime = TimerGetTime() -1;		// make FPS believe that the last time recorded was 1 ms backwards
	int loop;
	for (loop=0;loop<10;loop++)			// make FPS believe that the ten last records took 1 ms each
	{
		TimePerFrame[loop] = ms;
	}
	ms10 = 1;							// same thing for ms10
}

// Stuff U don't have to worry about :) /////////////////////////////////////////////////////////
void CTimeManager::CalculateMS(void)					// Calculates ms & msTotal
{
	// Calculate ms past per frame
	ms=(double)TimerGetTime()-oldTime;
	oldTime=(double)TimerGetTime();
	if (ms>100) ms = 100;					// on met une limite pour la pondération des déplacements
	msTotal+=ms;						// (absorbe les écarts ponctuels de vitesse trop importants)
}

void CTimeManager::CalculateFPS(void)					// Calculates fps (current)
{
	int loop;							// Used for FOR loop
	for (loop=0;loop<9;loop++)			// Update records of last ten frame times
	{
		TimePerFrame[loop]=TimePerFrame[loop+1];
	}
	TimePerFrame[9]=ms;					// Store most recent ms per frame
	fps=0;								// Reset fps variable
	for (loop=0;loop<10;loop++)			// Total past 10 records
	{
		fps+=TimePerFrame[loop];
	}
	fps				= fps/10.0f;				// Find mean average ms per frame of last ten frames
	ms10			= fps;						// Average ms of the past 10 frames
	intermede_ms	= fps;
	fps				= 1000.0f/fps;				// Finally divide by 1 second to get final result
}

void CTimeManager::CalculateAverageFPS(void)
{
	totalFrameCount++;					// Increase the total number of frames rendered
	fpsTotal+=(DWORD)fps;				// Calculate total fps past since app started
	//ACHTUNG: Is next accounting well done?. Recode
	fpsavr=(double)(fpsTotal/(double)totalFrameCount);		// Calculate Overall average frame rate
}

void CTimeManager::CalculateClock(void)
{
	ClockMS+=(int)ms;
	if (ClockMS>1000)					// If more than a second has passed
	{
		ClockMS-=1000;
		ClockS++;
	}
	if (ClockS>59)						// If more than a minute has passed
	{
		ClockS-=60;
		ClockM++;
	}
	if (ClockM>59)						// If more than an Hour has passed
	{
		ClockM-=60;
		ClockHR++;
	}
}



//////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------
// Time.Time.GetSysTimeSeconds (FPS counter method n°2)
// 
// Returns the current system time, in seconds,
//  based on the high-resolution performance timer
//  (if available) or the system millisecond timer
//  (if not).
//-----------------------------------------------------
double CTimeManager::GetSysTimeSeconds( void )
{
	static double  queryPerfSecsPerCount = 0.0;
	LARGE_INTEGER  queryPerfCount;
	double         seconds;
	BOOL           success;

	/// Check if the perf timer needs initializing
	if( queryPerfSecsPerCount == 0.0 )
	{
		LARGE_INTEGER queryPerfCountsPerSec;

		/// Get the system's perf-ticks-per-second ratio
		success = QueryPerformanceFrequency(
			&queryPerfCountsPerSec );
		if( success && queryPerfCountsPerSec.QuadPart )
		{
			/// Calculate the seconds-per-tick ratio (inverse)
			queryPerfSecsPerCount = (double) 1.0 /
				(double) queryPerfCountsPerSec.QuadPart;
		}
		else
		{
			/// Initialization failed; use GetTickCount()
			queryPerfSecsPerCount = -1.0; // no perf timer
		}
	}

	/// Use GetTickCount if perf counter is unavailable
	if( queryPerfSecsPerCount == -1.0 )
	{
		DWORD milliseconds = GetTickCount();
		seconds = 0.001 * (double) milliseconds;
	}
	else
	{
		/// Calculate precise seconds using perf timer
		QueryPerformanceCounter( &queryPerfCount );
		seconds = queryPerfSecsPerCount *
			(double) queryPerfCount.QuadPart;
	}

	return seconds;
}
