/**	Declaration of the class Application

	Prefix: CA_

	@author Ramon Molla
	@version 2014-07
*/

#include "Application.h"
#include <TextResources.h>
#include <UGKString.h>

/**   
   @fn void CApplication::Init (void)
   Initializes the application
 */

void CApplication::Init (void)
{
	#ifdef	UGKOS_WINDOWS
		Window.Name			= NULL;
		Name				= NULL;
		Window.hInstance	= hInstance;

		Bitrate				= NET_BR_DSL;	// MUSIC BITRATE 
	#elif	UGKOS_LINUX
	#elif	UGKOS_OSX
	#elif	UGKOS_ANDROID
	#endif
}

/**   
   @fn int CApplication::Update ()
   Performs the main loop application
*/
int CApplication::Update ()
{
	#ifdef	UGKOS_WINDOWS
	
	MSG msg;

	//Check For Window Messages
	while (PeekMessage(&msg, Window.hWnd, 0, 0, PM_REMOVE))
		// Check For WM_QUIT Message
		if (msg.message == WM_QUIT)						// Is The Message A WM_QUIT Message?
			return 0;	// Otherwise (If Message Is WM_QUIT)
					// Terminate The Message Pump
		else DispatchMessage(&msg);						// If Not, Dispatch The Message
														// to WindowProc()
	// when there Are No Messages
	if (FALSE == Window.Visible)					// If Window Is Not Visible
		WaitMessage();								// Application Is Minimized Wait For A Message (paused)
	return 1;
	#elif	UGKOS_LINUX
	#elif	UGKOS_OSX
	#elif	UGKOS_ANDROID
	#endif
}

/**   
   @fn void CApplication::Terminate()
   TERMINATE APPLICATION
*/
void CApplication::Terminate()					// Terminate The Application
{
	#ifdef	UGKOS_WINDOWS
		PostMessage(Window.hWnd, WM_QUIT, 0, 0);	// Send A WM_QUIT Message
	#elif	UGKOS_LINUX
	#elif	UGKOS_OSX
	#elif	UGKOS_ANDROID
	#endif
}

/**   
   @fn void CApplication::Init (void)
   Assigns a game version to the Game in order to let the game to open a correct/understandable version of the initialization or game files
   @param char V Version to be set to the Game
*/
int CApplication::GetOptions(bool Stream)
{
	
#ifdef	UGKOS_WINDOWS
	HKEY key;
	DWORD disposition;
	DWORD type, size;

	// check key
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, Name, 0, KEY_READ, &key) == ERROR_SUCCESS)
	{	
		
		size = sizeof(bool);
		RegQueryValueEx(key, "Stream",	NULL, &type, (LPBYTE)&Stream,	&size);

		// read int values
		type = REG_DWORD;
		size = sizeof(DWORD);
		RegQueryValueEx(key, "Bitrate",	NULL, &type, (LPBYTE)&Bitrate,	&size);

		return 1;
	}
	else
	{
		// create key
		RegCreateKeyEx(HKEY_LOCAL_MACHINE, Name, 0, UGKS_EMPTY_STRING, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &key, &disposition);
		return 0;
	}
	RegCloseKey(key);

	#elif	UGKOS_LINUX
	#elif	UGKOS_OSX
	#elif	UGKOS_ANDROID
	#endif
}

/**   
	* @fn bool CApplication::LoadCurrentLanguage ()
	* Construct the default file languages (log and lang)
	* Assign files and load language
 */
bool CApplication::LoadCurrentLanguage ()
{
	char languageDefaultFile	[CA_LONG_DIR_NAME],
		 languageLog			[CA_LONG_DIR_NAME];

	//Construct the default file languages (log and lang)
	strcpy(languageDefaultFile, LanguageFolder);
	strcpy(languageLog, LanguageFolder);	

	strcat(languageDefaultFile, CA_DEFAULT_LANGUAGE_FILE);	
	strcat(languageLog, CA_DEFAULT_LANGUAGE_LOG_FILE );

	Language->SetSize(TR_MAX_TR);
	//Assign files and load language
	Language->SetFile(languageDefaultFile);
	return Language->Load(languageLog, CA_DEFAULT_VERSION, CA_DEFAULT_TITULO);
};
