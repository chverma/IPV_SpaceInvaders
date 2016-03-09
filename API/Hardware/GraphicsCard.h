/** Header with definitions needed for the management of the graphic card
*	Prefix: CGC_	for class graphic card

*	@author Ramón Mollá
*	@version 2013-07
*/

#ifndef CGC_GRAPHIC_CARD
#define CGC_GRAPHIC_CARD

#include <Hardware.h>
//Video Graphics Array
#define CGC_VGA_HOR_RES	640
#define CGC_VGA_VER_RES	480
//Super Video Graphics Array
#define CGC_SVGA_HOR_RES	800
#define CGC_SVGA_VER_RES	600
//Extended Graphics Array
#define CGC_XGA_HOR_RES	1024
#define CGC_XGA_VER_RES	768
//Super Extended Graphics Array
#define CGC_SXGA_HOR_RES	1280
#define CGC_SXGA_VER_RES	1024
#define CGC_SXGA_VER_RES43	960
//Widescreen Super Extended Graphics Array Plus
#define CGC_WSXGA_HOR_RES	1680
#define CGC_WSXGA_VER_RES	1050
// Ultra Extended Graphics Array
#define CGC_UXGA_HOR_RES	1600
#define CGC_UXGA_VER_RES	1200
// Full High Definition
#define CGC_FHD_HOR_RES		1920
#define CGC_FHD_VER_RES		1080
// Wide Ultra Extended Graphics Array
#define CGC_WUXGA_HOR_RES	1920
#define CGC_WUXGA_VER_RES	1200

#define CGC_DEFAULT_DEPTH	24

#ifdef OS_MSWINDOWS
#define CGC_PIXEL_FORMAT PIXELFORMATDESCRIPTOR
#define CGC_WINDOW_HANDLE HWND
#define CGC_DEVICE_CONTEXT_HANDLE HDC

#elif	defined(OS_LINUX)
#elif	defined(OS_OSX)
#elif	defined(OS_ANDROID)
#endif

class CDeskTop
{
public:
	CGC_WINDOW_HANDLE			hDeskTop;
	CGC_DEVICE_CONTEXT_HANDLE	hDeskTopDC;

	int Width,
		Height,
		BitsPerPixel;

#ifdef	OS_MSWINDOWS

	CDeskTop ()
	{
		hDeskTop = GetDesktopWindow();
		hDeskTopDC = GetDC(hDeskTop);
		Width  = GetDeviceCaps(hDeskTopDC, PLANES) * GetDeviceCaps(hDeskTopDC, HORZRES);
		Height = GetDeviceCaps(hDeskTopDC, PLANES) * GetDeviceCaps(hDeskTopDC, VERTRES);
	}
	~CDeskTop (){};
#elif	defined(OS_LINUX)
#elif	defined(OS_OSX)
#elif	defined(OS_ANDROID)
#endif

	void SystemMetrics();
};

/**
*	CGraphicCard

*	Manages all the information relative to the general game application.
*	It is implemented as a singleton. This is not the game singleton
*/

class CGraphicCard: public CHardware
{
private:
	bool				HWAccelerated;	///<The graphic card is accelerated by hardware
	int					bitsPerPixel;	///<Bits Per Pixel

	CGC_PIXEL_FORMAT	PFD;

	void	DetectHWAcceleration();

public:
	CDeskTop DeskTop;

	//Methods
	void Init (void);
	CGraphicCard();		///<Constructor of the class
	~CGraphicCard(){};	///<Destructor of the class

	//Public methods
	inline bool IsHWAccelerated() {return HWAccelerated;};
	bool ChangeScreenResolution(int Width, int Height, int bitsPerPixel);
	bool ChangeScreenResolution	() {ChangeScreenResolution(DeskTop.Width, DeskTop.Height, bitsPerPixel);}

	void SystemMetrics();
};

#endif