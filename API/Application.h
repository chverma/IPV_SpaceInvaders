/**	Definition of the class Application

*	Prefix: CA_

*	@author Ramon Molla
*	@version 2011-09
*/

#ifndef CA_APPLICATION
#define CA_APPLICATION

#include <UGKOS.h>
#include <Network.h>
#include <UGKLanguage.h>

#define CA_LONG_DIR_NAME 256
#define CA_DEFAULT_LANGUAGE_FILE "default.html"
#define CA_DEFAULT_LANGUAGE_LOG_FILE "LanguageLog.txt"
#define CA_DEFAULT_VERSION  "1.0.0"
#define CA_DEFAULT_TITULO  "Space Invaders"

#define CA_OPENGL
//#define CA_DIRECTX

#ifdef	UGKOS_WINDOWS
	#ifdef CA_OPENGL
		#include <GLWindow.h>
	#elif	defined(CA_DIRECTX)
		#include <DXWindow.h>
	#endif
#elif	defined(UGKOS_LINUX)
	#include <GLWindow.h>
#elif	defined(UGKOS_OSX)
	#include <GLWindow.h>
#elif	defined(UGKOS_ANDROID)
	#include <GLWindow.h>
#endif

/**
*	CApplication

*	Manages all the information relative to the general game application.
*	It is implemented as a singleton. This is not the game singleton
*/
class CApplication
{
public:

	CLanguage *Language;	///< Language that holds all the text to be showed to the user, logs,...

	//Atributes
	char				LanguageFolder	[CA_LONG_DIR_NAME];

	HINSTANCE			hInstance;	///<Application Instance
	char*				Name;		///<Application Name

	CGLWindow			Window;		///<Singleton to manage the application Window

	NET_BITRATE_TYPE	Bitrate;	///<Network BITRATE 
	bool				Stream;		///<MUSIC STREAMING

	//Methods
	void Init (void);

	CApplication(){Init(); Language = new CLanguage;}			///<Constructor of the class
	void Terminate();

	int GetOptions(bool Stream);
	int Update();

	bool LoadCurrentLanguage ();
};

#endif
