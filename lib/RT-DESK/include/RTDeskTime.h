/**
 * RTDeskTime.h: Description of the global definition of how to represent Time y RT-DESK
 *
 * Copyright(C) 2013
 *
 * Prefix: RTDESKT_

 * @Author:	Dr. Ramón Mollá Vayá
 * @Date:	02/2013
 * @Version: 2.0
 *
 * Update:
 * Date:	
 * Version: 
 * Changes:
 *
 */

#ifndef RTDESK_TIME_H
#define RTDESK_TIME_H

#include <rttimer.h>

// Simulator Time
// ********************
#define RTDESKT_HRT

/// RTDESK works in real time (on line) or in simulation time (off line)
#define RTDESKT_REAL_TIME			///<When defined, RTDESK works in real time
//#define RTDESKT_FRACTIONAL_TIME		///<When defined, RTDESK works using fractional part of time using fixed point airthmetic

#ifdef RTDESKT_REAL_TIME
	#ifdef RTDESKT_HRT

		#define RTDESK_TIME RTT_Time

		#define RTDESKT_TIME_INVALID	-1LL
		#define RTDESKT_NOTIME			 0LL
		#define RTDESKT_INMEDIATELY		 RTDESKT_NOTIME	
		#define RTDESKT_FASTEST			 1LL	//Sent in just one single time click
		#define	RTDESKT_INFINITE_TIME	 LLONG_MAX

	#else
		#include <time.h>
		#include <cfloat>

		#define RTDESKT_TIME_INVALID	-1.0
		#define RTDESKT_NOTIME			 0.0
		#define RTDESKT_INMEDIATELY		 RTDESKT_NOTIME		//When delivering events just right now. Maximun priority and Store Slack Time is allowed
		#define RTDESKT_FASTEST			 DBL_MIN	//Sent in just one single time click
		#define	RTDESKT_INFINITE_TIME	 DBL_MAX 

		#define RTDESK_TIME		double	// In miliseconds

		#ifdef RTDESKT_REAL_TIME
			inline	void RTDESKT_GetRealTime(RTDESK_TIME *RT)	{*RT = clock();}	// In miliseconds
		#endif
	#endif
#else 
		#define RTDESKT_TIME_INVALID	-1.0
		#define RTDESKT_NOTIME			 0.0
		#define RTDESKT_INMEDIATELY		 RTDESKT_NOTIME	//When delivering events just right now. Maximun priority and Store Slack Time is allowed
		#define RTDESKT_FASTEST			 DBL_MIN	//Sent in just one single time click
		#define	RTDESKT_INFINITE_TIME	 DBL_MAX 

		#define RTDESK_TIME		double	// In miliseconds
#endif

#endif
