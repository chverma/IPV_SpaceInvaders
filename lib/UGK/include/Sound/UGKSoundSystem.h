/**	Definition of the class Sound System for the UPV Game Kernel

*	@Prefix: UGKCSNDS_

*	@author Ramon Molla
*	@NewVersion Ramón Mollá 2014-05 - API conversion and refactoring
*/

#ifndef UGKCSNDS_SOUND_SYSTEM
#define UGKCSNDS_SOUND_SYSTEM

#include <UGKSound.h>

//For the error msg displayed
#define UGKCSNDS_ERR_MAX_LONG 128

namespace UGK
{
	class UGK_API CSoundSystem
	{

	public:

		#ifdef UGKSND_FMOD375
		// FMOD CALLBACKS
		signed short	*OscBuffer;
		int				 OscBlock ;

		FSOUND_DSPUNIT* dspUnitSounds;		// DSP SOUNDS (UNIT)
		FSOUND_DSPUNIT* dspUnitSpectrum;	// DSP SPECTRUM (UNIT)
		#elif UGKSND_FMODEx
		#elif UGKSND_OAL
		#elif UGKSND_SDL
		#endif
	
		//Methods
		void Init (void);
	
		CSoundSystem(){Init();}			// Constructor of the class

		static char *ErrorMsg(unsigned int errcode);
	};
}

#endif