/**
 * RTDeskMessage.h: Message struct definition
 *
 * Copyright(C) 2012
 *
 * Prefix: RTDESKMSG_

 * @Author:	Dr. Inmaculada García García and Dr. Ramón Mollá Vayá
 * @Date:	01/04/2012
 * @Version: 2.0
 *
 * Update:
 * Date:	
 * Version: 
 * Changes:
 *
 */

#ifndef RTDESK_MESSAGE_H
#define RTDESK_MESSAGE_H

#include <RTDeskMsgEnvelope.h>

typedef enum
{
	RTDESKMSG_NO_PRIORITY,
	RTDESKMSG_LOW_PRIORITY,
	RTDESKMSG_MED_PRIORITY,
	RTDESKMSG_MAX_PRIORITY
} RTDESKMSG_PRIORITY;

typedef enum
{
	RTDESKMSG_NO_TYPE,			//for managing purpouses only
	//New types of messages included for this demo only
	TEST_INIT_SERVICE,			//The mail server gets ready to serve mail requests
	TEST_CLOSE_SERVICE,			//The mail server drops down the mail service
	//Init and close mail sessions
	TEST_INIT_SESION,			//Mail client initializes a sesion on the mail server
	TEST_CLOSE_SESION,			//Mail client closes a sesion on the mail server
	//Ask for login and password to validate mail session
	TEST_SEND_LOGIN_PASSWORD,
	//Login and password sent
	TEST_LOGIN_PASSWORD_SENT,
	TEST_SESSION_ID,
	TEST_LOGOUT,				//Mail client logs out the mail server
	//Send content
	TEST_SEND_CONTENT,
	TEST_CONTENT_SENT,
	//Managing a single e-mail
	TEST_DELETE_CONTENT,
	TEST_ANSWER_EMAIL,
	RTDESKMSG_MAX_TYPE			//for managing purpouses only
} RTDESKMSG_TYPE;

/// This struct is a wrapper around an envelope in order to support user defined extra information
/// Here there is an example of what it may be done

struct RTDESK_MESSAGE : public RTDESK_MSG_ENVELOPE 
{
	// Variables
	public:
		RTDESKMSG_PRIORITY	Priority;
		RTDESKMSG_TYPE		Type;
	private:
	protected:

	// Methods
	public:
	private:
	protected:
	
};

#endif  // RTDESKMESSAGE.H
