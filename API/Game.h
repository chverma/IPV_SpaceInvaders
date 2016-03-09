/**
*	Definition of the class game
*	General class to support all the attribute general to all the potential games produced in the future.
*	Prefix: CG_

*	@author Ramon Molla
*	@version 2012-10
*/

#ifndef CG_CLASS_GAME
#define CG_CLASS_GAME

#include <character.h>
#include <RTDeskEngine.h>
#include <HRTimerManager.h>

#define CG_LONG_DIR_NAME 256

typedef enum {
	CG_AI_DIR,
	CG_GEOMETRY_DIR,
	CG_LEVEL_DIR,
	CG_MUSIC_DIR,
	CG_SOUNDS_DIR,
	CG_TEXTURE_DIR,
	CG_MAX_DIR
} CG_DIRECTORIES;

typedef enum {
	CG_DISCRETE,
	CG_CONTINUOUS,
	CG_MAX_SIMULATION_MODE	//For management purpouses only. Always at the end of the enumeration
} CG_SIMULATION_MODE;

/**
*	\class CGame
* Game singleton that manages middleware specific for this specific game.
* It behaves also as a character because it has its own behaivor.
*/
class CGame: public CCharacter 
{
	//Game general attributes
	char	*Version;		///< Game VERSION
	char	*Title;			///< Game TITLE

public:

	unsigned int	MaxPlayers;		///<Only for initialization purpouses
	
	//Visualization
	CHAR_RENDER_MODE RenderMode;	///<The way the character is rendered on the screen

	///Level control
	int	Level;			///<Current LEVEL of the game
	unsigned int	MaxLevels;		///<Actual levels

	//These two attributes have to be managed by the score manager and initialized by the initialization parser
	//More scores and names have to be stored also when finished a game session
	unsigned int	Score;			///<Current score
	unsigned int	HiScore;		///<Current highest score

	///Directories
	char Directories	[CG_MAX_DIR][CG_LONG_DIR_NAME];

	///Devices
	std::vector<string>	DevicesIO;

//methods
	//Constructor
	CGame (){Reset();};
	~CGame ();

	void Reset();

	virtual void Save (char * fileName); /// Saves the game to a file - Rodrigo Aldecoa

	void SetVersion		(char *);
	void SetTitle		(char *);

	inline char *GetVersion()	{return Version;};
	inline char *GetTitle()		{return Title;};
};

#endif