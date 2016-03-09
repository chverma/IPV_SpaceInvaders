/*	Definition of the class Music for the UPV Game Kernel

	Prefix: UGKCM_

	@author Ramon Molla
	@version 2014-05
*/

#ifndef UGKCM_MUSIC //se borrar la macro musica
#define UGKCM_MUSIC //se define la macro musica

#include <UGK.h>

namespace UGK
{
	/**   
	   * Define the types of music
	   * @param enum UGKCM_MUSIC_TYPE whre the types of music is defined for the game
	*/
	typedef enum {
		CM_MOD,
		CM_MP3,
		CM_STREAMING,
		CM_MAX_MUSIC_TYPE
	} CM_MUSIC_TYPE;

	/**   
	   * Define the types of music
	   * Where the constructor and start are
	*/
	class UGK_API CMusic
	{
		//Atributes
	public:

		CM_MUSIC_TYPE	SubType;		/// Bonus subtype

		//Methods
		void Init (void){};

		CMusic(){Init();};				//Constructor of the class
	};
}
#endif
