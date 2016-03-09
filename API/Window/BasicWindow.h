/**
*	Definition of the class Basic Window

*	Prefix: CBW_

*	@author Ramón Mollá
*	@version 2013-01
*/
#ifndef CBW_CLASS_WINDOW
#define CBW_CLASS_WINDOW

#include <GraphicsCard.h>

#define NULL    0

//Antialiasing types
enum {AAx2, AAx2QUINQUX, AAx4, AAx4S};

class CBWindow
{

public:


	int				FullScreen;			///<FullScreen?
	int				quickFullScreen;	///<quickFullScreen?

	int				Visible;			///<Window Visible?
	unsigned int	Lang;
	char			*Name,				///<Application ClassName
					*Title;				///<Name of the window title
	
	int				bitsPerPixel;		///<Bits Per Pixel

	bool			Stream;
	bool			MipMap;				///<MIPMAP available
	bool			Vsync;				///<VSYNC available
	bool			VsyncActive;		///<VSYNC ACTIVE
	bool			Billboard;			///<BILLBOARDS 
	
	int				Width,
					Height,
					LeftUpX,			///<X Position of the left-up corner
					LeftUpY;			///<Y Position of the left-up corner

	//Antialiasing
	bool			Antialiasing;				///< ANTI-ALIASING available
	bool			Antialiasing_active;		///< ANTI-ALIASING ACTIVE
	int				AntialiasingMode;			///< ANTI-ALIASING MODE
	bool			AntialiasingModeOver;

	inline void DeleteTitle() {if(Title) {delete Title; Title = NULL;}}
	inline void DeleteName() {if(Name) {delete Name; Name = NULL;}}

	CBWindow(){Title = NULL; Width = CGC_VGA_HOR_RES; Height = CGC_VGA_VER_RES; bitsPerPixel = CGC_DEFAULT_DEPTH;}
	~ CBWindow(){DeleteTitle();}

	inline bool Init() {DeleteTitle();}	
};

#endif	//Window class
