/**
 * RTDeskMsg.h: interface for the Basic Message class
 *
 * Copyright(C) 2013
 *
 * Prefix: RTDM_

 * @Author: Dr. Ramón Mollá Vayá
 * @Date:	03/2013
 * @Version: 2.0
 *
 * Update:
 * Date:	
 * Version: 
 * Changes:
 *
 */


#ifndef RTDM_MSG_H
#define RTDM_MSG_H

#include <RTDESKDefCom.h>

#define RTDM_DEFAULT_TYPE 0


class RTDESK_API RTDESK_CEntity;

//Envelope class. Holds inside the significant information (letter) to send to the receiver
class RTDESK_API RTDESK_CMsg
{
	public:
		unsigned int		Type;		///<Class of message.
		bool				Propietary;	///<This msg may has an owner. It cannot be stored at the MsgTimeOrderedBuffer to be reused by any other entity
		// Entities:
		///Source  (send message)
		RTDESK_CEntity		*Sender;
		///Destiny (receive message)
		RTDESK_CEntity		*Receiver;

		///Pointers to be stored at the MsgPoolManager or in the event queue by the dispatcher
		RTDESK_CMsg			*Next,		///<Next msg in the same level Hi (long, int, byte) with a subsequent time stamp 
							*Previous,	///<Previous msg in the same level Hi (long, int, byte) with a previous time stamp 
							*First,		///<First msg in the next sub level Hi (int, byte) with a subsequent time stamp 
							*Last;		///<Last msg in the next sub level Hi (int, byte) with a subsequent time stamp 

		///Absolute message time to be dispatched
		RTDESK_TIME			AbsoluteTime;

#ifdef RTDESKT_FRACTIONAL_TIME
		unsigned short int	FractionalTime;	//16 bits by the moment in order to increase the real time resolution by 16*10^-6
											//Typically if HRT Time is around 4 MHz, time resolution can be increased up to 262GHz = 4 ps
											//Using 32 bits, time resolution could be increased upt to 16 PHz (PetaHz) = 64 as (attoseconds)
#endif
	//(Des)Constructor
	inline RTDESK_CMsg() {Init();};
	~RTDESK_CMsg(){};

	// Methods
	inline bool	IsLast			()	{return NULL == Next;}
	inline bool	IsFirst			()	{return Previous->First == this;}
	inline bool	SubqueueEmpty	()	{return NULL == First;}
	inline bool	SubqueueNotEmpty()	{return NULL != First;}
	
	inline void AsignPointers (RTDESK_CMsg *A) {Next = A->Next; Previous = A->Previous; First = A->First; Last = A->Last;}

	//Queue message management
	//Insertion
	/**
	* @fn InsertSingleMsg (RTDESK_CMsg *pMsg)
	* Inserts the message as a single msg in the time ordered tree
	* @param *pMsg: A pointer to the Previous message in the list 
	*/
	inline void InsertSingleMsg (RTDESK_CMsg *pMsg) {First = NULL; Last = NULL; Next = NULL; Previous = pMsg;};

	/**
	* @fn InsertFirstMsg (RTDESK_CMsg *pMsg)
	* Inserts the first message into the "down" time ordered tree
	* Preconditions: pMsg->First = pMsg->Last = NULL
	* @param *pMsg: A pointer to the message to insert in the first position of the list 
	*/
	inline void InsertFirstMsg (RTDESK_CMsg *pMsg) 	{First = pMsg; Last = pMsg; pMsg->InsertSingleMsg(this);};

	/**
	* @fn InsertLastMsg (RTDESK_CMsg *pMsg)
	* Inserts the last message into the "down" time ordered tree
	* Preconditions: pMsg->First = pMsg->Last = NULL
	* @param *pMsg: A pointer to the message to insert in the last position of the list 
	*/
	inline void InsertLastMsg (RTDESK_CMsg *pMsg) 	{pMsg->InsertSingleMsg(Last); Last->Next = pMsg; Last = pMsg;};
	
	void InsertPrevious		(RTDESK_CMsg *pMsg);
	void InsertNext			(RTDESK_CMsg *pMsg);
	void Substitute			(RTDESK_CMsg *pMsg);
	
	//Extraction
	RTDESK_CMsg *Pop		();

	/**
	* @fn SetNULLLinks ()
	* Sets all pointers to NULL value
	*/
	inline void SetNULLLinks ()	{First = NULL; Last = NULL; Next = NULL; Previous = NULL;};

	/**
	* @fn Init ()
	* Initializes all pointers and internal values to default
	*/
	inline void Init()
	{
		Sender			= NULL;
		Receiver		= NULL;
		SetNULLLinks();
		Type			= RTDM_DEFAULT_TYPE;
		AbsoluteTime	= RTDESKT_TIME_INVALID;
		Propietary		= false;
#ifdef RTDESKT_FRACTIONAL_TIME
		FractionalTime	= 0;
#endif
	};
};
#endif
