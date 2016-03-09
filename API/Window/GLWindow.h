/**	Definition of the class GL Window

	Prefix: CGLW_

	@author Ramon Molla
	@version 2011-10
*/

#ifndef CGLW_GLWINDOW
#define CGLW_GLWINDOW

#include <UGKOS.h>
#ifdef	UGKOS_WINDOWS
#include <Window.h>
#elif	defined(UGKOS_LINUX)
#elif	defined(UGKOS_OSX)
#elif	defined(UGKOS_ANDROID)
#endif

#define CGL_SAMPLE_BUFFERS_ARB		 0x2041      //Define a buffer size
#define CGL_SAMPLES_ARB			     0x2042      //Define a sample size

#define CGLW_NO_HWND NULL
#define CGLW_NO_HDC  NULL
#define CGLW_NO_HRC  NULL

#define CGL_ResetModelView() 	{glMatrixMode(GL_MODELVIEW);glLoadIdentity();}
#define CGL_ResetProjection() 	{glMatrixMode(GL_PROJECTION);glLoadIdentity();}

/**   
	* It defines the class Window (Screen)
    * Where is the constructor, destructor and refresh the screen
    * In addition to the configuration of the screen as well as the resolution and the size of the screen
*/
class CGLWindow: public CWindow
{

public:
	// ARB-MULTISAMPLE Anti-aliasing (OpenGL extension)
	bool				arbMultisampleSupported;	///< EXTENSION "ARBMULTISAMPLE" AVAILABLE
	int					arbMultisampleFormat;		///< PIXEL FORMAT USED IF THE EXTENSION IS AVAILABLE

	//Devices Input
	int					DevOption;		///< Devices Option
	
	void Init			(void);

	CGLWindow			()		{Init();}		///<Constructor of the class
	~CGLWindow			(void);

	bool Create			();		///<Create the Window
	void Refresh		(void);	///<glFlush and swapbuffers

	bool SetRenderingContext();

	bool WGLExtensionSupported	(const char *extension);
	#ifdef	UGKOS_WINDOWS
		bool InitMultiSample		(int AntialiasingMode);
	#elif	defined(UGKOS_LINUX)
	#elif	defined(UGKOS_OSX)
	#elif	defined(UGKOS_ANDROID)
	#endif

	void setWindowMetadata		(char* title,int titleMaxLength, int width, int height, int bitsPerPixel, bool fullScreen, bool quickFullScreen);

	void Reshape (float);				///< Updates the Window settings when it is resahaped or moved
};
#endif