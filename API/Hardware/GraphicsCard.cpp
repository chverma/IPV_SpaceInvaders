/** Header with definitions needed for the management of any generic graphics card
*	Prefix: CGC_	for class graphic card

*	@author Ramón Mollá
*	@version 2013-06
*/
#include <GraphicsCard.h>

void CDeskTop::SystemMetrics()
{
	hDeskTop = GetDesktopWindow();
	hDeskTopDC = GetDC(hDeskTop);

	Width = GetDeviceCaps(hDeskTopDC, PLANES) * GetDeviceCaps(hDeskTopDC, HORZRES); 
	Height = GetDeviceCaps(hDeskTopDC, PLANES) * GetDeviceCaps(hDeskTopDC, VERTRES);
	BitsPerPixel = GetDeviceCaps(hDeskTopDC, PLANES) * GetDeviceCaps(hDeskTopDC, BITSPIXEL);
	ReleaseDC(hDeskTop, hDeskTopDC);
}

CGraphicCard::CGraphicCard()		///<Constructor of the class
{
	HWAccelerated	= false;

	SubTypeHW		= CHW_OUT_GRAPHIC;
	TypeHW			= CHW_GRAPHIC;
	bitsPerPixel	= CGC_DEFAULT_DEPTH;

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
	Active			= false;
}


//////////////////////////////////////////////////////////////////////////////////////
//
// FONCTIONS => La carte graphique est-elle accélérée ?
//
//
void CGraphicCard::DetectHWAcceleration()
{

	HWAccelerated = false;

#ifdef OS_MSWINDOWS
  
   //Very important to set this variable.. Windows seems
   //not to be able to do this itself;)
   PFD.nSize = sizeof(PIXELFORMATDESCRIPTOR);
   
   //Device Context of the Window you want to draw in
   //must be the same as the one you use to draw GL
   //HDC hDC = GetDC(Window.hWnd);
   CGC_DEVICE_CONTEXT_HANDLE hDC = GetDC(HWND_DESKTOP);
 
   //number of available pixel formats
   int nFormatCount = DescribePixelFormat(hDC, 1, 0, NULL);
 
   //Go through all available pixel formats and check..
   for(int i = 1; i <= nFormatCount; i++)
   {
      //Get description of pixel format
      DescribePixelFormat(hDC, i, PFD.nSize, &PFD);
 
      //Not generic?
      if (!(PFD.dwFlags & PFD_GENERIC_FORMAT))
      {
         //It's HW accelerated!
         HWAccelerated = true;
         //We can stop here, as there is at least one acc. pixel format
         return;
      }
   }
#elif	OS_LINUX
#elif	OS_OSX
#elif	OS_ANDROID
#endif
}

bool CGraphicCard::ChangeScreenResolution(int Width, int Height, int bitsPerPixel)
{
#ifdef OS_MSWINDOWS

	DEVMODE dmScreenSettings;											// Device Mode
	ZeroMemory(&dmScreenSettings, sizeof(DEVMODE));						// Make Sure Memory Is Cleared

	dmScreenSettings.dmSize				= sizeof(DEVMODE);				// Size Of The Devmode Structure
	dmScreenSettings.dmPelsWidth		= Width;						// Select Screen Width
	dmScreenSettings.dmPelsHeight		= Height;						// Select Screen Height
	dmScreenSettings.dmBitsPerPel		= bitsPerPixel;					// Select Bits Per Pixel
	dmScreenSettings.dmFields			= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

	if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		return FALSE;													// Display Change Failed, Return False

#elif	OS_LINUX
#elif	OS_OSX
#elif	OS_ANDROID
#endif
	return TRUE;														// Display Change Was Successful, Return True
}
