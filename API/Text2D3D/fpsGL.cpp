#include <cstdio>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>

#include "fpsGL.h"
#include <HRTimerManager.h>

#include "Global.h"

extern CSIGame	Game;
#ifdef DEF_EXP_CSV_FILE
	extern bool initReg;
#endif

cFpsGL::cFpsGL(void){
	nFrames=0;
	fps=0.0;
	initTime=0.0;
	pctCeldPro=100.0;
	fpsContTotal=0;
	fpsAcum=0.0;
	AllnFrames=0;
}
cFpsGL::~cFpsGL(){
}
// Función para calcular y mostrar por pantalla el conteo de frames por segundo
void cFpsGL::calcFpsPct(void) 
{
//*********************************Calculo FPS***********************************
	nFrames++;
	if (nFrames==FPS_CYCLE)
	{
		Game.TimerManager.Timers[TmrApp].EndCounting();							//Solo EndCounting sin iniciar de nuevo tiene como referencia la toma de tiempo inicial
		Game.TimerManager.Timers[TmrFPS].EndCounting();

		float tIni = (float)Game.TimerManager.Timers[TmrApp].GetLastPeriodms();	//Tiempo desde el principio de la aplicación
		float tLast= (float)Game.TimerManager.Timers[TmrFPS].GetLastPeriodms();	//Tiempo desde la ultima vez que se realizaron los calculos

		if (tIni==initTime) return;
		fps=(float)(nFrames*1000.0)/(tIni-initTime);
		initTime=tIni;
		AllnFrames += nFrames;
		nFrames=0;

		#ifdef DEF_EXP_CSV_FILE
		if(initReg){
			fpsAcum += fps;
			fpsContTotal++;
		}
		#endif

//*********************************Calculo Tiempos***********************************

		double msRender;
		double msUpdate;
		double msIdle;
		double msRtdskMM;

		EndAccCounting(TmrRnd);

		msRender = Game.TimerManager.Timers[TmrRnd].GetAccumulatedms();
		msUpdate = Game.TimerManager.Timers[TmrUpd].GetAccumulatedms();
		msIdle = Game.TimerManager.Timers[TmrIdl].GetAccumulatedms();
		
		pctIdl = (msIdle*100.0)/tLast;
		pctRnd = (msRender*100.0)/tLast;
		pctUpd = (msUpdate*100.0)/tLast;

		if(DiscreteSimulation){
			msRtdskMM = Game.TimerManager.Timers[TmrRTDSKMM].GetAccumulatedms();
			pctRtd = (msRtdskMM*100.0)/tLast;
			pctSuma = pctIdl+pctRnd+pctUpd+pctRtd;
			Game.TimerManager.Timers[TmrRTDSKMM].Reset();
		}else 
			pctSuma = pctIdl+pctRnd+pctUpd;

		//******************************Restart Timers******************************
		Game.TimerManager.Timers[TmrUpd].Reset();
		Game.TimerManager.Timers[TmrRnd].Reset();
		Game.TimerManager.Timers[TmrIdl].Reset();
		Game.TimerManager.Timers[TmrFPS].Reset();
		Game.TimerManager.Timers[TmrRnd].InitCounting();
		Game.TimerManager.Timers[TmrFPS].InitCounting();
	}
	
}

void cFpsGL::Reset(void){
	nFrames=0;
	fps=0.0;
	initTime=0.0;
	pctCeldPro=100.0;
	fpsContTotal=0;
	fpsAcum=0.0;
	AllnFrames=0;
}

void cFpsGL::ReceiveMessage(RTDESK_CMsg *pMsg){
	
	//calcula_ModoDisc();
}