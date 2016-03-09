/** Header with definitions needed for the management of any generic web camera
*	Prefix: CWC_	for class Web Cam

*	@author Ramón Mollá
*	@version 2013-06
*/

#include <Hardware\WebCam.h>

CWebCam::CWebCam()		///<Constructor of the class
{
	SubTypeHW		= CHW_IN_WEBCAM;
	TypeHW			= CHW_VIDEO;

	Active			= false;
}

#ifdef UGKOS_WINDOWS
#include <windows.h>

#elif	UGKOS_LINUX
#elif	UGKOS_OSX
#elif	UGKOS_ANDROID
#endif