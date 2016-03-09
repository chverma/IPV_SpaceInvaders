/**
*	Definition of the class Window

*	Prefix: CW_

*	@author Ramón Mollá
*	@version 2013-01
*/
#ifndef CW_CLASS_WINDOW
#define CW_CLASS_WINDOW

#include <UGKOS.h>

#ifdef UGKOS_WINDOWS
#include <MSWindow.h>
#include <GraphicsCard.h>

/* Pixel format descriptor */
typedef struct
{
    WORD  nSize;
    WORD  nVersion;
    DWORD dwFlags;
    BYTE  iPixelType;
    BYTE  cColorBits;
    BYTE  cRedBits;
    BYTE  cRedShift;
    BYTE  cGreenBits;
    BYTE  cGreenShift;
    BYTE  cBlueBits;
    BYTE  cBlueShift;
    BYTE  cAlphaBits;
    BYTE  cAlphaShift;
    BYTE  cAccumBits;
    BYTE  cAccumRedBits;
    BYTE  cAccumGreenBits;
    BYTE  cAccumBlueBits;
    BYTE  cAccumAlphaBits;
    BYTE  cDepthBits;
    BYTE  cStencilBits;
    BYTE  cAuxBuffers;
    BYTE  iLayerType;
    BYTE  bReserved;
    DWORD dwLayerMask;
    DWORD dwVisibleMask;
    DWORD dwDamageMask;
} CW_PIXELFORMATDESCRIPTOR;

class CWindow: public CMSWindow
#elif	UGKOS_LINUX
class CWindow: public CLinuxWindow
#elif	UGKOS_OSX
class CWindow: public COSXWindow
#elif	UGKOS_ANDROID
class CWindow: public CAndWindow
#endif	//OS type
{
public:

	CWindow()	{}
	~ CWindow()	{}

#ifdef UGKOS_WINDOWS
	inline bool Init() {DeleteTitle(); return CMSWindow::Init();}
	
	inline void	SetPixelFormat	(PIXELFORMATDESCRIPTOR *pfd){CMSWindow::PFD = *pfd;};
	inline bool InitMultiSample	() {CMSWindow::InitMultiSample2(AntialiasingMode);}; 
	
	inline bool Create(int arbMultisampleFormat, bool arbMultisampleSupported){return CMSWindow::Create(LeftUpX, LeftUpY, Width, Height, bitsPerPixel, Title, arbMultisampleFormat, arbMultisampleSupported, Antialiasing, AntialiasingMode);}
#elif	UGKOS_LINUX
#elif	UGKOS_OSX
#elif	UGKOS_ANDROID
#endif	//OS type

};

#endif	//Window class
