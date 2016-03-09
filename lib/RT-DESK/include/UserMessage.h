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

typedef enum
{
	RTDESKMSG_BASIC_TYPE,
	RTDESKMSG_MATRIZCELDAS,	//Tipo 1
	RTDESKMSG_CELDAS,		//Tipo 2
	RTDESKMSG_TEXTOGL,		//Tipo 3
	RTDESKMSG_MAX_TYPE
} RTDESKMSG_TYPE;



// Tipo 1 ****************************************
typedef enum {

		TIPOCONTROL,
		TIPOADDCEL,
		TIPO_START_UPDCEL

} TIPO_MSG_MATCEL;

class cMsgMatCel : public RTDESK_CMsg{
public:
	TIPO_MSG_MATCEL msgSubType;
	int				integer;
};

// Tipo 2 ****************************************
typedef enum{
	MSG_UPDATE,
	MSG_DRAW,
	MSG_SETVAL1,
	MSG_SETDORMIDA,
	MSG_SETACTIVA,
	MSG_UPDCOLOR,
	MSG_SETDESPIERTA
} TipoMsgCelda;

class cMsgCel : public RTDESK_CMsg{
public:
	TipoMsgCelda msgSubType;
};

// Tipo 3 ****************************************
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






#endif  // UserMESSAGE.H
