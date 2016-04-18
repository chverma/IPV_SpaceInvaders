/*/////////////////////////////////////////////////////////////////////////////////////
//
// Definitions of Transitions and tags.
//
	Prefix: CHARS_

	@author Ramon Molla
	@version 2011-08
*/

//#include "Copyright.h"
#include "GameCharacters.h"

//Sensitive string tags
//See Character.h for information about their meanings
/**   
   * @fn CHARS_Tags 
   * Chars tags
   * @param CHARS_MAX_CHARTYPE: maximum number of fonts
   * @param GCHAR_TAG_MAX_LONG: the maximum length of the tags
   * Tag matrix for characters allowed
*/
char CHARS_Tags[CHARS_MAX_CHARTYPE][GCHAR_TAG_MAX_LONG ] = 
{
	/*CHARS_CHARACTER_UNKNOWN,	///<For management purpouses only
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
	CHARS_PLAYER2,				///<The player2
	CHARS_PLAYER_SHOOT,			///<Player shoots
	CHARS_REACTOR,				///<Extra power engines and shooters for the player. Obtained after the corresponding bonus has been won
	CHARS_SHIP,					///<Alien normal space ships
	CHARS_SHIP_SHOOT,			///<Alien normal space ships shoots
	CHARS_SPHERE_OGIVE,			///<Not available by the moment
	CHARS_SUPPLYSHIP,			///<Super ship of the alien troops
	CHARS_WEAPON,				///<Not available by the moment
	CHARS_MAX_CHARTYPE	
	*/

	"UNKNOWN",
	"BONUS",
	"BRICK",
	"BUNKER",
	"BUNKERMANAGER",
	"CIRCLESHIP",
	"GAME",
	"LASER",
	"MISSIL",
	"NAVY",
	"NAVY_SHOOT",
	"OGIVE",
	"PLAYER",
	"PLAYER2",
	"PLAYER_SHOOT",
	"REACTOR",
	"SHIP",
	"SHIP_SHOOT",
	"SPHERE_OGIVE",
	"SUPPLYSHIP",
	"WEAPON"
};

///Sensitive string tags
/**   
   * @fn CHARS_Transitions 
   * Transitions of characters
   * @param CHARS_MAXTRANSITION: maximum number of transitions
   * @param GCHAR_TAG_MAX_LONG: the maximum length of the tags
   * Transition matrix for characters allowed
*/
char CHARS_Transitions[CHARS_MAXTRANSITION][GCHAR_TAG_MAX_LONG ] = 
{
	"DEFAULT",
	"BORNING",
	"GETTING_ALIVE",
	"BURST",
	"DIE"
};

///Every time a new character is included, this table has to be updated
bool CHARS_COLLISION_TABLE[CHARS_MAX_CHARTYPE][CHARS_MAX_CHARTYPE] = 
{
//				UNKNOWN	BONUS	BRICK	BUNKER	BUNKER_M	CIRCLESHIP	GAME	LASER	MISSIL	NAVY	N_SHOOT	OGIVE	PLAYER	PLAYER2	P_SHOOT	REACTOR SHIP	S_SHOOT	SPH_OGIVE	SUPPLYSHIP	WEAPON	
/*UNKNOWN*/		{false,	false,	false,	false,	false,		false,		false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,		false,		false},
/*BONUS*/		{false,	false,	false,	false,	false,		false,		false,	true,	false,	false,	false,	false,	true,	true,	false,	true,	false,	true,	false,		false,		false},
/*BRICK*/		{false,	false,	false,	false,	false,		true,		false,	true,	true,	true,	true,	false,	true,	true,	true,	true,	true,	true,	false,		true,		false},
/*BUNKER*/		{false,	false,	false,	false,	false,		true,		false,	true,	true,	true,	true,	false,	true,	true,	true,	true,	false,	true,	false,		true,		false},
/*BUNKER_M*/	{false,	false,	false,	false,	false,		false,		false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,		false,		false},
/*CIRCLESHIP*/	{false, false,	true,	true,	false,		false,		false,	true,	true,	false,	false,	false,	true,	true,	true,	true,	false,	false,	false,		false,		true },
/*GAME*/		{false,	false,	false,	false,	false,		false,		false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,		false,		false},
/*LASER*/		{false,	true,	true,	true,	false,		true,		false,	false,	true,	true,	true,	false,	false,	false,	false,	false,	true,	true,	false,		true,		false},
/*MISSIL*/		{false,	false,	true,	true,	false,		true,		false,	true,	false,	true,	false,	false,	true,	true,	false,	true,	true,	true,	false,		true,		false},
/*NAVY*/		{false,	false,	true,	true,	false,		false,		false,	true,	true,	false,	false,	false,	true,	true,	true,	true,	false,	false,	false,		false,		true },
/*N_SHOOT*/		{false,	false,	true,	true,	false,		false,		false,	true,	false,	false,	false,	false,	true,	true,	false,	true,	false,	false,	false,		false,		false},
/*OGIVE*/		{false,	false,	false,	false,	false,		false,		false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,	false,		false,		false},
/*PLAYER*/		{false,	true,	true,	true,	false,		true,		false,	false,	true,	true,	true,	false,	false,	false,	false,	false,	false,	true,	false,		true,		false},
/*PLAYER2*/		{false,	true,	true,	true,	false,		true,		false,	false,	true,	true,	true,	false,	false,	false,	false,	false,	false,	true,	false,		true,		false},
/*P_SHOOT*/		{false,	false,	true,	true,	false,		true,		false,	false,	false,	true,	false,	false,	false,	false,	false,	false,	false,	false,	false,		true,		false},
/*SHIP*/		{false,	false,	true,	true,	false,		false,		false,	true,	true,	false,	false,	false,	true,	true,	true,	true,	false,	false,	false,		false,		true },
/*SHIP_SHOOT*/	{false,	false,	true,	true,	false,		false,		false,	true,	true,	false,	false,	false,	true,	true,	true,	true,	false,	false,	false,		false,		true },
/*SPHERE_OGIVE*/{false,	false,	true,	true,	false,		false,		false,	true,	true,	false,	false,	false,	true,	true,	true,	true,	false,	false,	false,		false,		true },
/*S_SHIP*/		{false,	false,	true,	true,	false,		false,		false,	true,	true,	false,	false,	false,	true,	true,	true,	true,	false,	false,	false,		false,		true },
/*SUPPLYSHIP*/	{false,	false,	true,	true,	false,		false,		false,	true,	true,	false,	false,	false,	true,	true,	true,	true,	false,	false,	false,		false,		true },
/*WEAPON*/		{false,	false,	false,	false,	false,		true,		false,	false,	false,	true,	false,	false,	false,	false,	false,	false,	false,	false,	false,		true,		false}
};
