/**
/**	OS.h: Definition of the Operating System the videogame is running on
 *
 * Copyright(C) 2012
 *
 *	Prefix: OS_

 * @Author:	Dr. Ram�n Moll� Vay�
 * @Date:	02/2013
 * @Version: 2.0
 *
 * Update:
 * Date:	
 * Version: 
 * Changes:
 *
 */

#ifndef OS_OPERATINGSYTEM
#define OS_OPERATINGSYTEM

#define OS_MSWINDOWS
//#define OS_LINUX
//#define OS_OSX
//#define OS_ANDROID

#define OS_32BITS
//#define OS_64BITS

#ifdef	OS_MSWINDOWS
#elif	defined(OS_LINUX)
#elif	defined(OS_OSX)
#elif	defined(OS_ANDROID)
#endif

#endif
