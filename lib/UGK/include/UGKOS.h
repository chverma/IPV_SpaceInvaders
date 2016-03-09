/**	Definition of the Operating System the videogame is running on

*	Prefix: UGKOS_

*	@author Ramon Molla
*	@modified 
*	@version 2014-05
*/

#ifndef UGKOS_OPERATINGSYTEM
#define UGKOS_OPERATINGSYTEM

#define UGKOS_WINDOWS
//#define UGKOS_LINUX
//#define UGKOS_OSX
//#define UGKOS_ANDROID

#define UGKOS_OPENGL
	
#ifdef	UGKOS_WINDOWS
	#define UGKOS_THIS_DIRECTORY "."
	#define UGKOS_DIRECTORY_SEPARATOR "\\"
	//#define UGKOS_DIRECTX
#elif	defined(UGKOS_LINUX)
	#define UGKOS_THIS_DIRECTORY "."
	#define UGKOS_DIRECTORY_SEPARATOR "/"
#elif	defined(UGKOS_OSX)
#elif	defined(UGKOS_ANDROID)
#endif

#endif
