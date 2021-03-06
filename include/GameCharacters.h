/*/////////////////////////////////////////////////////////////////////////////////////
//
// Every different kind of character is loaded using this header
//
	Prefix: GCHARS_

	@author Ramon Molla
	@version 2011-08
*/

#ifndef GAME_CHARACTERS
#define GAME_CHARACTERS

//#include "Copyright.h"

#define GCHAR_TAG_MAX_LONG	16	///<Maximun length of a sensitive tag

/**   
   * It defines the types of characters already in the game
   * @param enum GCHARS_CharacterType: which lists all the types of characters that can happen in a game
*/
typedef enum {
	CHARS_CHARACTER_UNKNOWN,	///<For management purpouses only
	CHARS_AIRPLANE,				///<New player called Airplane
	CHARS_BONUS,				///<For improving player strenght
	CHARS_BRICK,				///<Pieces that make a bunker
	CHARS_BUNKER,				///<A collection of bricks together in order to defend the player
	CHARS_BUNKERSMANAGER,		///<The manager of all the bunkers available in a given play
	CHARS_CIRCLESHIP,			///<Alien space ships that turns once and once again in clircles on the screen
	CHARS_GAME,					///<The game itself 
	CHARS_LASER,				///<Additional shooting left or right device for the player
	CHARS_MISSIL,				///<A common shooting from any ship, supplyship or player
	CHARS_NAVY,					///<Holds all the supplyships and all types of ships
	CHARS_PLAYER,				///<The player
	CHARS_PLAYER_SHOOT,			///<Player shoots
	CHARS_REACTOR,				///<Extra power engines and shooters for the player. Obtained after the corresponding bonus has been won
	CHARS_SHIP,					///<Alien normal space ships
	CHARS_SHIP_SHOOT,			///<Alien normal space ships shoots
	CHARS_SPHERE_OGIVE,			///<Not available by the moment
	CHARS_SUPPLYSHIP,			///<Super ship of the alien troops
	CHARS_WEAPON,				///<Not available by the moment
	CHARS_MAX_CHARTYPE			///<Only for character types management. No object exist in the game for this type
} GCHARS_CharacterType;

///Artificial Intelligence
/**   
   * It defines the types of states of an existing character in the game
   * @param enum CHARS_CHARACTER_STATE: which lists the types of states of a character
*/
typedef enum {
	CHARS_UNBORN=0,				///For management purpouses only
	CHARS_BORN,					///The character is just born but it is not still operative (living)
	CHARS_LIVING,				///Everything this character may do while it is alive 
	CHARS_DYING,				///The character has been touched so many times that its life has gone negative. So it has to burst before being died. Starts an explosion and dies
	CHARS_DEAD,					///The character is no operative. Reached after dying
	CHARS_MAXSTATE				///For management purpouses only
} CHARS_CHARACTER_STATE;		///Generic character possible states that can be any character by default

/**   
   * Defines the kinds of transitions from one character in the game existing
   * @param enum CHARS_CHARACTER_TRANSITIONS: which lists the types of transitions from one character
*/
typedef enum {
	CHARS_DEFAULT=0,			///For management purpouses only
	CHARS_BORNING,				///The character is just borning. Passing from UNBORN to BORN states
	CHARS_GETTING_ALIVE,		///Passing from BORN to LIVING states
	CHARS_BURST,				///The character has been touched so many times that its life has gone negative. So it has to burst. Passing from LIVING to DYING states
	CHARS_DIE,					///The character is no operative. Reached after dying
	CHARS_MAXTRANSITION			///For management purpouses only
} CHARS_CHARACTER_TRANSITIONS;	///Generic character possible states that can be any character by default

//Sensitive string tags
//See Character.h for information about their meanings
extern char CHARS_Tags[CHARS_MAX_CHARTYPE][GCHAR_TAG_MAX_LONG ];

///Transitions names
extern char CHARS_Transitions[CHARS_MAXTRANSITION][GCHAR_TAG_MAX_LONG ];

//Collision table that allows a character to collide with another one
extern bool CHARS_COLLISION_TABLE[CHARS_MAX_CHARTYPE][CHARS_MAX_CHARTYPE];
#endif
