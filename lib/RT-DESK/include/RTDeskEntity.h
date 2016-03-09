/**
 * RTDeskEntity.cpp: Basic class Entity declarations
 *
 * Copyright(C) 2013
 *
 * Prefix: RTDE_

 * @Author: Dr. Ramón Mollá Vayá
 * @Date:	02/2013
 * @Version: 2.0
 *
 * Update:	Verify that hierarchy is encapsulated
 * Date:	15/10/2012
 * Version:	2.0
 * Changes:
 */

#ifndef RTDESK_ENTITY_H
#define RTDESK_ENTITY_H

class RTDESK_CMsgDispatcher;
class RTDESK_CMsg;
class RTDESK_CEngine;

#include <RTDeskTime.h>
#include <string>

class RTDESK_CEntity
{
	RTDESK_CMsgDispatcher	*MsgDispatcher;	///< Memory Manager to whom this entity can ask for an enveloppe to send a message

public:

	inline RTDESK_CEntity	(){MsgDispatcher = NULL;}
	inline ~RTDESK_CEntity	(){}

	inline RTDESK_CMsg *GetMsgToFill(unsigned int MsgType);
	inline RTDESK_TIME	ms2Ticks	(double ms);

	void		SendMsg	(RTDESK_CMsg *pMsg, RTDESK_CEntity *pReceiver, RTDESK_TIME DeltaTime);
	inline void	SendMsg	(RTDESK_CMsg *pMsg, RTDESK_CEntity *pReceiver, double DeltaTimems){SendMsg(pMsg, pReceiver, ms2Ticks(DeltaTimems));};

	inline void	SendSelfMsg	(RTDESK_CMsg *pMsg, RTDESK_TIME DeltaTime)	{SendMsg(pMsg, this, DeltaTime);}
	inline void	SendSelfMsg	(RTDESK_CMsg *pMsg, double DeltaTimems)		{SendSelfMsg(pMsg, ms2Ticks(DeltaTimems));};

	//The programmer that makes all objects in the virtual world a derivated class form this one 
	//has to redefine what to do when receiving a msg
	virtual void	ReceiveMessage		(RTDESK_CMsg *pMsg) = 0;

	inline void		SetMsgDispatcher	(RTDESK_CMsgDispatcher *MD) {MsgDispatcher = MD;};
	inline void		SetMsgDispatcher	(RTDESK_CEngine *E);
};

#endif
