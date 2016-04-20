/**
*	Definition of the Class Space Invaders Game
*	General class to support all the attribute specific for the Sàce Invaders Game
*	Prefix: CSIG_

*	@author Ramon Molla
*	@version 2012-10
*/

#ifndef CSIG_CLASS_GAME
#define CSIG_CLASS_GAME

#include <Game.h>
#include "GameSounds.h"
#include <Characters.h>			// Header File class Bonus
#include <UGKMusic.h>
#include <UGKSoundsManager.h>
#include <UGKSoundSystem.h>
#include <SI_AI.h>
#include <iostream>
#include <fstream>
#include <ExecutionMode.h>
#include <SIMessage.h>
#include <GameCharacters.h>

#define CSIG_INIT_INITIAL	  0	//Number in the array or elements that holds the configuration by default
								//defined in the "initialization.html" file during the initialization phase
#define CSIG_INIT_CURRENT	  0	//Position of the current element used as default Player, super ship,...

//PLAYING GROUND BOUNDARIES. For frustrum culling,  player, navy,... movements
#define CSIG_PG_BOTTOM	-10.0f
#define CSIG_PG_CEILING	 13.0f
#define CSIG_PG_RIGHT	 08.0f
#define CSIG_PG_LEFT	-07.5f

//Limits for Ships Movement with Bounding Mode if it is Active.
#define CSIG_PG_SHIPS_BOTTOM	-3.5f
#define CSIG_PG_SHIPS_CEILING	10.0f

#define CSIG_PLAYER_Y_POSITION			-6.5f
#define CSIG_PLAYER2_Y_POSITION			-6.0f
#define CSIG_NAVY_INITIAL_Y_POSITION	 2.0f

#define CSIG_INTERMISSION_TEXT_BLINKS 3

#define CSIG_VERSION "1.0.0"			//Version number for the current implementation of the Space Invaders video game
#define CSIG_TITTLE  "Space Invaders"	//Tittle name for the current implementation of the Space Invaders video game

#define CSIG_SAVED_GAME  "SavedGame.html"

//Directories where to retrieve the information required by the video game
#define CSIG_INITIALIZATION_FILE		".\\Initialization.html"
#define CSIG_INITIALIZATION_LOG_FILE	".\\InitializationLog.txt"
//ANA
#define CSIG_LANGUAGE_LOG_FILE	".\\LANInitializationLog.txt"
#define CSIG_GAME_ULEVEL_FILE			".\\Levels\\"

#define CSIG_MIN_UPDATETIME_OBJECTS	40	//ms. None of the objects should update over this period of time.

#define SIGLB_1KB	1024


//Game States
typedef enum	
{
	CSIG_INITIAL,			///<The game is in the initialization phase
	CSIG_CONFIGURE,			///<The game is now interfacing with the player configuring render and game options
	CSIG_FADING_IN,			///<The game is appearing from black while is reading the level information and updating internal data structures
	CSIG_FADING_OUT2PLAY,	///<Fading out from loading to play
	CSIG_PLAYING,			///<The Player is playing now the game
	CSIG_SAVING,			///<The game is saved for posterior playing	
	CSIG_PASSING_2_3D,
	CSIG_INTERMISSION,		///<When the general game is moving because a bonus has been catched, the player has lost a life, the game,...
	CSIG_LOST_LIFE,			///<The Player has lost one life
	CSIG_WON,				///<The Player has won the game
	CSIG_LOST_GAME,			///<The Player has lost the game	
	CSIG_FADING_OUT2WON,	///<The game is Fading to black while releasing all data internal structures
	CSIG_FADING_OUT2END,	///<The game is Fading to black while releasing all data internal structures
	CSIG_FADING_OUT2LOST,	///<The game is Fading to black while releasing all data internal structures
	CSIG_PAUSED,			///<The game stays on hold
	CSIG_WAIT_KEY,			///<The game waits till the player presses any key.
	CSIG_END,				///<Releasing the videogame until new game session
	CSIG_LOST_LIFE2,
	CSIG_MAXSTATE			///<Amount of different states in the game
} CSIG_GAME_STATE;

