#include <MSWindow.h>
#include <cstdio>
#include <commctrl.h>		// Header File For Windows XP Common Controls
#include <UGKLog.h>

//Internal temporary varaibles
#define MAX_LOADSTRING 100
TCHAR AppTitle		[MAX_LOADSTRING];	// Main bar title for the Window
TCHAR AppWindowClass[MAX_LOADSTRING];	// Name of the main window class

CMSWindow::~CMSWindow(void)
{
}

bool CMSWindow::SetRenderingContext(){ return wglMakeCurrent (hDC, hRC);}

bool CMSWindow::GetRenderingContext()	
{
	hRC = wglGetCurrentContext();

	if (NULL == hRC)
	{
		int pixelFormat = ChoosePixelFormat(hDC, &PFD);
		if(0 == pixelFormat)
		{
			MessageBox(hWnd, "ChoosePixelFormat() failed", "Error", MB_ICONERROR | MB_OK);
			exit(1);
		}

		if(!SetPixelFormat(hDC, pixelFormat, &PFD))
		{
			MessageBox(hWnd, "SetPixelFormat() failed", "Error", MB_ICONERROR | MB_OK);
			exit(1);
		}

		hRC = wglCreateContext(hDC);		// Try To Get A Rendering Context

		if(!hRC)
            MessageBox(hWnd, "wglCreateContext() failed", "Error", MB_ICONERROR | MB_OK);

        if(!wglMakeCurrent(hDC, hRC))
            MessageBox(hWnd, "wglMakeCurrent() failed", "Error", MB_ICONERROR | MB_OK);
	}

	DWORD Error = GetLastError();

	return NULL != hRC;					// Did We Get A Rendering Context?
}

bool CMSWindow::Destroy(){
	if (hWnd != NULL)								/// If the Window has a handle
	{	
		if (hDC != NULL)							/// if the Window has a device context
		{
			wglMakeCurrent(hDC, 0);					/// Set The Current Active Rendering Context To Zero
			if (hRC != NULL)						/// if The Window Have A Rendering Context
			{
				wglDeleteContext(hRC);				/// Release The Rendering Context
				hRC = 0;							/// Zero The Rendering Context
			}
			ReleaseDC(hWnd, hDC);					/// Release The Device Context
			hDC = 0;								/// Zero The Device Context
		}
		DestroyWindow(hWnd);						/// Destroy The Window
		hWnd = 0;									/// Zero The Window Handle
	}

	if (FullScreen)									/// If the Window is in fullscreen mode
	{
		ChangeDisplaySettings(NULL, 0);				/// Switch Back To Desktop Resolution
		ShowCursor(TRUE);							/// Show The Cursor
	}	
	return TRUE;
}


bool CMSWindow::Init()
{
	
	ZeroMemory(this, sizeof(CMSWindow));
	FullScreen=false;
	quickFullScreen=false;

	PIXELFORMATDESCRIPTOR pfd =					// pfd Tells Windows How We Want Things To Be
	{	
		sizeof(PIXELFORMATDESCRIPTOR),			// Size Of This Pixel Format Descriptor
			1,									// Version Number
			PFD_DRAW_TO_WINDOW |				// Format Must Support Window
			PFD_SUPPORT_OPENGL |				// Format Must Support OpenGL
			PFD_DOUBLEBUFFER,					// Must Support Double Buffering
			PFD_TYPE_RGBA,						// Request An RGBA Format
			bitsPerPixel,						// Select Our Color Depth
			0, 0, 0, 0, 0, 0,					// Color Bits Ignored
			0,									// No Alpha Buffer
			0,									// Shift Bit Ignored
			0,									// No Accumulation Buffer
			0, 0, 0, 0,							// Accumulation Bits Ignored
			CGC_DEFAULT_DEPTH,					// 24Bit Z-Buffer (Depth Buffer)  
			0,									// No Stencil Buffer
			0,									// No Auxiliary Buffer
			PFD_MAIN_PLANE,						// Main Drawing Layer
			0,									// Reserved
			0, 0, 0								// Layer Masks Ignored
	};

	PFD = pfd;

	return true;
}

