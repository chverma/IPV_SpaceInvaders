/* 
	Author: Vicente Broseta Toribio
	Last update: 2012-10
*/

#ifndef FPSGL_H
#define FPSGL_H

#include <RTDeskEntity.h>
#include <glut.h>
#include "UserMessage.h"

#define FPS_CYCLE 10		// Actualizar el contador de fps cada N frames

class cFpsGL : public RTDESK_CEntity{
//class cFpsGL{
public:
	unsigned int nFrames;		// Contador del n�mero de frames visualizados
	unsigned int AllnFrames;		// Contador del n�mero de frames visualizados
	float fps;					// Velocidad actual de visualizaci�n (frames por segundo)
	float initTime;				// Soporte del c�lculo de fps
	double pctRnd;
	double pctUpd;
	double pctIdl;
	double pctSuma;
	float  pctCeldPro;
	double pctRtd;
	unsigned int	fpsContTotal;	//Variable que cuenta el total de frames procesados.
	double fpsAcum;

	double timeMsg;						//Tiempo Mensaje GDESK
	
	cFpsGL(void);
	virtual ~cFpsGL();

	void calcFpsPct(void);
	void Reset(void);

	void ReceiveMessage(RTDESK_CMsg *pMsg);
};

#endif // FPSGL.H