///Different transitions for the FSMs that control the Game behaviour
typedef enum {
	CSIG_DEFAULT=0,			///For management purpouses only
	CSIG_STARTING,			///The game is just born and initialized. Now it is time for interfacing with the player
	CSIG_LOADING,			///Loading level to play
	CSIG_FADING_OUT,		///Finishing the loading stage before atarting the match
	CSIG_LOADED,			///The current level has just being loaded. Go to play
	CSIG_LIFE_LOST,			///One life has been lost. A general camera moving has to be performed
	CSIG_PLAY,				///Go on playing
	CSIG_SAVE,				///Save the game
	CSIG_SAVED,				///The game has been saved and goes back to playing
	CSIG_CHANGING_2_3D,		///When a CBN_BONUS_3D Bonus is touched, a change to 3D view mode is performed
	CSIG_3D_CHANGED,		///The transition to 3D view mode is finished. Now, turn back to CSIG_PLAYING state
	CD_INTERMISS_FINISHED,	///The intermission has finished
	CSIG_FADING2WON,		///The current level is going to being won
	CSIG_FADING2END,		///The game is going to end
	CSIG_FADING2LOST,		///The current level is going to being lost
	CSIG_FADED,				///The current level has just been won or lost or the game has just ended
	CSIG_NEXT_LEVEL,		///The level has been won and now a new level has to be loaded
	CSIG_UPDATE,			///Event given to any state to evolve/update itself
	CSIG_PAUSE,				///The player asks the game to pause
	CSIG_CONTINUE,			///The player asks the game to continue
	CGS_KEYPRESSED,			///The player pressed a key to start a new game or level.
	CSIG_2END,
	CSIG_LIFE_LOST2,
	//v
	CSIG_MAXTRANSITION		///For management purpouses only
} CSIG_GAME_TRANSITIONS;	///Generic character possible states that can be any character by default

enum TimerType
{
	TmrApp,					//Timer for timing the running application.
	TmrRTDesk,				//Timer to control calls to RT-DESK Engine Simulation.
	TmrUpd,					//Timer to measure the time of Update.
	TmrRnd,					//Timer to measure the time of Render.
	TmrIdl,					//Timer to measure the time of Idle.
	TmrRTDSKMM,				//Timer to control message handling time and time memory management.
	TmrFPS,					//Timer to control the time displayed readings.
	TIMER_AMOUNT
};

typedef enum {
	SIGLB_FILE_MUSIC,
	SIGLB_STREAMING_MUSIC,
	SIGLB_MP3_MUSIC,
	SIGLB_MAX_MUSIC,
} MUSIC_SOURCE;

extern char CSIG_NameTransition[CSIG_MAXTRANSITION][GCHAR_TAG_MAX_LONG ];
extern UGK::CSoundsManager	SoundsManager;

/**
*	\class CSIGame
* Game singleton that manages middleware specific for this specific game.
* It behaves also as a character because it has its own behaviour.
*/
class CSIGame: public CGame
{
	//Attributes
	RTDESK_TIME nowTime;
	RTDESK_TIME elapTimeRnd;
	RTDESK_TIME elapTimeUpd;
	RTDESK_TIME elapTimeShipsUpd;
	RTDESK_TIME elapTimeSShipsUpd;
	RTDESK_TIME elapTimeCShipsUpd;
	RTDESK_TIME elapTimePlayerUpd;
	RTDESK_TIME elapTimePlayer2Upd;

	RTDESK_TIME lastTimeRnd;
	RTDESK_TIME lastTimeUpd;
	RTDESK_TIME lastTimeShipsUpd;
	RTDESK_TIME lastTimeSShipsUpd;
	RTDESK_TIME lastTimeCShipsUpd;
	RTDESK_TIME lastTimePlayerUpd;
	RTDESK_TIME lastTimePlayer2Upd;

	void InitGraphics();

public:

		//Music Management
	char	CurrentMusic		[SIGLB_1KB],
			music_affichee		[SIGLB_1KB],
			stream_name			[SIGLB_1KB],
			stream_name_user	[SIGLB_1KB];

	cHRTimerManager TimerManager;
	CHAR_RENDER_MODE NextRenderMode;

	//Presentation attributes
	bool	LoadingBMP;
	float	LoadingBMPPosition;
	bool	ProgressBar;

	bool	Text_2D;							// 2D TEXT AFFICHE ?
	bool	Spectrum;							// SPECTRUM (BEAT DETECTION)?

	bool			Paused;
	bool			LevelReaded;				// Level reading on
	bool			ReadLevel;					// Activate/Deactivate, reading on from parser
	bool			OnlyOneLevel;				// Activated, Game is executed only with the current level
	int				NextHtmlInd2Load;			// With OnlyOneLevel active, indicate the html file index to load next level.
	char *			LevelFileName;
	vector <char*>	HtmlFiles;
	bool			StopAfterOneLevel;
	unsigned int	SSxLEV;						// Amount of Supply Ships per level...
	int				PlayerLevDefaultHealth;		// Player default health per level

	bool			passing_2D_2_3D;

	//Legacy attributes to refactor
	#ifdef UGKSND_FMOD375
	#elif CSND_FMODEx
	#elif CSND_OAL
	#elif UGKSND_SDL
		Mix_Music *music;
	#endif