//
//   FUNCIÓN: InitInstance(HINSTANCE, int)
//   PROPÓSITO: guardar el identificador de instancia y crear la ventana principal
//   COMENTARIOS:
//        En esta función, se guarda el identificador de instancia en una variable común y
//        se crea y muestra la ventana principal del programa.
//
BOOL CMSWindow::InitInstance()
{
	RECT	windowRect			= {0, 0, Width, Height};	// Define Our Window Coordinates
	DWORD	windowStyle			= WS_OVERLAPPEDWINDOW;		// Define Our Window Style
	DWORD	windowExtendedStyle	= WS_EX_APPWINDOW;			// Define The Window's Extended Style

	// quick fullscreen mode
	if (quickFullScreen) 
	{
		ShowCursor(TRUE);												// Turn Off The Cursor
		windowStyle = WS_POPUP;											// Set The WindowStyle To WS_POPUP (Popup Window)
		windowExtendedStyle |= WS_EX_TOPMOST;							// Set The Extended Window Style To WS_EX_TOPMOST
	}																	// (Top Window Covering Everything Else)
	// fullscreen mode
	else if (FullScreen)												// Fullscreen Requested, Try Changing Video Modes
	{
		SetFullScreen();
		ShowCursor(TRUE);											// Turn On The Cursor
		windowStyle = WS_POPUP;										// Set The WindowStyle To WS_POPUP (Popup Window)
		windowExtendedStyle |= WS_EX_TOPMOST;						// Set The Extended Window Style To WS_EX_TOPMOST
	}

	// Window mode
	else																// If Fullscreen Was Not Selected
		AdjustWindowRectEx(&windowRect, windowStyle, 0, windowExtendedStyle);// Adjust Window, Account For Window Borders

	hWnd = CreateWindowEx(windowExtendedStyle,		// Extended Style
		Name,										// Class Name
		Title,										// Window Title
		windowStyle,								// Window Style
		LeftUpX, LeftUpY,							// Window X,Y Position
		windowRect.right - windowRect.left,			// Window Width
		windowRect.bottom - windowRect.top,			// Window Height
		HWND_DESKTOP,								// Desktop Is Window's Parent
		0,											// No Menu
		hInstance,									// Pass The Window Instance
		NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   return TRUE;
}

bool CMSWindow::Create(int LeftUpX, int LeftUpY, int Width, int Height, int bitsPerPixel, char *title, int arbMultisampleFormat, bool arbMultisampleSupported, bool Antialiasing, int AntialiasingMode)
{
	//Initializing global strings
	LoadString(hInstance, IdTitle, AppTitle, MAX_LOADSTRING);
	LoadString(hInstance, IdApp, AppWindowClass, MAX_LOADSTRING);
	Register();

	// Realizar la inicialización de la aplicación:
	if (!InitInstance ())
		return FALSE;

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IdApp));

	//**************************************************************//
	// Initialize Common Controls
	INITCOMMONCONTROLSEX icex;					// Initialize Windows Common Controls
	
	// Load the ToolTip class from the DLL
	icex.dwSize = sizeof(icex);
	icex.dwICC  = ICC_BAR_CLASSES;				// For Tooltips
	
	if(!InitCommonControlsEx(&icex))
		BOX("InitCommonControlEx() failed.");

	CreateRenderingContext();
	if(!IsDeviceContextOK() || !IsRenderingContextOK())
	{
		Destroy();
		return FALSE;
	}

	Visible = TRUE;						/// Set Visible To True

	//Reshape(45.0f);

	return TRUE;						// Window Creating Was A Success
}

void CMSWindow::SetFullScreen(){
	// fullscreen
	DWORD windowStyle			= WS_POPUP;			// Set The WindowStyle To WS_POPUP (Popup Window)
	DWORD windowExtendedStyle	= WS_EX_TOPMOST;	// Set The Extended Window Style To WS_EX_TOPMOST

	// change style
	DWORD dwStyle,dwNewStyle;
	
	dwStyle = GetWindowLong(hWnd, (TRUE ? GWL_EXSTYLE : GWL_STYLE));

	dwNewStyle = (dwStyle & (~NULL)) | windowExtendedStyle;
	
	SetWindowLong(hWnd, (TRUE ? GWL_EXSTYLE : GWL_STYLE), dwNewStyle);

	SetWindowPos(hWnd,NULL,0,0, DeskTop->Width, DeskTop->Height,SWP_NOZORDER);

}

void CMSWindow::CreateRenderingContext(){

	//No DC is available for this hWnd, so create a new one
	if (GetDeviceContext())
		//In case the DC is new, so a new RC has to be created ans associated to it 
		if (GetRenderingContext())
			if (!SetRenderingContext())	// Get the one associated to this OpenGL Window
			{
				LPVOID lpMsgBuf;
				FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf, 0, NULL);
				//MessageBox(NULL, "Current window context lost. No way to device context.", "Error", MB_OK | MB_ICONINFORMATION);
				LocalFree(lpMsgBuf);

				OutputDebugString("Failed in CreateRenderingContext()");
			}
}

bool CMSWindow::GetDeviceContext()
{
	hDC = wglGetCurrentDC();
	if (NULL == hDC)
		// Get a Device Context for this Window
		hDC = GetDC(hWnd);				// Grab A Device Context

	return NULL != hDC;					// Did We Get A Device Context?
}

//LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Register A Window Class
bool CMSWindow::Register()
{

	WNDCLASSEX windowClass;												// Window Class
	ZeroMemory(&windowClass, sizeof(WNDCLASSEX));						// Make Sure Memory Is Cleared

	windowClass.cbSize			= sizeof(WNDCLASSEX);					// Size Of The windowClass Structure
	windowClass.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraws The Window For Any Movement / Resizing
	windowClass.lpfnWndProc		= WndProc;								// WindowProc Handles Messages
	windowClass.hInstance		= hInstance;							// Set The Instance
	windowClass.hbrBackground	= static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));// Class Background Brush Color
	//windowClass.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	windowClass.hCursor			= LoadCursor(hInstance, IDC_ARROW);		// Load The Arrow Pointer
	windowClass.lpszClassName	= Name;									// Sets The Applications Classname
	windowClass.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IdApp));
	windowClass.cbClsExtra		= 0;
	windowClass.cbWndExtra		= 0;
	windowClass.lpszMenuName	= MAKEINTRESOURCE(IdApp);
	windowClass.hIconSm			= LoadIcon(windowClass.hInstance, MAKEINTRESOURCE(IdIcon));

	if (0 == RegisterClassEx(&windowClass))								// Did Registering The Class Fail?
	{
		// NOTE: Failure, Should Never Happen
		MessageBox(HWND_DESKTOP, "RegisterClassEx Failed!", "Error", MB_OK | MB_ICONEXCLAMATION);
		return false;													// Return False (Failure)
	}

	return true;														// Return True (Success)
}