/**	Wiimote.cpp - Wrapper to use the wiiuse API

	@author Ramon Molla
	@version 2012-06
*/

//#include "Copyright.h"
#include <cstdio>
#include "WiimoteManager.h"
#include <process.h>			// Header File For _beginthread, _endthread 
#include <UGKString.h>

char CWM_Msgs[CWM_MAX_MSG][CWM_MSJ_MAX_LONG] = 
{
	"No Wiimotes Found.",
	"Failed to connect to any wiimote.\n"
};

/**
*	\fn CWiimoteManager::~CWiimoteManager()

*	destroys the data structure of the wiimotes
*/
CWiimoteManager::~CWiimoteManager()
{	
	//Disconnect the Wiimotes
	wiiuse_cleanup(Wiimotes, WiimotesAmount);
}

/**
*	\fn CWiimoteManager::HandleEvent()

*	Activates or deactivates the motion sensing of the wiimote
*/
void CWiimoteManager::HandleEvent()
{	
	if (IS_JUST_PRESSED(Wiimotes[CurrentWiimote], WIIMOTE_BUTTON_MINUS))
		wiiuse_motion_sensing(Wiimotes[CurrentWiimote], CWM_DEACTIVATE);
	
	if (IS_JUST_PRESSED(Wiimotes[CurrentWiimote], WIIMOTE_BUTTON_PLUS))
		wiiuse_motion_sensing(Wiimotes[CurrentWiimote], CWM_ACTIVATE);
}

/**
*	\fn CWiimoteManager::HandleEvent()

*	Shows the information passed to the user
*	\param[in] data Information to show out to the user
*	\param[in] len	Amount of information passed to be output
*/
void CWiimoteManager::HandleRead(byte* data, unsigned short len)
{
	int i = 0;

	printf("\n\n--- DATA READ [wiimote id %i] ---\n", Wiimotes[CurrentWiimote]->unid);
	printf("finished read of size %i\n", len);
	for (; i < len; ++i) {
		if (!(i%16))
			printf(UGKS_NEW_LINE_STRING);
		printf("%x ", data[i]);
	}
	printf("\n\n");
}

/**
*	\fn CWiimoteManager::HandleCtrlStatus()

*	Shows the the current wiimote status to the user
*/
void CWiimoteManager::HandleCtrlStatus()
{
	printf("\n\n--- CONTROLLER STATUS [wiimote id %i] ---\n", Wiimotes[CurrentWiimote]->unid);

	printf("attachment:      %i\n", Wiimotes[CurrentWiimote]->exp.type);
	printf("speaker:         %i\n", WIIUSE_USING_SPEAKER(Wiimotes[CurrentWiimote]));
	printf("ir:              %i\n", WIIUSE_USING_IR(Wiimotes[CurrentWiimote]));
	printf("leds:            %i %i %i %i\n", WIIUSE_IS_LED_SET(Wiimotes[CurrentWiimote], 1), WIIUSE_IS_LED_SET(Wiimotes[CurrentWiimote], 2), WIIUSE_IS_LED_SET(Wiimotes[CurrentWiimote], 3), WIIUSE_IS_LED_SET(Wiimotes[CurrentWiimote], 4));
	printf("battery:         %f %%\n", Wiimotes[CurrentWiimote]->battery_level);
}

/**
*	\fn CWiimoteManager::HandleDisconnect()

*	Shows the the current wiimote id to the user. Nonsense. TODO.
*/
void CWiimoteManager::HandleDisconnect()
{
	printf("\n\n--- DISWiimotesConnected [wiimote id %i] ---\n", Wiimotes[CurrentWiimote]->unid);
}

/**
*	\fn HandleWiimotesActivity()

*	Polls wiimote activity and manages it
*	\param[in] WiimoteMng	The manager that holds the wiimotes
*/
void HandleWiimotesActivity(CWiimoteManager *WiimoteMng)
{
	int i;

	while(1){
		while (wiiuse_poll(WiimoteMng->Wiimotes, WiimoteMng->WiimotesAmount)) {
			//si ha pasado algo
			for (i = 0; i < WiimoteMng->WiimotesAmount; ++i) {
				switch (WiimoteMng->Wiimotes[i]->event) {
					case WIIUSE_EVENT:
						//evento generico
						WiimoteMng->SetCurrentWiimote (i);
						WiimoteMng->HandleEvent();
						break;

					case WIIUSE_STATUS:
						//evento de estado
						WiimoteMng->SetCurrentWiimote (i);
						WiimoteMng->HandleCtrlStatus();
						break;

					case WIIUSE_DISCONNECT:
					case WIIUSE_UNEXPECTED_DISCONNECT:
						//Disconection
						WiimoteMng->SetCurrentWiimote (i);
						WiimoteMng->HandleDisconnect();
						break;

					default:
						break;
				}
			}
		}
	
	}

}

/**
*	\fn CWiimoteManager::Init()

*	Starts the use of WiimotesAmount wiimotes
*	\param[in] WiimotesAmount How many wiimotes to use 
*/
void CWiimoteManager::Init(int WiimotesAmount)
{
	int i;

	if (0 > WiimotesAmount) WiimotesAmount = 0;
	if (CWM_MAX_WIIMOTES <= WiimotesAmount) WiimotesAmount = CWM_MAX_WIIMOTES-1;

	//inicializar el array de Wiimotes
	Wiimotes =  wiiuse_init(WiimotesAmount);

	//encontrar Wiimotes
	WiimotesFound = wiiuse_find(Wiimotes, WiimotesAmount, WIIMOTE_DEFAULT_TIMEOUT);
	if (!WiimotesFound)
	{
		printf (CWM_Msgs[CWM_NO_WIIMOTES_FOUND]);
		return;
	}

	//conexion
	WiimotesConnected = wiiuse_connect(Wiimotes, WiimotesAmount);
	if (WiimotesConnected)
		printf("WiimotesConnected to %i Wiimotes (of %i WiimotesFound).\n", WiimotesConnected, WiimotesFound);
	else {
		printf(CWM_Msgs[CWM_WIIMOTES_CONNECT_FAILED]);
		return;
	}

	for (i=0;i<WiimotesAmount;i++)
	{
		//Light on its led
		wiiuse_set_leds(Wiimotes[i], WIIMOTE_LED_1);
	
		//Vibrate
		wiiuse_rumble(Wiimotes[i], CWM_ACTIVATE);
	
		#ifndef WIN32
			usleep(200000);
		#else
			Sleep(200);
		#endif

		wiiuse_rumble(Wiimotes[i], CWM_DEACTIVATE);
	}

	_beginthread((void (__cdecl *)(void *))HandleWiimotesActivity,sizeof(CWiimoteManager),this);
	return;
}

/**
*	\fn CWiimoteManager::Vibrate()

*	Let the current wiimote to perform a vibration
*/
void CWiimoteManager::Vibrate()
{

	wiiuse_rumble(Wiimotes[CurrentWiimote], CWM_ACTIVATE);
	//wiiuse_rumble(Wiimotes[1], 1);

	#ifndef WIN32
		usleep(1000);
	#else
		Sleep(10);
	#endif

	wiiuse_rumble(Wiimotes[CurrentWiimote], CWM_DEACTIVATE);

	return;
}
