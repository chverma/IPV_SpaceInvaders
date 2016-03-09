/*	Definition of the class Time Manager

	Prefix: CT_

	@author Ramon Molla
	@version 2011-09
*/

#ifndef CT_TIME
#define CT_TIME

// Got the TimerInit & Time.TimerGetTime functions from gamedeveloper.co.uk.

#ifndef WINDOWS_LOADED
#define WINDOWS_LOADED
#include <windows.h>
#endif

//////////////////////////////////////////////////////////////////////
//
// Frames Per Second Stuff
// by: TerraX AKA Dave (Mostly)
//
// Calculates FPS
//
//////////////////////////////////////////////////////////////////////

#pragma comment (lib,"winmm.lib")	// Lib needed for WIndows multimedia stuff (timergettime etc)

class CTimeManager
{
	//Atributes
public:

	struct{			 								// Create A Structure For The Timer Information
		__int64			frequency;					// Timer Frequency
		double			resolution;					// Timer Resolution
		unsigned long	mm_timer_start;				// Multimedia Timer Start Value
		unsigned long	mm_timer_elapsed;			// Multimedia Timer Elapsed Time
		bool			performance_timer;			// Using The Performance Timer?
		__int64			performance_timer_start;	// Performance Timer Start Value
		__int64			performance_timer_elapsed;	// Performance Timer Elapsed Time
	} timer;

	// FPS METHOD
	bool	Fps2_firstpass;
	double	sec_start;
	double	sec_end;
	double	sec_frame;


	double	fps,			// Current FramesPerSecond
			fpsavr,			// Average FramesPerSecond
			ms,				// Number of milliseconds past between each frame
			ms10,			// Average Time.ms of the past 10 frames (used for more fluidity in movement)
			msTotal,		// Total number of milliseconds past since start of app
			intermede_ms,	// Constant average time used during intermission . Avoids ms speed up or down
			oldTime,		// Used to Calculate Time.ms in function CalculateMS(void)
			TimePerFrame[10];	// Records Time.ms taken to draw each of the past 10 frames
	DWORD	fpsoldtime,		// Used to store previous time, to calculate Time.ms taken to draw each frame
			fpsTotal;		// Totalled Time.fps (Use for average calculation
	int		totalFrameCount,// The number of frames that have been rendered since app started
			ClockMS,		// Time past since app started in milliseconds
			ClockS,			// seconds
			ClockM,			// minutes
			ClockHR;		// hours

	//Methods
	void Init (void);

	CTimeManager(){Init();}			//Constructor of the class

	void	Fps_Init();			// Call this on startup of application
	void	FPSReset();	// Reinitialize the records of the ten frames by the last Time.ms past since last frame
	void	Fps_Update();		// Call this each program loop to update vars
	double	TimerGetTime();
	void	TimerInit(void);

	double	GetSysTimeSeconds(void);

	void	CalculateMS(void);			// Time.ms, for time based movement instead of frame based
	void	CalculateFPS(void);		// Current FPS
	void	CalculateFPS2(void);	// Current FPS
	void	CalculateAverageFPS(void);	// Overall Average FPS since app begun running
	void	CalculateClock(void);		// Clock
};

#endif