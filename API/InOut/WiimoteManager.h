/**	Wiimote.cpp - Wrapper to use the wiiuse API

*	Prefix: CWM_
*	@author Ramon Molla
*	@version 2012-06
*
*/
//#include "Copyright.h"

#ifndef CWM_WIIMOTE
#define CWM_WIIMOTE

#include <wiiuse.h>
#include <Hardware\Hardware.h>

#define CWM_NO_WIIMOTE		-1
#define CWM_MAX_WIIMOTES	 4

#define CWM_ACTIVATE	 1
#define CWM_DEACTIVATE	 0

#define CWM_MSJ_MAX_LONG 128

typedef enum {
	CWM_NO_WIIMOTES_FOUND,
	CWM_WIIMOTES_CONNECT_FAILED,
	CWM_MAX_MSG
} CWM_WiimoteMsg;


class CWiimoteManager: public CHardware
{
private:

	int			WiimotesFound, 
				WiimotesConnected;
	int			CurrentWiimote;

public:

	wiimote**	Wiimotes;
	int			WiimotesAmount;

	void Init				(int WiimotesAmount);
	CWiimoteManager			(){CurrentWiimote = CWM_NO_WIIMOTE; WiimotesAmount = 0; Wiimotes = NULL;}		///<Constructor of the class
	~CWiimoteManager		();		///<Destructor of the class

	///Activates the wiimote to work with from now on
	inline void SetCurrentWiimote (int wm) {if (wm < WiimotesAmount && wm >= 0) CurrentWiimote = wm;}

	//HW handles for managing input events
	void HandleEvent		();
	void HandleRead			(byte* data, unsigned short len);
	void HandleCtrlStatus	();
	void HandleDisconnect	();

	void Vibrate			();
};
#endif