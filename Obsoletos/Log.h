/**	Definition of the class Log

	Prefix: CL_

	@author Ramon Molla
	@version 2012-11
*/

#ifndef CL_LOG
#define CL_LOG

#ifndef FSTREAM_INITIALIZED //se borra la macro Inicializacion del log
#define FSTREAM_INITIALIZED //se define la macro Inicializacion del log
#include <fstream>
#endif

#include <string.h>
#ifndef WINDOWS_LOADED
#define WINDOWS_LOADED
#include <windows.h>
#endif

#define CL_VERSION_MAX_LENGTH 16

#define CL_MAXSTRINGS		50	//Amount of different logs that may appear simultaneously on the screen
#define CL_MAXSTRING_LENGTH	50	//Maximun length of every log string on the screen
#define CL_LOADINGBMP_FINAL_POSITION 1.3f

// MACRO : affiche une messagebox avec un message
#define BOX(text) MessageBox(NULL,text,LPCSTR("Info"),MB_OK | MB_TOPMOST);

// MACRO : affiche un message d'erreur avec le nom du fichier et la ligne
#define ERR(s) Log.DisplayMsgBoxErrorAndExit(__FILE__, __LINE__, s)

// MACRO : affiche le dernier message d'erreur sur la sortie de Debug
#define LAST_ERROR() Log.DisplayLastErrorDebug()

// MACRO : affiche un *entier* dans le debugger *une seule fois* (si différent de 0)
#define DISPLAY_1INT(var)											\
 		static int my_number = 0;									\
		if (my_number == 0)											\
		{															\
			my_number = var;										\
			char my_string[300]="";									\
			OutputDebugString (_itoa(my_number, my_string, 10));	\
		}															\

// MACRO : affiche un *entier* dans le debugger
#define DISPLAY_INT(var)											\
			{														\
				char my_string[300]="";								\
				OutputDebugString (_itoa(var, my_string, 10));		\
			}														\

// MACRO : affiche une *chaîne* dans le debugger
#define DISPLAY(var)												\
			OutputDebugString (var);								\

/**
	@class CLog

   * Set information on a openGL screen/file for debugging options

	@author Ramon Molla
	@version 2012-11
*/
class CLog
{
public:
	//Atributes
	std::ofstream	LogFile;
	std::string		FileName;
	char			Version	[CL_VERSION_MAX_LENGTH];				// Program VERSION to provide information about
	char			LogStrings[CL_MAXSTRING_LENGTH][CL_MAXSTRINGS];	// 50 logs of 50 characters available
	int				LogIndex;										// Pointer to the current available log

	float	LoadingBMPPosition;

	int		Width;			///<Window Width where to put a message
	int		Heigth;			///<Window Heigth where to put a message

	//Methods
	void Init (void);
	void Init (char *FileName);

	CLog();				///<Constructor of the class

	//Display procedures in pop up windows
	void DisplayMsgBoxErrorAndExit	(LPCTSTR File, int Line, LPCTSTR Message);
	void DisplayMsgBoxError			(LPCTSTR File, int Line, LPCTSTR Message);
	void DisplayLastErrorDebug		(void);
	void DisplayLastErrorBox		(void);

	//Information written on main window 
	void Add		(char *Text);
	void Display	();

	//Writting in a log file
	///Closes the log file before exiting
	void Close();
	///Writes in a log file a given log information
	void Write();
};
#endif
