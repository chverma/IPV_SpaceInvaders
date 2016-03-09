/**
*	Definition of the class Game Sounds
*	General class to support all the different types of sounds listened during the playing game
*	Prefix: CSIG_

*	@author Ramon Molla
*	@version 2011-11
*/

#ifndef CSIG_CLASS_GAME_SOUNDS
#define CSIG_CLASS_GAME_SOUNDS

#include <string>

/**   
   * It defines the types of sounds for the game
   * @param enum CSIG_SOUNDS: which lists the types of sounds for the game
*/

//#define CGS_SOUND_TAG_MAX_LONG 32
#define CSG_MAX_MUSIC_TRACKS 4

typedef enum
{
	CGS_NO_SOUND,	///<There is no sound played
	CGS_APPLAUSE,	///<Player has won a level and an applause is listened
	CGS_BONUS,		///<A bonus is got and an improvement is got
	CGS_CROSS,		///<Passing from 2D to 3D loop 
	CGS_DESCEND,	///<Ship descends down one step
	CGS_EXPLOSION,	///<Explosion Sound
	CGS_KEY,		///<Touched
	CGS_SHIELD,		///<Passing from 2D to 3D loop 
	CGS_SHOOT,		///<Player's shoot sound
	CGS_SHOOT3D,	///<Player's shoot sound when in 3D mode
	CGS_TOUCH,		///<Shoot sound when a hit is got
	CGS_MAX_SOUND	///<For accounting purpouses only
} CSIG_SOUNDS;

extern std::string CGS_NameSounds	[CGS_MAX_SOUND];
extern std::string CGS_SoundsFiles	[CGS_MAX_SOUND];
extern std::string CGS_MusicFiles	[CSG_MAX_MUSIC_TRACKS];

#endif
