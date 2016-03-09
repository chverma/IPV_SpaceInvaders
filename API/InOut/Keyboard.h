/**	Definition of the class keyboard

*	Prefix: CK_

*	@author Ramon Molla
*	@version 2013-10
*/

#ifndef CK_KEYBOARD //Se borra la macro del teclado
#define CK_KEYBOARD //Se define la macro del teclado
// Se define macro para librerias de windows
#ifndef WINDOWS_LOADED
#define WINDOWS_LOADED
#include <windows.h>
#endif

#include <Hardware\Hardware.h>
#include <list>

#define CK_MAX_TOKENS	256	//The maximum amount of different char. 8 bits
#define CK_NO_ACTION	0	//No action is performed. 8 bits
#define CK_TOKEN		unsigned char

#define  CK_ACTIVE_TOKENS	std::list<CK_TOKEN>

/**   
	* Keyboard class is defined for the management of keys
*/
class CKeyboard: public CHardware
{
private:
	inline void ResetActionKeys() {for (int i = 0; i < CK_MAX_TOKENS; i++) ActionKeys[i] = CK_NO_ACTION;}
	
	unsigned char	ActionSize;
	CK_TOKEN 		ActionKeys[CK_MAX_TOKENS];	///<Holds the keys to press on the keyboard in order to start up an action on the game

public:
	
	///Keys pressed down at any given time
	CK_ACTIVE_TOKENS ActiveTokens;	///<When a key is released, it is removed from this list of pressed keys. It passes to be inactive

	//Methods
	void Init ();

	CKeyboard();					///<Constructor of the class
	~CKeyboard();					///<Destructor of the class

	void SetDelay		(HRT_Time *);
	void SetDelay		(float *);
	void SetActionKeys	(unsigned char *, unsigned char);

	//inline CK_TOKEN Tokenize(char PressedKey){return ActionKeys[toupper(PressedKey)];};
	inline CK_TOKEN Tokenize(char PressedKey){return ActionKeys[PressedKey];};

	bool KeyWait	(const CK_TOKEN);	///<Returns true when time has elapsed more than int ms for a given keyboard action
	void KeyPressed	(const CK_TOKEN);	///<Set a new key into the active list of key pressed
	void KeyReleased(const CK_TOKEN);	///<Release a new key from the active list of key pressed

#ifdef UGKOS_WINDOWS
	inline void KeyPressed	(const WPARAM c){KeyPressed(Tokenize(c));};	///<Set a new key into the active list of key pressed
	inline void KeyReleased	(const WPARAM c){KeyReleased(Tokenize(c));};///<Release a new key from the active list of key pressed
#endif // UGKOS_WINDOWS
};

#endif
