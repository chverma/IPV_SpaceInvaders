/**	Definition of the class Micro Soft Window

	Prefix: CMSW_

	@author Ramon Molla
	@version 2013-06
*/

#ifndef WMSW_WIN
#define WMSW_WIN

#include <windows.h>
#include <BasicWindow.h>

#define CMSW_NO_HWND NULL
#define CMSW_NO_HDC  NULL
#define CMSW_NO_HRC  NULL

#define CMSW_OGL
//#define CMSW_DX

#ifdef CMSW_OGL
#define CMSW_SAMPLE_BUFFERS_ARB		 0x2041      //Define the buffer size
#define CMSW_SAMPLES_ARB			 0x2042      //Define a sample size
#elif CMSW_DX
#endif

class CMSWindow: public CBWindow
{
	bool	Register	();
	BOOL	InitInstance();

	int		IdApp,
			IdIcon,
			IdTitle;	//For MS Windows internal management purposes only
	WNDPROC	WndProc;	//Call back to manage the window
	int		CmdShow;	//For initialization purposes
	HACCEL	hAccelTable;//Acceleration Table handle
	CDeskTop *DeskTop;	//Desktop reference

public:

	CGC_WINDOW_HANDLE			hWnd;	///<Window Handle
	CGC_DEVICE_CONTEXT_HANDLE	hDC;	///<Device Context
	HGLRC						hRC;	///<Rendering Context
	CGC_PIXEL_FORMAT			PFD;

	//Application attributes
	HINSTANCE		hInstance;			///<Application Instance
	
	bool Init();

	CMSWindow	()		{Init();};
	~CMSWindow	(void);

	inline bool IsDeviceContextOK	()	{return NULL != hDC;};
	inline bool IsRenderingContextOK()	{return NULL != hRC;};
	/**   
	* @fn bool CGLWindow::IsCreated()
	* This function verifies if the OpenGL window is created
	*/
	inline bool IsCreated		() {return NULL != hWnd; }		///<Is the Window created?

	bool InitMultiSample2		(int AntialiasingMode);

	bool		Destroy					();
	bool		Create					(int LeftUpX, int LeftUpY, int Width, int Height, int bitsPerPixel, char *title, int arbMultisampleFormat, bool arbMultisampleSupported, bool Antialiasing, int AntialiasingMode);
	inline void Show					()								{ShowWindow(hWnd, CmdShow); UpdateWindow(hWnd);}				

	inline void	SetId					(int App, int Icon, int Title)	{IdApp = App; IdIcon = Icon;	IdTitle	= Title;}
	inline void SetWndProc				(WNDPROC Proc)					{WndProc = Proc;}
	inline void SetCmdShow				(int CommandShow)				{CmdShow = CommandShow;}
	inline void SetDeskTop				(CDeskTop *DT)					{DeskTop = DT;}
	inline int	TranslateAcc			(MSG *msg)						{return TranslateAccelerator(msg->hwnd, hAccelTable, msg);}
			
	void		SetFullScreen			();
	void		CreateRenderingContext	();
	bool		GetDeviceContext		();
	bool		SetRenderingContext		();
	bool		GetRenderingContext		();	///<Determines if the GL context is available and gets it if possible
};

#endif