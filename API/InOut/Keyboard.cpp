/** 
*	Class Keyboard

*	Class prefix: CK_

*	@author Ramon Molla
*	@version 2013-10
*/

#include <InOut\Keyboard.h>

/**   
   * @fn CKeyboard::CKeyboard()
   * Constructor for the keyboard
*/
CKeyboard::CKeyboard()
{
	TypeHW      = CHW_INPUT;			// Define Type Hardware In
	SubTypeHW   = CHW_IN_KEYBOARD;		// Define SubType Keyboard
	Active		= true;					// Define Active Keyboard
	ActionSize	= 0;
	Init();
}

/**   
   * @fn CKeyboard::~CKeyboard()
   * Destruct Keyboard
*/
CKeyboard::~CKeyboard()
{
	ActiveTokens.clear();
}
/**   
   * @fn CKeyboard::Init
   * Init keys
*/
void CKeyboard::Init ()
{
	///Keyboard INITIALIZATION
	ActiveTokens.clear();
	ResetActionKeys();
	HRTimerManager.CreateTimers(ActionSize);		
};

/**   
   * @fn CKeyboard::SetActionKeys(unsigned char *AK, unsigned char Size)
   * Set the translation table that associates the input lexema (pressed key) to the corresponding token
   * It determines which keys the game is sensitive to
   * @Param char AK List of Action Keys to make the keyboard sensitive to
   * @Param char Size Length of the list
*/
void CKeyboard::SetActionKeys	(unsigned char *AK, unsigned char Size)
{
	ResetActionKeys();

	ActionSize = Size;

	//The order of the key means the name of the token to be released when the key is pressed
	for (unsigned char i = 0; i< Size; i++)
		ActionKeys[AK[i]] = i;	//Copies the action keys list
		
}

/**   
	* @fn CKeyboard::SetDelay(HRT_Time *D)
	* Establishes Keys delays. This sets the minimun repeating period of a key.
    * @param char AK List of Action Keys to make the keyboard sensitive to
    * @param char Size Length of the list
    * Set the list of actions to keys
    * Check the list of sensitive keys
*/
void CKeyboard::SetDelay	(HRT_Time *D)
{
	//Every previous timer is released and new ActionSize are generated and reset to 0
	HRTimerManager.CreateTimers(ActionSize);

	for (unsigned char i = 0; i< ActionSize; i++)
	{
		HRTimerManager.Timers[i].SetAlarm (D[i]);	//Copies the action keys list
		HRTimerManager.Timers[i].ResetAlarm();
	}
}

/**   
	* @fn CKeyboard::SetDelay(float *D)
	* Establishes Keys delays. This sets the minimun repeating period of a key.
    * @param char AK List of Action Keys to make the keyboard sensitive to
    * @param char Size Length of the list
    * Set the list of actions to keys
    * Check the list of sensitive keys
*/
void CKeyboard::SetDelay	(float *D)
{
	//Every previous timer is released and new ActionSize are generated and reset to 0
	HRTimerManager.CreateTimers(ActionSize);

	for (unsigned char i = 0; i< ActionSize; i++)
		HRTimerManager.Timers[i].SetAlarm (D[i]*HRTimerManager.Timers[i].GetSamplingFreqms());	//Copies the action keys list
}

/**
   * @fn CKeyboard::KeyWait(const CK_TOKEN Token)
   * Wait for the key
   * @Param char Token to determine if it is the time for repeating again the order associated to this token
   * Avoids to set a lot of repetitive touches
   * @return true if the wait has finished and resets the alarm
*/
bool CKeyboard::KeyWait(const CK_TOKEN Token)
{
	if (HRTimerManager.Timers[Token].IsSounding())
	{
		HRTimerManager.Timers[Token].ResetAlarm();
		return true;
	}
	else
		return false;
}

/**
   * @fn CKeyboard::KeyPressed(const CK_TOKEN Token)
   * Set the actions associated to a pressed token
   * @Param char Token to include in the list of active tokens
*/
void  CKeyboard::KeyPressed	(const CK_TOKEN Token)  	///<Set a new key into the active list of key pressed
{
	 for (CK_ACTIVE_TOKENS::iterator it=ActiveTokens.begin(); it != ActiveTokens.end(); ++it)
		if(*it == Token)	return;

	ActiveTokens.push_back(Token);
}

/**
   * @fn CKeyboard::KeyReleased(const CK_TOKEN Token)
   * Set the actions associated to a released token
   * @Param char Token to exclude in the list of active tokens
*/
void CKeyboard::KeyReleased (const CK_TOKEN Token)	///<Release a new key from the active list of key pressed
{
			ActiveTokens.remove(Token);
}