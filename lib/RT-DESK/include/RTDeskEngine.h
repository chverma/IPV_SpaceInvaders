/**
 * RTDeskEngine.h: The core engine for RTDESK
 *
 * Copyright(C) 2013
 *
 * Prefix: RTDESK_

 * @Author:	Dr. Ramón Mollá Vayá
 * @Date:	18/12/2013
 * @Version: 2.0
 *
 * Update:
 * Date:	
 * Version: 
 * Changes:
 *
 */

#ifndef RTDESK_ENGINE_H
#define RTDESK_ENGINE_H

class RTDESK_CMsgPoolManager;
class RTDESK_CMsgDispatcher;

#include <RTDeskMsg.h>
#include <RTDeskTime.h>
class RTDESK_API RTDESK_CEngine
{
	char			Version[RTDESK_MSG_BUFFER_CONFIG];
	void			SetVersion();

public:

	RTDESK_CMsgPoolManager	*MsgPoolManager;
	RTDESK_CMsgDispatcher	*MsgDispatcher;

	#ifdef RTDESK_PROFILING
	cHRTimerManager			*HRTimerManager;
	#endif

	//Constructor
	inline RTDESK_CEngine	(){SetVersion();Reset();};
	inline ~RTDESK_CEngine	(){FreeMemory();};

	void		Reset		();
	bool		StartUp		();

	inline void ShutDown	(){FreeMemory();}
	void		Delay		(double lMilliseconds);
	void		FreeMemory	();
	
	void		SynchSim2RealTime	(RTDESK_TIME Ticks);
	void		SynchSim2RealTime	(double ms);
	
	void		SetStoreSlackTime	(RTDESK_TIME SlackTime);
	void		SetDeliverSlackTime	(RTDESK_TIME SlackTime);

	bool		SetMaxMsgTypes		(unsigned int MsgTypes);
	void		SetMsgType			(unsigned int MsgType, int MsgSize, unsigned int BunchSize);	///<Set the parameters of a given message
	inline void	SetMsgType			(unsigned int MsgType, int MsgSize){SetMsgType(MsgType, MsgSize, 1);}	///<Set the parameters of a given message
	inline void	SetMsgType			(unsigned int MsgType){SetMsgType(MsgType, sizeof(RTDESK_CMsg), 1);}	///<Set the parameters of a given message

	RTDESK_TIME	Simulate			();
	void		EndSimulation		();

	#ifdef RTDESK_PROFILING
	void InitTimeManager();
	#endif
};
#endif