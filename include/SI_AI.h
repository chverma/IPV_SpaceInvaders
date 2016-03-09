/**	Definition of the different types of Artificial Intelligence in the Space Invaders game

*Prefix: SIAI_

*	@author Ramon Molla
*	@version 2012-12
*/

#ifndef SIAI_AI
#define SIAI_AI

#define SIAI_GAME_AI_FILE				"FSM_game.html"
#define SIAI_GAME_AI_LOG_FILE			"FSM_game_Log.txt"

#define SIAI_CIRCLESHIP_AI_FILE			"FSM_circleship.html"
#define SIAI_CIRCLESHIP_AI_LOG_FILE		"FSM_circleship_Log.txt"

#define SIAI_SUPPLYSHIP_AI_FILE			"FSM_supplyship.html"
#define SIAI_SUPPLYSHIP_AI_LOG_FILE		"FSM_supplyship_Log.txt"

#define SIAI_SHIP_AI_FILE				"FSM_ship.html"
#define SIAI_SHIP_AI_LOG_FILE			"FSM_ship_Log.txt"

typedef enum {
	SIAI_GAME_DEFAULT,
	SIAI_SHIP_DEFAULT,
	SIAI_SUPPLYSHIP_DEFAULT,
	SIAI_CIRCLESHIP_DEFAULT,
	SIAI_BONUS_DEFAULT,
	SIAI_MAX_AI
} SIAI_AI_TYPE;

#endif