/**	Definition of the class Direct X Window

	Prefix: CDXW_

	@author Ramon Molla
	@version 2014-09
*/

#ifndef CDXW_GLWINDOW 
#define CDXW_GLWINDOW

#include <Window/Window.h>

/**   
	* It defines the class Window (Screen)
    * Where is the constructor, destructor and refresh the screen
    * In addition to the configuration of the screen as well as the resolution and the size of the screen
*/
class CDXWindow: public CWindow
{

public:

	//Devices Input
	int					DevOption;		///< Devices Option
	
	void Init			(void);

	CDXWindow			()		{Init();}		///<Constructor of the class
	~CDXWindow			(void);

	bool Create			();		///<Create the Window
	void Refresh		(void);	///<glFlush and swapbuffers

	bool InitMultiSample		(int AntialiasingMode);
	
	void setWindowMetadata		(char* title,int titleMaxLength, int width, int height, int bitsPerPixel, bool fullScreen, bool quickFullScreen);

	void Reshape (float);				///< Updates the Window settings when it is resahaped or moved
};
#endif