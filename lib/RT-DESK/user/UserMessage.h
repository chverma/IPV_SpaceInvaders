/**
 * UserMessage.h: User Message struct definition
 *
 * Copyright(C) 2013
 *
 * Prefix: UMSG_

 * @Author:	Dr. Ramón Mollá Vayá
 * @Date:	01/02/2013
 * @Version: 2.0
 *
 * There is no need for a class since no methods are attached for managing this data container
 * Update:
 * Date:	
 * Version: 
 * Changes:
 *
 */

#ifndef RTDESK_CMsg_H
#define RTDESK_CMsg_H

#include <RTDeskMsg.h>

typedef enum
{
	RTDESKMSG_BASIC_TYPE,
	RTDESKMSG_TEXTOGL,		//Tipo 1
	RTDESKMSG_GAME,			//Tipo 2
	RTDESKMSG_NAVY,			//Tipo 3
	RTDESKMSG_SHIP,			//Tipo 4
	RTDESKMSG_SSHIP,		//Tipo 5
	RTDESKMSG_CSHIP,		//Tipo 6
	RTDESKMSG_MAX_TYPE
} RTDESKMSG_TYPE;



// Tipo 1 ****************************************
typedef struct{
	GLfloat r;
	GLfloat g;
	GLfloat b;
} rgbTexto;
typedef struct{
	GLfloat x;
	GLfloat y;
	GLfloat z;
} xyzTexto;
typedef enum{
	MSG_UPD,
	MSG_DIBUJA2D,
	MSG_DIBUJA3D,
	MSG_SETCOLOR,
	MSG_SETPOSI,
	MSG_SETSCALE
} TIPOMSG_Texto;
typedef struct{
	int x;
	int y;
	char *txt;
	int anchoV;
	int altoV;
	GLfloat *color;
} param2DTexto;

class cMsgTxtGL : public RTDESK_CMsg{
public:
	TIPOMSG_Texto tipoMsg;
	xyzTexto	xyz;
	rgbTexto	rgb;
	char		*text;
	param2DTexto	txt2D;
};

// Tipo 2 ****************************************
typedef enum {

		TIPORENDER,
		TIPOUPDATE
} TIPO_MSG_GAME;

class cMsgGame : public RTDESK_CMsg{
public:
	TIPO_MSG_GAME msgSubType;
	int				integer;
};

// Tipo 3 ****************************************
typedef enum {

		TIPOUPDSHIPS,
		TIPOUPDSSHIPS,
		TIPOUPDCSHIPS,
		TIPOTURNALL
} TIPO_MSG_NAVY;

class cMsgNavy : public RTDESK_CMsg{
public:
	TIPO_MSG_NAVY msgSubType;
	int				integer;
};

// Tipo 4 ****************************************
typedef enum {

		TIPOUPD,
		TIPOTURN
} TIPO_MSG_SHIP;

class cMsgShip : public RTDESK_CMsg{
public:
	TIPO_MSG_SHIP msgSubType;
	int				integer;
};

// Tipo 5 ****************************************
typedef enum {

		TIPOUPD_SS
} TIPO_MSG_SSHIP;

class cMsgSShip : public RTDESK_CMsg{
public:
	TIPO_MSG_SSHIP msgSubType;
	int				integer;
};

// Tipo 6 ****************************************
typedef enum {

		TIPOUPD_CS
} TIPO_MSG_CSHIP;

class cMsgCShip : public RTDESK_CMsg{
public:
	TIPO_MSG_CSHIP msgSubType;
	int				integer;
};

#endif  // UserMESSAGE.H
