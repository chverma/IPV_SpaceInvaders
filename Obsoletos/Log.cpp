/**	Declaration of the class Log

	Prefix: CL_

	@author Ramon Molla
	@version 2011-11
*/

#include <stdlib.h>
#include "Log.h"
#include <gl\glError.h>
#include "FontsManager.h"


/**   
   * @fn CLog
   * Constructor to the Log
   * By default the file name starts empty    
   * And the function is called Home
*/
CLog::CLog()
{
	Width		= 640;
	Heigth		= 480;
	FileName	= "";
	Init();
}

/**   
   * @fn CLog::Init
   * Start with the Log File
   * @param char File the name of the file
   * @return This function opens the log file otherwise it returns 0
*/
void CLog::Init(char *File)
{
	if (0 == FileName.length()) 
		FileName.resize(strlen(File)+1);	//One more for the end character '\0'
	FileName =File;

	LogFile.open(File);
	if (!LogFile)	//NAME the file
		exit(0);
	Init();
}

/**   
   * @fn CLog::Init
   * Start the Log
   * This function starts the progress bar, the image of the game loaded disabled
*/
void CLog::Init(){LogIndex	= 0;}

/**   
   * @fn CLog::DisplayMsgBoxError
   * @param LPCTSTR File name of the file
   * @param int Line represents the line number where the error occurred
   * @param LPCTSTR Message represents the message to be displayed
   * This function displays an error in a text box
*/
void CLog::DisplayMsgBoxError(LPCTSTR File, int Line, LPCTSTR Message)
{
	TCHAR ErrorBuf[512];

	// Create the Error string.
	wsprintf(ErrorBuf, LPCSTR("Sorry, an error has occurred...\n\n%s\nFile %s, line %d, version %s\n\nPlease check http://ramon.blogs.upv.es to grab the last release.\nIf the error persists email Ramon Molla at rmolla@dsic.upv.es"), 
		Message, File, Line, Version);

	// Dump the error to the debugger, if present.
	OutputDebugString(ErrorBuf);

	// Display the error using MessageBox.
	if (lstrlen(Message))
		MessageBox(NULL, ErrorBuf, Message, MB_OK | MB_TOPMOST);
	
	exit(-1);
}

/**   
   * @fn CLog::DisplayMsgBoxErrorAndExit
   * @param LPCTSTR File name of the file
   * @param int Line represents the line number where the error occurred
   * @param LPCTSTR Message represents the message to display
   * This function calls the function to display the error DisplayMsgBoxError
*/
void CLog::DisplayMsgBoxErrorAndExit(LPCTSTR File, int Line, LPCTSTR Message)
{
	DisplayMsgBoxError(File, Line, Message);
	exit(-1);
}

/**   
   * @fn CLog::DisplayLastErrorDebug
   * Show last bug in the compiler
   * You configure the format of the message box to the errors generated FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS 
   * This function shows the format of the error on the screen
*/
void CLog::DisplayLastErrorDebug(void)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf, 0, NULL);
	OutputDebugString((LPCTSTR)lpMsgBuf); //Envia lpMsgBuf al depurado por pantalla
	LocalFree(lpMsgBuf); //libera lpMsgBuf de memoria bloqueado
}
/**   
   * @fn CLog::DisplayLastErrorBox
   * Show last error on the box
   * You set the format to display the message box to the errors generated FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS 
   * This function shows in a box the configuration of the message error
*/
void CLog::DisplayLastErrorBox(void)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf, 0, NULL);
	BOX((LPCTSTR)lpMsgBuf);
	LocalFree(lpMsgBuf);
}
/**   
   * @fn CLog::Add
   * Add to the Log
   * @param char Text: represents the text to copy to Log
   * Copy the text to the Log and increments its index
   * After updates the Log
*/
void CLog::Add (char *Text)
{
	strcpy(LogStrings[LogIndex], Text);		// Copies the text onto the current Log Index

	LogIndex++;								// The index is incremented in one unit
	if (CL_MAXSTRINGS <= LogIndex) LogIndex = CL_MAXSTRINGS -1;
	Display();								// Renders the just updated log
}

/**   
   * @fn CLog::Display
   * Show the Log
   * Started the size of the font
   * After will be drawing the Logs, depending of the screen resolution 
   * One time finish, shows the image "Loading..." with a progress bar
*/
void CLog::Display ()
{
//	Application.Window.CreateRenderingContext();

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Makes the buffers dark
	CFontsManager FontsManager;

	// Showing all the log strings
	FontsManager.SetFontSize(8);												// taille du texte des logs
	if (Width >= 800)									// logs affichés si résolution >= 800x600
		for(int i=0; i<LogIndex; i++)							// pour toutes les entrées de log
		{
			FontsManager.SetFontSize ();
			if (Width >= 1024) 

					FontsManager.DefaultFontColor(); 
			else	FontsManager.SetFontColor(0.8f);
			//fontDrawString (10, Heigth -30 -(FontsManager.GetFontSize()+2)*i, "%s",  LogStrings[i]);
			FontsManager.DrawFont(10, Heigth -30 -(FontsManager.GetFontSize()+2)*i, "%s",  LogStrings[i]);

/*					fontColor (1.0f, 1.0f, 1.0f); 
			else	fontColor (0.8f, 0.8f, 0.8f);
			fontDrawString (10, Heigth -30 -(FontsManager.GetFontSize+2)*i, "%s",  LogStrings[i]);
			*/
		}

	// Showing the image "loading..."
	LoadingBMPPosition = CL_LOADINGBMP_FINAL_POSITION;
}

/**   
   * @fn CLog::Close
   * Closes the log file before exiting
*/
void CLog::Close()
{
}

/**   
   * @fn CLog::Write()
   * Writes in a log file a given log information
*/

void CLog::Write()
{
}