	bool			AboutDlgBoxMODWorking;	// Music For About-DialogBox
	int				MusicConsoleOutput;
	bool			Sound2D3DPassed;		// Sound passing from 2D to 3D

	///Auxiliar attributes for transition management
	bool	intermede_sound;
	double	intermede_angle;		// INTERMEDE ANGLE pour l'animation de l'Game.intermede
	float	intermede_angle_total;	// INTERMEDE ANGLE TOTAL pour l'animation de l'Game.intermede
	bool	intermede_angle_moitie;	// INTERMEDE MOITIE moitie du chemin parcouru pour l'accélération et la décélération
	bool	active_splash;			// SPLASH SCREEN activé après l'intermède
	float	blink_intermede;		// INTERMEDE le texte de l'Game.intermede apparaît
	int		blink_intermede_nb;		// Number of Blinks performed
	
	//RT-DESK
	RTDESK_CEngine *RTDESK_Engine;
	RTDESK_TIME NextSimulation;

	double timeResetTestMsg;						//RTDESK Reset Test Message Time
	RTDESK_CMsg *msg;								//RTDESK Message Time
	cMsgGame *msgRender;							//Message to Render
	cMsgGame *msgUpdate;							//Message to Update
	RTDESK_CMsg *msgResetTest;						//RTDESK Reset Test Message
	

	double CircleShipTemp;
	double SupplyShipTemp;
	double ShipTemp;
	double NavyShipsTemp;
	double PlayerTemp;
	double Player2Temp;
	double ShootsTemp;
	double RenderTemp;
	double UpdateTemp;
	double BonusTemp;

	//Timers and times
	RTDESK_TIME TicksToRender;
	cHRTimer RenderTimer;
	RTDESK_TIME TicksToUpdate;
	cHRTimer UpdateTimer;
	RTDESK_TIME TicksToUpdShootsMngr;
	cHRTimer UpdateShootsTimer;
	RTDESK_TIME TicksToUpdBonusMngr;
	cHRTimer UpdateBonusTimer;


//methods
	//Constructor
	 CSIGame ();
	~CSIGame ();

	void Init(void);
	bool Initialize (void);			// Any GL Init Code & User Initialization Goes Here
	void InitCharFactory();
	void Deinitialize (void);
	void ResetObjects(void);

	void LoadDevice(void);     // Method for load input devices 

	void Loading();

	void LoadRTDESK();
	void LauncherRTDESK();

	void LoadLevel(int lev);
	void LoadLevelFile(int HtmlFileInd);
	void LoadGame (char * filename);
	void InitTimers(void);
	void RunMainLoop();
	void PromoteLevel();	
	void Display();
	void DisplayScore();
	void DisplayLoadingBMP();
	void Snapshot(char* filename);

	void Save (char * fileName); /// Saves the game to a file - Rodrigo Aldecoa

	///Shows the whole game on the screen
	///These are not virtual since this class does not come from class Character
	void Render(void);
	void ReshapeGL(void);

	///Change the way the whole game is going to be rendered on the screen
	void ChangeRenderMode(CHAR_RENDER_MODE);

	//AI Methods
	inline void GameEvent(CSIG_GAME_TRANSITIONS EventName) {AI->outEvent(CSIG_NameTransition[EventName], NULL, (void*) this);}
	///Artificial Intelligence character initialization
	void AI_Init		(SIAI_AI_TYPE Type);	
	///Moving the Supply Ship
	void AI_Move		(float ms10);		
	///The supply ship is going to burst before being dead
	void AI_Dye			();					

	void StartCameraMov();
	void Update ();		//Update the world. Similar to render() but updating
	void DiscreteUpdate ();		//Discrete Simulation Update
	
	void ReceiveMessage(RTDESK_CMsg *pMsg);

	inline void MusicFadeIn		(void)	{SoundsManager.MusicFadingIn();	AboutDlgBoxMODWorking = false;}
	inline void MusicFadeOut	(void)	{SoundsManager.MusicFadingOut();AboutDlgBoxMODWorking = false;}

	inline void GenerateFullAIName (char * Result, char * Name) {GenerateFullPathName(Result, Directories[CG_AI_DIR], Name);}
	void GenerateFullPathName (char * Result, char * Path, char * Name);
};

char * SaveCharacter(int i, GCHARS_CharacterType type); /// Saves the current properties of a character - Rodrigo Aldecoa

void *CSIG_Go2Play(LPSTR *args, CSIGame *Game);

//Calbacks for threads
	void MusicFadingIn	(void *dummy);
	void MusicFadingOut	(void *dummy);

#endif