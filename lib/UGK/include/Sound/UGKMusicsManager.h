/**
*	Definition of the class Music Manager for the UPV Game Kernel
*	Prefix: UGKCMM_

*	@author Ramon Molla
*	@version 2014-05
*	@NewVersion Ramón Mollá 2014-05 - API conversion and refactoring
*/

#ifndef UGKCMM_MUSIC //Se borra la macro de Manejador de Musica
#define UGKCMM_MUSIC //Se define la macro de Manejador de Musica

#include <UGKMusic.h>

namespace UGK
{
	/**   
	   * It defines the class of Music Manager
	   * Vector that defines the music for a given time
	*/
	class UGK_API CMusicsManager
	{
		//Attributes
	public:
		CMusic *Music;	///<Array of different musics that can be performed at any time

		void Load	(char* file);
	};
}
#endif
