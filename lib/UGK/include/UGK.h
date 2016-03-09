/**	Definition of the general characteristics of the UPV Game Kernel - UGK

*	Prefix: UGK_

*	@author Ramon Molla
*	@modified 
*	@version 2014-05
*/

#ifndef UGK_UGK
#define UGK_UGK

#include <UGKOS.h>

#ifdef	UGKOS_WINDOWS
	//#define DLL_EXPORTS
	//#define DLL_IMPORTS
	
	#ifdef DLL_EXPORTS
		#define UGKMMO_API __declspec(dllexport)
		#define UGK_API __declspec(dllexport)
	#elif defined(DLL_IMPORTS)
		#define UGKMMO_API __declspec(dllimport)
		#define UGK_API __declspec(dllimport)
	#else
		#define UGKMMO_API
		#define UGK_API
	#endif
#elif	defined(UGKOS_LINUX)
#elif	defined(UGKOS_OSX)
#elif	defined(UGKOS_ANDROID)
#endif

#endif
