/**	Definition of the class Log

	Prefix: UGKCL_

	@author Ramon Molla
	@version 2014-05
*/

#ifndef UGKCL_LOG
#define UGKCL_LOG

#ifndef FSTREAM_INITIALIZED //se borra la macro Inicializacion del log
#define FSTREAM_INITIALIZED //se define la macro Inicializacion del log
#include <fstream>
#endif

#include <string.h>
#include <UGK.h>

#define UGKCL_MAXSTRINGS		50	//Amount of different logs that may appear simultaneously on the screen
#define UGKCL_MAXSTRING_LENGTH	50	//Maximun length of every log string on the screen
#define UGKCL_LOADINGBMP_FINAL_POSITION 1.3f

// Shows a messagebox window with a message on the screen
#ifdef UGKOS_WINDOWS
#ifndef WINDOWS_LOADED
	#define WINDOWS_LOADED
	#include <windows.h>
	#endif
	#define BOX(text) MessageBox(NULL,text,LPCSTR("Info"),MB_OK | MB_TOPMOST);
#elif	defined(UGKOS_LINUX)
#elif	defined(UGKOS_OSX)
#elif	defined(UGKOS_ANDROID)
#endif

// MACRO : Show an error message with the file name and the erroneous line
#define ERR(s) Log.DisplayMsgBoxErrorAndExit(__FILE__, __LINE__, s)

// MACRO : Show the last error message into the debug output
#define LAST_ERROR() Log.DisplayLastErrorDebug()

// MACRO : Show an integer output string into the debugger once if it si not zero
#ifdef UGKOS_WINDOWS
	#define DISPLAY_1INT(var)											\
 			static int my_number = 0;									\
			if (my_number == 0)											\
			{															\
				my_number = var;										\
				char my_string[300]="";									\
				OutputDebugString (_itoa(my_number, my_string, 10));	\
			}
#elif	defined(UGKOS_LINUX)
#elif	defined(UGKOS_OSX)
#elif	defined(UGKOS_ANDROID)
#endif

// MACRO : Show an integer output into the debugger
#ifdef UGKOS_WINDOWS
	#define DISPLAY_INT(var)											\
				{														\
					char my_string[300]="";								\
					OutputDebugString (_itoa(var, my_string, 10));		\
				}
#elif	defined(UGKOS_LINUX)
#elif	defined(UGKOS_OSX)
#elif	defined(UGKOS_ANDROID)
#endif

// MACRO : Show an output string into the debugger
#ifdef UGKOS_WINDOWS
	#define DISPLAY(var) OutputDebugString (var);
	#pragma warning(disable: 4251)
#elif	defined(UGKOS_LINUX)
#elif	defined(UGKOS_OSX)
#elif	defined(UGKOS_ANDROID)
#endif

namespace UGK
{
	/**
		@class CLog

	   * Set information on a openGL screen/file for debugging options

		@author Ramon Molla
		@version 2012-11
	*/
	class UGK_API CLog
	{
	private:
		std::ofstream	LogFile;
		std::string		FileName;
	public:
		//Atributes
		char			*Version;												///< Program VERSION to provide information about
		char			LogStrings[UGKCL_MAXSTRING_LENGTH][UGKCL_MAXSTRINGS];	///< 50 logs of 50 characters available
		int				LogIndex;												///< Pointer to the current available log

		float			LoadingBMPPosition;

		//Methods
		void Init (void);
		void Init (char *FileName);

		CLog();				///<Constructor of the class

		//Display procedures in pop up windows
		#ifdef UGKOS_WINDOWS
			void DisplayMsgBoxErrorAndExit	(LPCTSTR File, int Line, LPCTSTR Message);
			void DisplayMsgBoxError			(LPCTSTR File, int Line, LPCTSTR Message);
		#elif	defined(UGKOS_LINUX)
		#elif	defined(UGKOS_OSX)
		#elif	defined(UGKOS_ANDROID)
		#endif

		void DisplayLastErrorDebug		(void);
		void DisplayLastErrorBox		(void);

		//Information written on main window 
		void Add		(const char *Text);

		//Writting in a log file
		///Closes the log file before exiting
		void Close();
		///Writes in a log file a given log information
		void Write();
	};
}	//namespace UGK
#endif
