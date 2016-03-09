/* 
	Author: Vicente Broseta Toribio
	Last update: 2012-10
*/


#ifndef TEXTOGL_H
#define TEXTOGL_H

#include <RTDeskEntity.h>
#include <glut.h>
#include<SIMessage.h>

//typedef struct{
//	GLfloat r;
//	GLfloat g;
//	GLfloat b;
//} rgbTexto;
//typedef struct{
//	GLfloat x;
//	GLfloat y;
//	GLfloat z;
//} xyzTexto;
//typedef enum{
//	MSG_UPD,
//	MSG_DIBUJA2D,
//  MSG_DIBUJA3D,
//	MSG_SETCOLOR,
//	MSG_SETPOSI,
//	MSG_SETSCALE
//} TIPOMSG_Texto;

class cTextoGL : public RTDESK_CEntity{
public:

	rgbTexto color;
	xyzTexto posicion;
	xyzTexto escalado;


	float timeGdeskMsg;						//Tiempo Mensaje GDESK
	cMsgTxtGL *msg;
	TIPOMSG_Texto tipoMsg;

	cTextoGL(void);
	~cTextoGL();

	void Draw3D(char *txt);
	void Draw3D_RTdesk(char *txt);
	void setColor(GLfloat red, GLfloat green, GLfloat blue);
	void setPosicion(GLfloat x, GLfloat y, GLfloat z);
	void setEscalado(GLfloat x, GLfloat y, GLfloat z);
	void text2D_draw(unsigned int x, unsigned int y, char *text, int anchoVentana, int altoVentana, GLfloat *color);

private:
	void ReceiveMessage(RTDESK_CMsg *pMsg);
};

#endif // TEXTOGL.H