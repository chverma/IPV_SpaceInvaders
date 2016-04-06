/**The place where resides the class game
*  General class to support all the attribute general to all the levels, players, ships,...
*  Prefix: CSIG_

*	@author Ramon Molla
*	@version 2011-11
*/

#include "LevelParser.h"
#include "API/Parsers/DeviceParser.h"
#include "InitializationParser.h"
#include "UGKLanguageParser.h"
#include <AI\FSMParser.h>
#include "Global.h"
#include <glext.h>
#include <UGKglError.h>
#include <UGKSoundsManager.h>
#include <UGKString.h>
#include <iostream>
#include <list>
#include <MMSystem.h>
#include <SI_AI.h>
#include <NetWork.h>
#include <string>
#include <TextResources.h>
#include <GameCharacters.h>


#define STRINGMAXLENGTH 256

char *CSIG_DefaultDirectories [CG_MAX_DIR] =
{
	"AI",
	"Models",
	"Levels",
	"Sounds",
	"Sounds",
	"Images"
};

///Different name states for the FSMs that control the SS behaviour
char CSIG_NameState[CSIG_MAXSTATE][GCHAR_TAG_MAX_LONG ] = 
{
	"INITIAL",			///The game is in the initialization phase
	"CONFIGURE",		///The game is now interfacing with the player configuring render and game options
	"FADING IN",		///The game is appearing from black while is reading the level information and updating internal data structures
	"FADING OUT PLAY",	///The game is Fading from the loading phase into the playing phase
	"PLAYING",			///The Player is playing now the game
	"SAVING",			///The Player is saving now the game
	"CHANGING TO 3D",
	"INTERMISSION",
	"LOST LIFE",		///The Player has lost one life
	"WON",				///The Player has won the game
	"LOST GAME",		///The Player has lost the game
	"FADING OUT WON",	///The game is Fading to black while releasing all data internal structures
	"FADING OUT END",	///The game is Fading to black while releasing all data internal structures
	"FADING OUT LOST",	///The game is Fading to black while releasing all data internal structures
	"PAUSED",			///The game stays on hold.
	"WAITING FOR KEY",	///The game waits till the player presses any key.
	"END"				///The game is Fading to black while releasing all data internal structures

};

///Sensitive string tags
char CSIG_NameTransition[CSIG_MAXTRANSITION][GCHAR_TAG_MAX_LONG ] = 
{
	"DEFAULT",			///For management purpouses only
	"STARTING",			///The game is just borning. Loading initialization data
	"LOADING",			///Loading level to play
	"FADING_OUT",		///Finishing the loading and going into the match
	"LOADED",			///The current level has just being loaded
	"LIFE LOST",		///One life has been lost
	"PLAY",				///Go on playing
	"SAVE",				///Go on saving
	"SAVED",			///Game Saved
	"CHANGING_TO_3D",	///When a CBN_BONUS_3D Bonus is touched, a change to 3D view mode is performed
	"3D_CHANGED",		///The transition to 3D view mode is finished. Now, turn back to CSIG_PLAYING state
	"INTERMISS_ENDED",	///The intermission has finished
	"FADING2WON",		///The current level is going to being won
	"FADING2END",		///The game is going to end
	"FADING2LOST",		///The current level is going to being lost
	"FADED",			///The current level has just been won or lost or the game has just ended
	"NEXT_LEVEL",		///The level has been won and now a new level has to be loaded
	"UPDATE",			///Event given to any state to evolve/update itself
	"PAUSE",			///The player asks the game to pause
	"CONTINUE",			///The player asks the game to continue
	"KEY_PRESSED",		///The player pressed a key to start a new game or level.
	"2END"		///v

};
///Timers names
char *TimersName[TIMER_AMOUNT] = {
									"Application",
									"RTDESK",
									"Update",
									"Render",
									"Idle",
									"System",
									"CalcFPS",
									};

///Different kind of sounds used during running time
USHORT CSIG_Resource_id[CGS_MAX_SOUND] =
{
	0,			//For management purpouses only
	IDR_SOUND_TIR,
	IDR_SOUND_TIR3D,
	IDR_SOUND_EXPLOSION,
	IDR_SOUND_APPLAUSE,
	IDR_SOUND_SHIELD_CHARGE,
	IDR_SOUND_CROSS,
	IDR_SOUND_BONUS,
	IDR_SOUND_KEY
};

/**
*	Values always betweeen 0 (No sound) and 255 (Maximun power). One byte.
*/
USHORT CSIG_SoundVolume[CGS_MAX_SOUND] =
{
	0,		//For management purpouses only
	40,		//2D Shoot
	20,		//3D Shoot
	120,	//Explosion
	250,	//Applause
	200,	//Player Shield
	255,	//Changing from 2d mode to 3D
	255,	//Bonus
	200		//Key pressed
};

Vector control_points[6];

unsigned int i;
void EndAccCounting(unsigned int indTmr);
cTextoGL txtFPS, txtRND, txtUPD, txtIDL, txtSUM, txtCP, txtRTDSKMM;
extern int initialWidthW;
extern int initialHeightW;
//2D Texts Colors
GLfloat red[3]={1.0f, 0.0f, 0.0f};
GLfloat gren[3]={0.0f, 1.0f, 0.0f};
GLfloat yelow[3]={1.0f, 1.0f, 0.0f};
GLfloat blue[3]={0.0f, 0.0f, 1.0f};
cFpsGL info;
char texto[50];

CSIGame::CSIGame()
{
	Init();
}

CSIGame::~CSIGame()
{
	//delete(msgRender);
	//delete(msgUpdate);

	//(RTDESK_Engine->Reset();
	delete(RTDESK_Engine);
}

void CSIGame::GenerateFullPathName (char * Result, char * Path, char * Name)
{	
	if (NULL == Name)
	{
		strcpy(Result, "");
		return;
	}

	strcpy(Result, UGKOS_THIS_DIRECTORY);
	strcat(Result, UGKOS_DIRECTORY_SEPARATOR);

	if (0 != strcmp("", Path))
	{
		strcat(Result, Path);
		strcat(Result, UGKOS_DIRECTORY_SEPARATOR);
	}

	strcat(Result, Name);
}

void CSIGame::Init(void)
{
	unsigned int i;

	//Game general attributes
	SetVersion	(CSIG_VERSION);	// CURRENT VERSION
	SetTitle	(CSIG_TITTLE);	// TITLE

	Log.Version = GetVersion();	// For debugging purpouses

	AI = NULL;

	//Visualization
	RenderMode			= CHAR_2D;
	
	Text_2D				= true;		// 2D TEXT AFFICHE ?
	Spectrum			= false;	// SPECTRUM (BEAT DETECTION)?

	ProgressBar			= false;	//No progress bar available when initializing the game

	passing_2D_2_3D		= false;
	Paused				= false;

	//Level control
	OnlyOneLevel		= false;
	StopAfterOneLevel	= false;
	Level				= 0;		// First LEVEL of the game
	LevelReaded			= false;	// Level reading not done
	ReadLevel			= true;		// Activate level reading by parser
	SSxLEV				= 1;		// Amount of Supply Ships per level...
	PlayerLevDefaultHealth	= -1;	// Player default health per level
	MaxLevels			= 6;

	intermede_sound			= false;
	intermede_angle			= 0;	// INTERMEDE ANGLE pour l'animation de l'Game.intermede
	intermede_angle_total	= 0;	// INTERMEDE ANGLE TOTAL pour l'animation de l'Game.intermede
	intermede_angle_moitie	= false;// INTERMEDE MOITIE moitie du chemin parcouru pour l'accélération et la décélération
	active_splash			= false;// SPLASH SCREEN activé après l'intermède
	blink_intermede			= 0;	// INTERMEDE le texte de l'Game.intermede apparaît
	blink_intermede_nb		= 0;	// INTERMEDE (clignote 3 fois)

	Alive					= false;	// Field of the father class CCharacter 
	Type					= CHARS_GAME;// Field of the father class CCharacter

	CircleShipTemp	=	41.0;
	SupplyShipTemp	=	41.0;
	ShipTemp		=	41.0;
	NavyShipsTemp	=	41.0;
	PlayerTemp		=	41.0;
	Player2Temp		=	41.0;
	ShootsTemp		=	41.0;
	RenderTemp		=	41.0;
	UpdateTemp		=	41.0;

	TicksToRender = RTDESKT_NOTIME;

	msgRender				= new cMsgGame;
	msgRender->Type			= RTDESKMSG_GAME;
	msgRender->msgSubType	= TIPORENDER;
	msgRender->Propietary	= true;

	msgUpdate				= new cMsgGame;
	msgUpdate->Type			= RTDESKMSG_GAME;
	msgUpdate->msgSubType	= TIPOUPDATE;
	msgUpdate->Propietary	= true;

	for(i = CG_AI_DIR; i<CG_MAX_DIR;i++)
		strcpy(Directories[i], CSIG_DefaultDirectories[i]);

	lastTimeRnd			= RTDESKT_NOTIME;
	lastTimeUpd			= RTDESKT_NOTIME;
	lastTimeShipsUpd	= RTDESKT_NOTIME;
	lastTimeSShipsUpd	= RTDESKT_NOTIME;
	lastTimeCShipsUpd	= RTDESKT_NOTIME;
	lastTimePlayerUpd	= RTDESKT_NOTIME;
	lastTimePlayer2Upd	= RTDESKT_NOTIME;
}

/**
* Performs the actions to destroy the game also as a 'Character'.
*/
void CSIGame::AI_Dye (void) 
{
	CCharacter::AI_Dye();
}


/*___________________________________________________________________
|                                                                    |
|                           LOAD DEVICES                             |
|____________________________________________________________________| 
*/

//////////////////////////////////////////////////////////////////////////////////////
//
// Load the devices
//
//
void CSIGame::LoadDevice(void)
{
	//HTML Reader
	CFile			HTMLFile;		//File to parser in the initialization
	CDeviceReader	DeviceReader;	//The parser used to understand the contents of the initialization file
	CLiteHTMLReader	HTMLReader;		//The real HTML reader
	char deviceFile	[STRINGMAXLENGTH],
		 deviceLog	[STRINGMAXLENGTH];

	strcpy(deviceFile,".\\Devices");
	strcpy(deviceLog,".\\Devices_");

	strcat(deviceFile, ".html");
	strcat(deviceLog, "Log.txt");

	// Initialization load
	DeviceReader.Init(deviceLog);
	HTMLReader.setEventHandler(&DeviceReader);//Assigning the initialization parser to the generic reader

	if (HTMLFile.Open(_T(deviceFile), CFile::modeRead))
	{
		HTMLReader.ReadFile((HANDLE) HTMLFile.m_hFile);
		HTMLFile.Close();
	}	
	return;
}


/*___________________________________________________________________
|                                                                    |
|                           LOAD LEVELS                              |
|____________________________________________________________________| 
*/

//////////////////////////////////////////////////////////////////////////////////////
//
// Load game levels
//
//
void CSIGame::LoadLevel(int lev)
{
	//HTML Reader
	CFile			HTMLFile;		//File to parser in the initialization
	CLevelReader	LevelReader;	//The parser used to understand the contents of the initialization file
	CLiteHTMLReader	HTMLReader;		//The real HTML reader

	char levelFile	[STRINGMAXLENGTH],
		 levelLog	[STRINGMAXLENGTH],
		 cLevel		[3];		// Until CSIG_MAX_LEVELS

	GenerateFullPathName (levelFile, Directories[CG_LEVEL_DIR], "Level_");
	strcpy(levelLog,levelFile);

	_itoa(lev, cLevel, 10);
	strcat(levelLog, cLevel);
	strcat(levelLog, "_Log.txt");

	strcat(levelFile, cLevel);
	strcat(levelFile, ".html");

	// Level load
	LevelReader.Init(levelLog);
	HTMLReader.setEventHandler(&LevelReader);//Assigning the initialization parser to the generic reader
	if (HTMLFile.Open(_T(levelFile), CFile::modeRead))
	{
		HTMLReader.ReadFile((HANDLE) HTMLFile.m_hFile);
		HTMLFile.Close();
	}
	
	#ifdef CHAR_USE_QUADTREE
	for(unsigned int i=0;i<BunkersManager.BunkerAvailable;i++){
		QuadtreeRoot::Instance()->getQuadtree()->AddBigCharacter(&BunkersManager.Bunker[i]);
	}
	#endif
	return;
}


void CSIGame::LoadLevelFile(int HtmlFileInd)
//filename without extension
{
	//HTML Reader
	CFile			HTMLFile;		//File to parser in the initialization
	CLevelReader	LevelReader;	//The parser used to understand the contents of the initialization file
	CLiteHTMLReader	HTMLReader;		//The real HTML reader

	char levelFile	[STRINGMAXLENGTH],
		 levelLog	[STRINGMAXLENGTH];

	strcpy(levelFile,UGKS_EMPTY_STRING);
	strcpy(levelLog,UGKS_EMPTY_STRING);

	strcpy(levelFile,	CSIG_GAME_ULEVEL_FILE);
	strcpy(levelLog,	CSIG_GAME_ULEVEL_FILE);

	strcat(levelLog, HtmlFiles[HtmlFileInd]);
	strcat(levelLog, "_Log.txt");

	strcat(levelFile, HtmlFiles[HtmlFileInd]);
	//strcat(levelFile, ".html");

	// Initialization load
	LevelReader.Init(levelLog);
	HTMLReader.setEventHandler(&LevelReader);//Assigning the initialization parser to the generic reader
	if (HTMLFile.Open(_T(levelFile), CFile::modeRead))
	{
		HTMLReader.ReadFile((HANDLE) HTMLFile.m_hFile);
		HTMLFile.Close();
	}
	
	for(unsigned int i=0;i<BunkersManager.BunkerAvailable;i++){
		QuadtreeRoot::Instance()->getQuadtree()->AddBigCharacter(&BunkersManager.Bunker[i]);
	}
	return;
}

void CSIGame::PromoteLevel(){
	if(!OnlyOneLevel){
		HiScore=Score;
		Level++;
		LevelReaded	= false;
		Navy.Init();

		if(Level<=5){
			//Reset of the differents sets
			ResetObjects();
		
			GameEvent(CSIG_NEXT_LEVEL);	//v Controlado 9->2
		}
		else{
			//END GAME WITH GAME WON
			GameEvent(CSIG_FADING2END);	//v Controlado 9->12
		}
	}
	else 
		//END GAME WITH GAME WON
		GameEvent(CSIG_FADING2END);	//v Controlado 9->12
}

//////////////////////////////////////////////////////////////////////////////////////
//
// Load saved game
//
//
void CSIGame::LoadGame (char * filename)
{
	//HTML Reader
	CFile			HTMLFile;		//File to parser in the initialization
	CLevelReader	LevelReader;	//The parser used to understand the contents of the initialization file
	CLiteHTMLReader	HTMLReader;		//The real HTML reader

	char levelLog	[32];

	strcpy(levelLog,filename);
	strcat(levelLog, "_Log.txt");
	
	// Initialization load
	LevelReader.Init(levelLog);
	HTMLReader.setEventHandler(&LevelReader);//Assigning the initialization parser to the generic reader
	
	if (HTMLFile.Open(_T(filename), CFile::modeRead))
	{
		HTMLReader.ReadFile((HANDLE) HTMLFile.m_hFile);
		HTMLFile.Close();
	}
}

void CSIGame::StartCameraMov()
{
	TimerManager.Timers[TmrApp].EndCounting();

	Game.intermede_sound	= true;

	float grados=0;
	Camera.Position = UGK::Bezier(control_points, 5, 0.5);
}

//AI methods
///Initializes the Game AI with the especified predefined FSM State 
void CSIGame::AI_Init(SIAI_AI_TYPE Type) 
{
	char AIPath[STRINGMAXLENGTH];

	//NEW CODE TO INITIALIZE FSM
	CFile					HTMLFile;	//File to parser in the initialization
	FSMReader				reader;		//The parser used to understand the contents of the FSM file
	CLiteHTMLReader			HTMLReader;	//The real HTML reader

	switch(Type)
	{
		case SIAI_GAME_DEFAULT:
		FSM *NewAI; ///<Declares the FSM to deliver to the AIManager 

		//INITIALIZE PARSER FILE
		GenerateFullAIName (AIPath, SIAI_SUPPLYSHIP_AI_LOG_FILE);

		reader.Init		(AIPath);
		reader.Title	= GetTitle();
		reader.Version	= GetVersion();

		//ADD FUNCTIONS TO BE USED BY THE FSM
		reader.addPublicFunction("NULL",NULL);
		reader.addPublicFunction("CSIG_Go2Play",(mFuncPtr)CSIG_Go2Play);

		HTMLReader.setEventHandler(&reader);
		GenerateFullAIName (AIPath, SIAI_GAME_AI_FILE);
		if (HTMLFile.Open(_T(AIPath), CFile::modeRead))
		{
			HTMLReader.ReadFile((HANDLE) HTMLFile.m_hFile);
			HTMLFile.Close();
		}

		NewAI = new FSM();
		NewAI->addStates(reader.getFSMStatesVector(),reader.getFSMNumStates()); ///<Setting the game FSM states
		NewAI->SetState(reader.getFSMInitState());  ///<Sets the game 'FSM' current state as the initial.

		AIManager.SetAI(NewAI,SIAI_GAME_DEFAULT);  ///<Informs the 'AIManager' about the 'FSM' that the game must have.
		if (NULL != AI)
			delete AI;
		AI = AIManager.GetAI(SIAI_GAME_DEFAULT);  ///<Asks the 'AIManager' for the 'FSM' corresponding to the game appropiately initialized.

		break;
		default:;
	}//Behaviour switch
}

/*___________________________________________________________________
|                                                                    |
|        D R A W  (D I S P L A Y  2 D / 3 D  E L E M E N T S)        |
|____________________________________________________________________| 
*/

void CSIGame::Render(void)
{
	#ifdef DEF_RND_TIME
	TimerManager.Timers[TmrRnd].InitCounting();
	#endif

	unsigned int i;
	static float position_player[] =		{-10.0, 20.0, 1.0, 0.0}; // for Player
	static float position_player2[] =		{-10.0, 20.0, 1.0, 0.0}; // for Player2
	static float position_ships[]  =		{-10.0, 10.0, 5.0, 0.0}; // for ships
	static float position_ships2[] =		{ 10.0, 0.0, -20.0, 0.0};// for ships

	switch(RenderMode)
	{
	case CHAR_NO_RENDER:
		return;
	case CHAR_2D:
	case CHAR_LINES3D:
	case CHAR_3D:
		UGKGLE_glResetError();
		Application.Window.SetRenderingContext();

		glClear  (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		Application.Window.Reshape(45.0f);

		Camera.SetLook();

		// CAMERA -> modification du Camera.zoom pendant le passage 2D->3D

		if (CSIG_PASSING_2_3D == AI->GetState())
			glTranslatef ( 0.0, 0.0, Camera.zoom_total_bonus);					// Camera.zoom pendant le passage à mode 3D
		else
			glTranslatef ( 0.0, 0.0, Camera.zoom_total );						// Camera.zoom utilisateur
		
		// CAMERA -> modification pendant l'intermède (rotation de la scène)
		//
		// la rotation de l'intermède ne se fait pas directement avec un angle incrémenté
		// pour éviter des opérations trop nombreuses sur la matrice MODEL_VIEW, mais sur
		// un angle global après la réinitialisation de la matrice par glLoadIdentity() (ci-dessus)
		glRotatef(intermede_angle_total, -0.3f, 0.55f, 1.0f);			// rotation de l'intermède -.3 .5 1.0

		// CAMERA -> modification utilisateur (clic et déplacement de la souris)
		glRotatef(Scene.Angle.v[YDIM], 1.0, 0.0, 0.0);								// rotation utilisateur (par rapport à l'axe des x)
		glRotatef(Scene.Angle.v[XDIM], 0.0, 1.0, 0.0);								// rotation utilisateur (par rapport à l'axe des y)

		// LUMIERES -> POSITION
		glLightfv(GL_LIGHT0, GL_POSITION, position_player);
		glLightfv(GL_LIGHT1, GL_POSITION, position_player2);
		glLightfv(GL_LIGHT2, GL_POSITION, position_ships);
		glLightfv(GL_LIGHT3, GL_POSITION, position_ships2);

		
		// Background	
		GUI.display_background(false);										

		//********************************** Start Render with Blending *************************
		glEnable(GL_BLEND);		
		// Display Player Health
		GUI.display_Health();
		// Number of lives
		GUI.display_below_lives();
		// Score, Hiscore, Lives
		GUI.display_score_hiscore_lives();									

		//Shoots
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
		if(!DiscreteSimulation) ShootsManager.Render();
		else DiscreteShootsManager.Render();

		//Bonus
		if(DiscreteSimulation) DiscreteBonusManager.Render();
		else BonusManager.Render();

		//Ship Explosions
		for (i=0;i<Navy.Ship.size(); i++){
			if(Navy.Ship[i]->Explosion.Alive && Navy.Ship[i]->Explosion.Active){
				#ifdef DEF_RND_TIME
				EndAccCounting(TmrRnd);
				#endif
				#ifdef DEF_RND_TIME
				Game.TimerManager.Timers[TmrUpd].InitCounting();
				#endif

				Navy.Ship[i]->Explosion.Update();

				#ifdef DEF_RND_TIME
				EndAccCounting(TmrUpd);
				#endif
				#ifdef DEF_RND_TIME
				Game.TimerManager.Timers[TmrRnd].InitCounting();
				#endif

				Navy.Ship[i]->Explosion.Render();
			}
		}
		//SupplyShip Explosions
		for(i=0;i<Navy.SupplyShip.size();i++){
			if(Navy.SupplyShip[i]->Explosion.Alive && Navy.SupplyShip[i]->Explosion.Active){
				#ifdef DEF_RND_TIME
				EndAccCounting(TmrRnd);
				#endif
				#ifdef DEF_RND_TIME
				Game.TimerManager.Timers[TmrUpd].InitCounting();
				#endif

				Navy.SupplyShip[i]->Explosion.Update();

				#ifdef DEF_RND_TIME
				EndAccCounting(TmrUpd);
				#endif
				#ifdef DEF_RND_TIME
				Game.TimerManager.Timers[TmrRnd].InitCounting();
				#endif
				Navy.SupplyShip[i]->Explosion.Render();
			}
		}
		//CircleShip Explosions
		for(i=0;i<Navy.CircleShip.size();i++){
			if(Navy.CircleShip[i]->Explosion.Alive && Navy.CircleShip[i]->Explosion.Active){
				#ifdef DEF_RND_TIME
				EndAccCounting(TmrRnd);
				#endif
				#ifdef DEF_RND_TIME
				Game.TimerManager.Timers[TmrUpd].InitCounting();
				#endif

				Navy.CircleShip[i]->Explosion.Update();

				#ifdef DEF_RND_TIME
				EndAccCounting(TmrUpd);
				#endif
				#ifdef DEF_RND_TIME
				Game.TimerManager.Timers[TmrRnd].InitCounting();
				#endif

				Navy.CircleShip[i]->Explosion.Render();
			}
		}
		//Player Explosions
		for(i=0;i<CP_MAX_PLAYERS;i++){
			if(Player[i].Explosion.Alive && Player[i].Explosion.Active){
				#ifdef DEF_RND_TIME
				EndAccCounting(TmrRnd);
				#endif
				#ifdef DEF_RND_TIME
				TimerManager.Timers[TmrUpd].InitCounting();
				#endif

				Player[i].Explosion.Update();

				#ifdef DEF_RND_TIME
				EndAccCounting(TmrUpd);
				#endif
				#ifdef DEF_RND_TIME
				TimerManager.Timers[TmrRnd].InitCounting();
				#endif

				Player[i].Explosion.Render();
				if(!Player[i].Explosion.Alive && Player[i].Lives>0)
					Player[i].Active = Player[i].Alive = true;
			}
		}

		if(passing_2D_2_3D) GUI.display_ring_2d_to_3d();

		glDisable(GL_BLEND);
		//********************************** Finish Render with Blending *************************

		// 3D WRAPPER
		if (CHAR_2D != Game.RenderMode)
		{
			//glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
			glEnable(GL_CULL_FACE);		// on n'affiche pas les faces cachées
			glFrontFace(GL_CCW);		// les faces frontales (non cachées) sont dans le sens CCW
			glEnable(GL_DEPTH_TEST);	// active le test de profondeur (les faces situées derrière d'autres ne seront pas affichées)
		
			glEnable(GL_LIGHTING);		// active les lumières
			glEnable(GL_LIGHT0);		// allume la lumière Player
		}

		//Players.
		for(i=0;i<CP_MAX_PLAYERS;i++)
			if(Player[i].Alive)
				Player[i].Render();	

		//Players.
		//for(i=0;i<CP_MAX_PLAYERS;i++)
			if(Player2[0].Alive)
				Player2[0].Render();	

		// LUMIERE DES SHIPS (DEBUT) ET PLAYER (FIN)
		if (CHAR_2D != Game.RenderMode)
		{
			glEnable(GL_LIGHTING);	// active les lumières
			glDisable(GL_LIGHT0);	// éteint la lumière Player
			glEnable(GL_LIGHT1);	// allume la lumière ships
			glEnable(GL_LIGHT2);	// allume la lumière ships
		}else 
			glEnable(GL_BLEND);

		Navy.Render();
		
		if (CHAR_2D != Game.RenderMode)
		{
			/// End of SHIPS Lights
			glDisable(GL_LIGHT1);
			glDisable(GL_LIGHT2);
			glDisable(GL_LIGHTING);

			/// End 3D WRAPPER
			glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
			glDisable(GL_DEPTH_TEST);		// désactive le test de profondeur
			glEnable(GL_BLEND);				// active le mélange de couleurs (ici transparence)
			glDisable(GL_CULL_FACE);		// on affiche les faces "cachées" (le dos des éléments en mode 2D)
		}else 
			glDisable(GL_BLEND);


		//Bunkers
		BunkersManager.Render();

		//Quadtree
		//QuadtreeRoot::Instance()->Render();

		//Measures calculation
		info.calcFpsPct();

		//Draw Info Texts
		if(!DiscreteSimulation){
			sprintf(texto,"FPS %.2f",info.fps);
			txtFPS.text2D_draw(10, 14, texto, initialWidthW, initialHeightW, yelow);

			sprintf(texto,"Rnd %.2f%%",info.pctRnd);
			txtRND.text2D_draw(10, 468, texto, initialWidthW, initialHeightW, yelow);
	
			sprintf(texto,"Upd %.2f%%",info.pctUpd);
			txtUPD.text2D_draw(275, 468, texto, initialWidthW, initialHeightW, yelow);
	
			sprintf(texto,"Idl %.2f%%",info.pctIdl);
			txtIDL.text2D_draw(550, 468, texto, initialWidthW, initialHeightW, yelow);
	
			sprintf(texto,"Suma %% %.2f",info.pctSuma);
			txtSUM.text2D_draw(475, 14, texto, initialWidthW, initialHeightW, yelow);
		}
		else{
			sprintf(texto,"FPS %.2f",info.fps);
			txtFPS.text2D_draw(10, 14, texto, initialWidthW, initialHeightW, yelow);

			sprintf(texto,"Rnd %.2f%%",info.pctRnd);
			txtRND.text2D_draw(10, 468, texto, initialWidthW, initialHeightW, yelow);
	
			sprintf(texto,"Upd %.2f%%",info.pctUpd);
			txtUPD.text2D_draw(180, 468, texto, initialWidthW, initialHeightW, yelow);

			sprintf(texto,"Rtdsk %.2f%%",info.pctRtd);
			txtIDL.text2D_draw(340, 468, texto, initialWidthW, initialHeightW, yelow);
	
			sprintf(texto,"Idl %.2f%%",info.pctIdl);
			txtIDL.text2D_draw(520, 468, texto, initialWidthW, initialHeightW, yelow);
	
			sprintf(texto,"Suma %% %.2f",info.pctSuma);
			txtSUM.text2D_draw(475, 14, texto, initialWidthW, initialHeightW, yelow);
		}

		Application.Window.Refresh();
		break;
	default: return;
	}
	#ifdef DEF_RND_TIME
	EndAccCounting(TmrRnd);
	#endif
}

void CSIGame::ChangeRenderMode(CHAR_RENDER_MODE Mode)
{
	unsigned int i;

	RenderMode = Mode;
	
	switch(RenderMode)
	{
	case CHAR_NO_RENDER:
		break;
	case CHAR_2D:
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		break;
	case CHAR_LINES3D:
	case CHAR_3D:
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);	//ACHTUNG: Verify this is OK
		break;
	default: return;
	}

	//Transfer the event to every character in the world
	//Enemy navy
	Navy.ChangeRenderMode(RenderMode);

	//Players
	for(i=0;i<CP_MAX_PLAYERS;i++)	Player[i].ChangeRenderMode(RenderMode);

	//Bonus
	BonusManager.ChangeRenderMode(RenderMode);
	DiscreteBonusManager.ChangeRenderMode(RenderMode);

	//Bunkers
	BunkersManager.ChangeRenderMode(RenderMode);

	//Shoots
	ShootsManager.ChangeRenderMode(RenderMode);
	DiscreteShootsManager.ChangeRenderMode(RenderMode);
}

void CSIGame::Update ()		//Update the world in continuous simulation.
{
	#ifdef DEF_UPD_TIME
	TimerManager.Timers[TmrUpd].InitCounting();
	#endif

	unsigned int i;
	
	Log.Add("Update Game");

	//***********Updating Managers***************
	//Bonus Manager
	BonusManager.Maintenance();

	//Bunkers Manager
	//BunkersManager.Update();

	//Shoots Manager
	ShootsManager.Maintenance();

	SIKeyboard.Update();
	Application.Update();

	//***********Checking for Win*************
	//Test if there exist any enemy	
	bool Won = true;
	for (i=0;i<Navy.Ship.size();i++)										// pour tous les ships...
		//Debugging: The next line throws the exception.
		if (Navy.Ship[i]->Alive)										// si il reste encore au moins 1 ship vivant...
		{												// idem pour le super ship
			Won = false;
			break;
		} // for - if
		
	if (Won)
		for(i=0;i<Navy.SupplyShip.size();i++)
			if (Navy.SupplyShip[i]->Alive)
			{												// idem pour le super ship
				Won = false;
				break;
			} // for - if

	if (Won)
		for(i=0;i<Navy.CircleShip.size();i++)
			if (Navy.CircleShip[i]->Alive)
			{												// idem pour le super ship
				Won = false;
				break;
			} // for - if

	if (Won){
		Game.NextRenderMode = Game.RenderMode;
		GameEvent(CSIG_FADING2WON);  //v Controlado 4->11
		ScoresManager.SetHiscore(Score);
	}

	#ifdef DEF_UPD_TIME
	EndAccCounting(TmrUpd);
	#endif
}

void CSIGame::DiscreteUpdate ()		//Update the world. Similar to render() but updating
{
	#ifdef DEF_UPD_TIME
	TimerManager.Timers[TmrUpd].InitCounting();
	#endif

	unsigned int i;
	Log.Add("Update Game");

	//***********Updating Managers***************
	//Bonus Manager
	DiscreteBonusManager.Maintenance();

	//Shoots Manager
	DiscreteShootsManager.Maintenance();

	SIKeyboard.Update();
	Application.Update();

	//Bunkers Manager
	//BunkersManager.Update();

	#ifdef DEF_EXP_CSV_FILE
	//***********Checking for end time***************
	switch (WayToEnd)
	{
	case END_BY_TIME:
		if(DEF_MAX_TICKS < TimerManager.Timers[TmrApp].TicksUntilNow())
			endByTime = true;
		break;
	case END_BY_FRAME:
		if(DEF_END_IN_FRAME <= info.AllnFrames)
			endByFrame = true;
		break;
	}
	#endif
	
	
	//***********Checking for Win***************
	//Test if there exist any enemy	
	bool Won = true;
	for (i=0;i<Navy.Ship.size();i++)										// pour tous les ships...
		//Debugging: The next line throws the exception.
		if (Navy.Ship[i]->Alive)										// si il reste encore au moins 1 ship vivant...
		{												// idem pour le super ship
			Won = false;
			break;
		} // for - if
		
	if (Won)
		for(i=0;i<Navy.SupplyShip.size();i++)
			if (Navy.SupplyShip[i]->Alive)
			{												// idem pour le super ship
				Won = false;
				break;
			} // for - if

	if (Won)
		for(i=0;i<Navy.CircleShip.size();i++)
			if (Navy.CircleShip[i]->Alive)
			{												// idem pour le super ship
				Won = false;
				break;
			} // for - if

	if (Won){
		Game.NextRenderMode = Game.RenderMode;
		GameEvent(CSIG_FADING2WON);  //v Controlado 4->11
	}

	#ifdef DEF_UPD_TIME
	EndAccCounting(TmrUpd);
	#endif
}

/*___________________________________________________________________
|                                                                    |
|                           INITIALISATION                           |
|____________________________________________________________________| 
*/

//////////////////////////////////////////////////////////////////////////////////////
//
// Game Initialization
//

/**   
	* @fn DWORD WINAPI CSIG_ClearManagers(LPVOID lpParam) 
	* Thread for clearing all the manager internal queues
*/
DWORD WINAPI CSIG_ClearManagers(LPVOID lpParam) 
{
	int i;

	Log.Add("Clearing application Managers...");

	//Clearing application Managers
	Navy.Ship.clear();
	Navy.SupplyShip.clear();
	Navy.CircleShip.clear();
	TexturesManager.Textures.clear();
	MeshesManager.Meshes.clear();
	AnimationsManager.Animations.clear();
	SoundsManager.Sound.clear();
	ShootsManager.ShootsAct.clear();
	DiscreteShootsManager.ShootsAct.clear();

	for (i=0;i<CBN_MAX_BONUSES;i++){
		BonusManager.Bonus[i].Init();
		DiscreteBonusManager.Bonus[i].Init();
	}
	return NULL;
}

/**   
	* @fn DWORD WINAPI CSIG_InitGraphics(LPVOID lpParam) 
	* Thread for initializing all the OpenGL internal values
*/
void CSIGame::InitGraphics() 
{
	Log.Add("initializing all the OpenGL internal values...");

	if (!Application.Window.FullScreen && !Application.Window.quickFullScreen)
		if (Application.Window.Width < GraphicsCard.DeskTop.Width && Application.Window.Height < GraphicsCard.DeskTop.Height) // on ne centre que si la fenêtre est + petite que le bureau
	{
		Application.Window.LeftUpX = (GraphicsCard.DeskTop.Width - Application.Window.Width) / 2;
		Application.Window.LeftUpY = (GraphicsCard.DeskTop.Height - Application.Window.Height) / 2;
	}

	Application.Window.SetDeskTop(&GraphicsCard.DeskTop);
	Application.Window.Create();

	// OPENGL -> INITIALISATION
	glClearColor (0.0f, 0.0f, 0.0f, 0.0f);						// Black Background
	glClearDepth (1.0f);										// Depth Buffer Setup
	glEnable(GL_TEXTURE_2D);									// Enable Texture Mapping
	glDisable(GL_CULL_FACE);									// Front and Back faces of polygons are drawn

	glDepthFunc(GL_LEQUAL);										// Define Depth
	glDisable(GL_DEPTH_TEST);									// No depth test

	glBlendFunc(GL_SRC_ALPHA,GL_ONE);							// Define Alpha Blending
	glEnable(GL_BLEND);											// Enable Alpha Blending
	
	// OPENGL -> QUALITÉ
	glShadeModel (GL_SMOOTH);									// Smooth Shading for lines and polygons (for 3D objects)
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			// Hint for perspective calculations (most accurate)
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);						// Hint for lines rasterization (progress bar, 3D objects in Lines3D mode...)
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);					// Hint for polygon rasterization (3D objects...)

	// OPENGL -> Lights (Only for 3D MODE)

	static float ambient[]			=	{0.9, 0.9, 0.9, 1.0};
	static float diffuse[]			=	{0.25, 0.25, 0.25, 1.0};
	static float specular[]			=	{1.0, 1.0, 1.0, 1.0};
	
	static float ambient2[]			=	{0.2, 0.2, 0.2, 1.0};
	static float diffuse2[]			=	{0.05, 0.05, 0.05, 0.05};

	static float lmodel_ambient[]	=	{0.1, 0.1, 0.1, 1.0};
	static float lmodel_localviewer[] =	{1.0};

	static float mat_shininess[]	=	{10.0};
	static float mat_specular[]		=	{0.8, 0.8, 0.8, 1.0};
	static float mat_ambient[]		=	{0.6, 0.6, 0.6, 1.0};
	static float mat_diffuse[]		=	{0.4, 0.4, 0.4, 1.0};

	static float spot_direction[] =		{0.0, -1.0, 0.0};

	// lumière n°0, pour le vaisseau du joueur (mode 3D)
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	// lumière n°1, pour les vaisseaux enemis (mode 3D)
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	// lumière n°2, pour les vaisseaux enemis (mode 3D)
	glLightfv(GL_LIGHT2, GL_AMBIENT, ambient2);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuse2);

	// lumière (réfléchie sur les matériaux)
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);

	// PASSAGE 2D à 3D -> camera path, courbe de Bézier (mouvement de caméra pour le passage au mode 3D)
	Camera.Position.v[XDIM]  =  0; Camera.Position.v[YDIM] = 0; Camera.Position.v[ZDIM]  = 20;
	control_points[0].v[XDIM] =  0; control_points[0].v[YDIM] = 0; control_points[0].v[ZDIM] = 20;
	control_points[1].v[XDIM] = -8; control_points[1].v[YDIM] = 0; control_points[1].v[ZDIM] = 18;//15
	control_points[2].v[XDIM] = -2; control_points[2].v[YDIM] = 0; control_points[2].v[ZDIM] =  5;//5
	control_points[3].v[XDIM] =  2; control_points[3].v[YDIM] = 0; control_points[3].v[ZDIM] = -7;//3, -2
	control_points[4].v[XDIM] =  5; control_points[4].v[YDIM] = 0; control_points[4].v[ZDIM] = 18;//15
	control_points[5].v[XDIM] =  0; control_points[5].v[YDIM] = 0; control_points[5].v[ZDIM] = 20;

	// SPHERE
	quadratic=gluNewQuadric();							// Create A Pointer To The Quadric Object (Return 0 If No Memory)
	gluQuadricNormals(quadratic, GLU_SMOOTH);			// Create Smooth Normals 
	gluQuadricTexture(quadratic, GL_TRUE);				// Create Texture Coords 

	// LOADING -> vue orthogonale 2D
	Application.Window.Reshape(90.0f);
	Camera.Reshape(10.0f);
}

/**   
	* @fn DWORD WINAPI CSIG_InitSounds(LPVOID lpParam) 
	* Thread for initializing all the sounds internal values
*/
DWORD WINAPI CSIG_InitSounds(LPVOID lpParam) 
{
	//char *SoundError;
//	int  FilesFormatSupported;

	Log.Add(SIGLB_GetText(TR_GAME_LOADING_SOUNDS));	//Loading Sounds...

	Game.Spectrum = false;

	// Load the sound manager
	SoundSystem.Init();
	SoundsManager.Init();
	SoundsManager.SetMusicPath(Game.Directories[CG_MUSIC_DIR]);
	SoundsManager.SetTrackPath(Game.Directories[CG_SOUNDS_DIR]);
	//FilesFormatSupported = SoundsManager.SupportFiles(CSM_ASK_SOUND_FILES);
	/*if (0 == FilesFormatSupported)
	{			
		MessageBox(0, Mix_GetError(), "Sound Initialization Error", MB_TOPMOST);
		Mix_CloseAudio();
	}
	else SoundError = SoundsManager.GetErrorMsg();*/

	// Load the sound effects
	UGKSND_SoundType Sound = NULL;	
	//Sound = Mix_LoadWAV(".\\sounds\\cross.ogg");
	/*Sound = Mix_LoadWAV(".\\sounds\\cross.wav");
	SoundError = SDL_GetError();
	*/
	SoundsManager.LoadSounds(CGS_MAX_SOUND, CGS_SoundsFiles);

	return NULL;
}

/**   
	* @fn bool CSIGame::Initialize (void)
	* Any GL Init Code & User Initialization Goes Here
*/
bool CSIGame::Initialize (void)
{
	HANDLE  hThread;
	DWORD   dwThreadId;

	//HTML Reader
	CFile					HTMLFile;		//File to parser in the initialization
	CInitializationReader	InitReader;		//The parser used to understand the contents of the initialization file
	CLiteHTMLReader			HTMLReader;		//The real HTML reader

	// Language reader
	CLanguageReader			LanguageReader(CA_DEFAULT_VERSION,CA_DEFAULT_TITULO); 

	GLenum Error;
	char temp[STRINGMAXLENGTH];

	InitGraphics();
	InitCharFactory();

	//Creates a thread in order to speed up the initialization of the program
	hThread = CreateThread( 
		NULL,               // default security attributes
		0,                  // use default stack size  
		CSIG_ClearManagers,		    // thread function name
		NULL,				// argument to thread function 
		0,                  // use default creation flags 
		&dwThreadId);		// returns the thread identifier 

	//Verifies that everything is fine
	if (NULL == dwThreadId) 
		CSIG_ClearManagers(NULL);	//Do it blocking. HW does not allow threads

	ProgressBar = false;

	// Generating VERSION
	strcpy(temp, GetTitle());
	strcat(temp, " v");
	strcat(temp, GetVersion());
	Log.Add(temp);

	// BUILD DATE
	strcpy(temp, "Date: ");
	strcat(temp, __DATE__);
	strcat(temp, ", ");
	strcat(temp, __TIME__);
	Log.Add(temp);
	Log.Add(UGKS_EMPTY_STRING);

		//Inicializar variables globales antes de leer el parser de inicialización (por si no están los valores includios)
	Navy.Title		= GetTitle();
	Navy.Version	= GetVersion();

	BunkersManager.BunkersAmount = 4;	// Default amount of Bunkers

	LoadingBMP	= true;
	ProgressBar	= true;

	Log.Add("Reading initialization file...");

	//Initialization load
	InitReader.Init(CSIG_INITIALIZATION_LOG_FILE);
	HTMLReader.setEventHandler(&InitReader);//Assigning the initialization parser to the generic reader
	if (HTMLFile.Open(_T(CSIG_INITIALIZATION_FILE), CFile::modeRead)) 
	{
		HTMLReader.ReadFile((HANDLE) HTMLFile.m_hFile);
		HTMLFile.Close();
	}

	//Game AI initialization
	AI_Init (SIAI_GAME_DEFAULT);	//<AI initialization by default

	//FSM initialization for Ships, SupplyShips and CircleShips.
	Navy.AI_Init();

	// Loading characters (fonts) images
	strcpy(temp, CSIG_DefaultDirectories [CG_TEXTURE_DIR]);
	strcat(temp, UGKOS_DIRECTORY_SEPARATOR);
	strcat(temp, "font.tga");
	fontLoad (temp);

	Application.LoadCurrentLanguage();

	for(i=0;i<CP_MAX_PLAYERS;i++)
		Player[i] = defaultPlayer;

	StartAnimations();

	//Creates a thread in order to speed up the loading of the sounds
	hThread = CreateThread( 
		NULL,               // default security attributes
		0,                  // use default stack size  
		CSIG_InitSounds,    // thread function name
		NULL,				// argument to thread function 
		0,                  // use default creation flags 
		&dwThreadId);		// returns the thread identifier 

	//Verifies that everything is fine
	if (NULL == dwThreadId) 
		CSIG_InitSounds(NULL);	//Do it blocking. HW does not allow threads

	/*Reads the particular configuration of level 0*/
	if(ReadLevel)
	{   //Level reading is activated
		if(!LevelReaded)
		{ //If the level is not read

			bool	loadSavedGame	= false,
					SavedGame		= false;

			/* Checks if there is any saved game */
			if(FILE *f = fopen(CSIG_SAVED_GAME,"r"))
			{
				SavedGame = true;
				fclose(f);

				// Message Box to decide if the saved game is loaded or not
				// The buttons language is the system defaults
				int msgBox = MessageBox(NULL,
										TEXT("A saved game has been detected. Would you like to load this?"),
										TEXT("Load Game"),
										MB_ICONASTERISK | MB_OKCANCEL );

				if(msgBox == IDOK) // The saved game will be loaded
					loadSavedGame = true;
			}

			/* To load or not to load. That is the question */
			if(loadSavedGame){
				// Load saved game
				Log.Add(SIGLB_GetText(TR_GAME_READ_SAVED_GAME));
				LoadGame(CSIG_SAVED_GAME);
				LevelReaded	= true;
			}

			Log.Add("Shoots...");
			ShootsManager.Init();
			DiscreteShootsManager.Init();
				
			// Set Score to zero
			Game.Score = 0;

			//Delete saved game. Do not ask again if you want to play the saved game
			if (SavedGame)
			{
				 if( remove( CSIG_SAVED_GAME ) != 0 )
					 Log.Add("Error deleting SAVED game");
				 else
					 Log.Add("SAVED game successfully deleted");
			}

			// Load HiScore
			Game.HiScore = ScoresManager.GetHiscore();
		}
	}

	if(Game.NextRenderMode != Game.RenderMode)
		Game.ChangeRenderMode(Game.NextRenderMode);

	Log.Add("Environment variables...");

	// GAME OPTION -> SYNCHRO VERTICALE (VSYNC) SI DISPONIBLE
	typedef BOOL (WINAPI *PFNWGLSWAPINTERVALEXTPROC)(int interval);
	PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = NULL;
	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
	wglSwapIntervalEXT = ( BOOL (WINAPI *)(int)) wglGetProcAddress( "wglSwapIntervalEXT" );
	Error = glGetError();
	if (wglSwapIntervalEXT)
		if (Application.Window.Vsync)
		{
			wglSwapIntervalEXT(1);
			Application.Window.VsyncActive = true;
		}
		else
		{
			wglSwapIntervalEXT(0);
			Application.Window.VsyncActive = false;
		}
	Error = glGetError();
	// MATHS -> Randomize function INITIALISATION
	//srand((unsigned)timeGetTime());
	srand(1234);

//RT-Desk engine initializatión. RT-Desk is a Discrete Decoupled Simulation Tool.
	if(LevelReaded)
	{
		LoadRTDESK();
		LauncherRTDESK();
	}


	//Initialization Timers
	InitTimers();

	//Space Invaders Keyboard input initialization. Do the nest three instructions in the given order.
	SIKeyboard.Init();
	SIKeyboard.SetActionKeys	(CSIK_ActionKeys, CSIK_MAX_KBD_ACTION);
	SIKeyboard.SetDelay			(CSIK_ActionsDelay);

	return TRUE;
}

/**   
	* @fn void CSIGame::InitCharFactory()
	* Initializes explicitly all the information the factory requires to produce any kind of characters in this video game
*/
void CSIGame::InitCharFactory()
{
	CCFC_Character Character;
	
	Character.Id	= CHARS_BONUS;				///<For improving player strenght
	Character.Name	= CHARS_Tags[CHARS_BONUS];
	Character.Size	= sizeof(CBonus);
	CharacterFactory.SetCharacter(Character);

	Character.Id	= CHARS_BRICK;				///<Pieces that make a bunker
	Character.Name	= CHARS_Tags[CHARS_BRICK];
	Character.Size	= sizeof(CBrick);
	CharacterFactory.SetCharacter(Character);

	Character.Id	= CHARS_BUNKER;			///<A collection of bricks together in order to defend the player
	Character.Name	= CHARS_Tags[CHARS_BUNKER];
	Character.Size	= sizeof(CBunker);
	CharacterFactory.SetCharacter(Character);

	Character.Id	= CHARS_BUNKERSMANAGER;	///<The manager of all the bunkers available in a given play
	Character.Name	= CHARS_Tags[CHARS_BUNKERSMANAGER];
	Character.Size	= sizeof(CBunkersManager);
	CharacterFactory.SetCharacter(Character);

	Character.Id	= CHARS_CIRCLESHIP;		///<Alien space ships that turns once and once again in clircles on the screen
	Character.Name	= CHARS_Tags[CHARS_CIRCLESHIP];
	Character.Size	= sizeof(CCircleShip);
	CharacterFactory.SetCharacter(Character);

	Character.Id	= CHARS_GAME;				///<The game itself 
	Character.Name	= CHARS_Tags[CHARS_GAME];
	Character.Size	= sizeof(CSIGame);
	CharacterFactory.SetCharacter(Character);

	Character.Id	= CHARS_LASER;				///<Additional shooting left or right device for the player
	Character.Name	= CHARS_Tags[CHARS_LASER];
	Character.Size	= sizeof(CLaser);
	CharacterFactory.SetCharacter(Character);

	Character.Id	= CHARS_MISSIL;			///<A common shooting from any ship, supplyship or player
	Character.Name	= CHARS_Tags[CHARS_MISSIL];
	Character.Size	= sizeof(CShoot);
	CharacterFactory.SetCharacter(Character);

	Character.Id	= CHARS_NAVY;				///<Holds all the supplyships and all types of ships
	Character.Name	= CHARS_Tags[CHARS_NAVY];
	Character.Size	= sizeof(CNavy);
	CharacterFactory.SetCharacter(Character);

	Character.Id	= CHARS_PLAYER;			///<The player
	Character.Name	= CHARS_Tags[CHARS_PLAYER];
	Character.Size	= sizeof(CPlayer);
	CharacterFactory.SetCharacter(Character);
	
	Character.Id	= CHARS_PLAYER2;			///<The player2
	Character.Name	= CHARS_Tags[CHARS_PLAYER2];
	Character.Size	= sizeof(CPlayer2);
	CharacterFactory.SetCharacter(Character);

	Character.Id	= CHARS_PLAYER_SHOOT;		///<Player shoots
	Character.Name	= CHARS_Tags[CHARS_PLAYER_SHOOT];
	Character.Size	= sizeof(CShoot);
	CharacterFactory.SetCharacter(Character);

	Character.Id = CHARS_REACTOR;			///<Extra power engines and shooters for the player. Obtained after the corresponding bonus has been won
	Character.Name	= CHARS_Tags[CHARS_REACTOR];
	Character.Size	= sizeof(CReactor);
	CharacterFactory.SetCharacter(Character);

	Character.Id = CHARS_SHIP;				///<Alien normal space ships
	Character.Name	= CHARS_Tags[CHARS_SHIP];
	Character.Size	= sizeof(CShip);
	CharacterFactory.SetCharacter(Character);

	Character.Id	= CHARS_SHIP_SHOOT;		///<Alien normal space ships shoots
	Character.Name	= CHARS_Tags[CHARS_SHIP_SHOOT];
	Character.Size	= sizeof(CShoot);
	CharacterFactory.SetCharacter(Character);

	Character.Id	= CHARS_SPHERE_OGIVE;		///<Environment
	Character.Name	= CHARS_Tags[CHARS_SPHERE_OGIVE];
	Character.Size	= sizeof(CCharacter);
	CharacterFactory.SetCharacter(Character);

	Character.Id	= CHARS_SUPPLYSHIP;		///<Super ship of the alien troops
	Character.Name	= CHARS_Tags[CHARS_SUPPLYSHIP];
	Character.Size	= sizeof(CSupplyShip);
	CharacterFactory.SetCharacter(Character);

	Character.Id	= CHARS_WEAPON;
	Character.Name	= CHARS_Tags[CHARS_WEAPON];
	Character.Size	= sizeof(CShoot);
	CharacterFactory.SetCharacter(Character);
}

/// Called at the end of the game when cleaning all the game data
void CSIGame::Deinitialize (void)
{
	ScoresManager.SetHiscore(Game.Score);

	#ifdef UGKSND_FMOD375 
			SoundsManager.MusicFadingOut();
	#elif UGKSND_SDL			
		// Stop the music
		SoundsManager.StopMusic();
		SoundsManager.CloseMixer();			
	#endif

	ResetObjects();

	LevelReaded = false;
	Level = 0;
	StopAfterOneLevel = false;

	RTDESK_Engine->Reset();

	lastTimeRnd			= RTDESKT_NOTIME;
	lastTimeUpd			= RTDESKT_NOTIME;
	lastTimeShipsUpd	= RTDESKT_NOTIME;
	lastTimeSShipsUpd	= RTDESKT_NOTIME;
	lastTimeCShipsUpd	= RTDESKT_NOTIME;
	lastTimePlayerUpd	= RTDESKT_NOTIME;

	//write to file the Time.fps average
	FILE *f;
	f=fopen("log_avgfps.txt","w+");
	char buffer[CGUI_MAX_CONSOLE_LENGTH];
	if (f){
		fwrite("*** Execution with FSM ****\n",sizeof(char),32,f);
		strcpy(buffer,UGKS_EMPTY_STRING);
		//sprintf(buffer,"Average Time.fps: %2.2f\n",Time.fpsavr);
		fwrite(buffer,sizeof(char),strlen(buffer),f);
	}
	fclose(f);
}

void CSIGame::ResetObjects(void)
{
	Navy.CurrentCircleShip=0;
	Navy.CircleShip.clear();
	Navy.CurrentShip=0;
	Navy.Ship.clear();
	Navy.CurrentSupplyShip=0;
	Navy.SupplyShip.clear();

	Navy.ShootsFrequency = Level;

	ShootsManager.ShootsAct.clear();
	ShootsManager.ShootsInact.clear();
	DiscreteShootsManager.ShootsAct.clear();
	DiscreteShootsManager.ShootsInact.clear();

	Game.TimerManager.Timers.clear();

	BunkersManager.BunkerAvailable=0;
	BunkersManager.BunkersAmount=CBM_MAX_BUNKERS;
	BunkersManager.DefaultBunker	= CBNK_HOME;	
	for(unsigned int i=0; i<CBM_MAX_BUNKERS; i++){
		BunkersManager.Bunker[i].Init();
		BunkersManager.Bunker[i].Active = BunkersManager.Bunker[i].Alive = false;
	}

	for(unsigned int i=0; i<CBN_MAX_BONUSES; i++){
		BonusManager.Bonus[i].Init();
		DiscreteBonusManager.Bonus[i].Init();
	}

	endByTime = false;
	endByFrame = false;

	// Load HiScore
	//Game.HiScore = ScoresManager.GetHiscore();

	#ifdef QT_USE_QUADTREE
	QuadtreeRoot::Instance()->getQuadtree()->Clear();
	#endif
}

 void CSIGame::Save (char * fileName)			//Saves the game to fileName - Rodrigo Aldecoa
{
	FILE *f;
	f=fopen(fileName,"w+");
	if(f){

		// HEADER
		char *header = new char[500];
		sprintf(header, "<HTML>\n\t<HEAD>\n\t\t<TITLE>%s</TITLE>\n", Game.GetTitle());
		sprintf(header, "%s\t\t<VERSION>%s</VERSION>\n", header, Game.GetVersion());
		
		//char *cLevel = new char[3];
		//_itoa(Game.Level, cLevel, 10);
		sprintf(header, "%s\t\t<TYPE>Level</TYPE>\n\t\t<LEVEL>%i</LEVEL>\n", header, Game.Level);

		//char *cScore = new char[10];
		//_itoa(score, cScore, 10);
		sprintf(header, "%s\t\t<SCORE>%i</SCORE>\n", header, Game.Score);
		
		strcat(header, "\t</HEAD>\n");
		fwrite(header, sizeof(char), strlen(header), f);

		//// BODY
		char *body = new char[128];
		sprintf(body, "\t<BODY>\n");
		fwrite(body, sizeof(char), strlen(body), f); 
		
		/// SAVE PLAYERS
		for(int i=0; i<CP_MAX_PLAYERS; i++){
			if(Player[i].Alive){
				char *player = SaveCharacter(i, CHARS_PLAYER); // If the player is alive, save its parameters
				fwrite(player, sizeof(char), strlen(player), f); // Print player to file
			}
		}

		/// SAVE NAVY

		// Save Supplyships
		for(unsigned int i=0; i<Game.SSxLEV; i++){
			if(Navy.SupplyShip[i]->Alive){
				char *sShip = SaveCharacter(i, CHARS_SUPPLYSHIP); // If the SupplyShip is alive, save its parameters
				fwrite(sShip, sizeof(char), strlen(sShip), f); // Print SupplyShip to file
			}
		}

		// Save Ships
		for(int i=0; i<CN_MAX_SHIPS; i++){
			if(Navy.Ship[i]->Alive){
				char *ship = SaveCharacter(i, CHARS_SHIP); // If the SupplyShip is alive, save its parameters
				fwrite(ship, sizeof(char), strlen(ship), f); // Print Ship to file
			}
		}

		/// SAVE BUNKERS
		for(int i=0; i<CBM_MAX_BUNKERS; i++){
			if(BunkersManager.Bunker[i].Alive){
				char *bunker = SaveCharacter(i, CHARS_BUNKER); // If the Bunker is alive, save its parameters
				fwrite(bunker, sizeof(char), strlen(bunker), f); // Print Bunker to file
			}
		}

		/// END
		char *end = new char[128];
		sprintf(end, "\t</BODY>\n");
		sprintf(end, "%s</HTML>\n", end);
		fwrite(end, sizeof(char), strlen(end), f);

		fclose(f);
	}
}


char * SaveCharacter(int i, GCHARS_CharacterType type){ 

	char *cName = new char[32];
	char *mesh = new char[64];
	float health, explosion;
	float posX, posY, posZ;
	float speedX, speedY, speedZ;
	float sizeX, sizeY, sizeZ;
	int lives;

	switch(type){

		case CHARS_PLAYER:
			strcpy(cName, "PLAYER");
			health = Player[i].Health;
			explosion = Player[i].Hit_duration;
			lives = Player[i].Lives;
			strcpy(mesh, Player[i].MeshName);
			posX = Player[i].Position.v[XDIM];
			posY = Player[i].Position.v[YDIM];
			posZ = Player[i].Position.v[ZDIM];
			speedX = Player[i].Speed.v[XDIM];
			speedY = Player[i].Speed.v[YDIM];
			speedZ = Player[i].Speed.v[ZDIM];
					
			break;

		case CHARS_SUPPLYSHIP:
			strcpy(cName, "SUPPLYSHIP");
			health = Navy.SupplyShip[i]->Health;
			explosion = Navy.SupplyShip[i]->Hit_duration;
			strcpy(mesh, Navy.SupplyShip[i]->MeshName);
			posX = Navy.SupplyShip[i]->Position.v[XDIM];
			posY = Navy.SupplyShip[i]->Position.v[YDIM];
			posZ = Navy.SupplyShip[i]->Position.v[ZDIM];
			speedX = Navy.SupplyShip[i]->Speed.v[XDIM];
			speedY = Navy.SupplyShip[i]->Speed.v[YDIM];
			speedZ = Navy.SupplyShip[i]->Speed.v[ZDIM];
			break;

		case CHARS_SHIP:
			strcpy(cName, "SHIP");
			health = Navy.Ship[i]->Health;
			explosion = Navy.Ship[i]->Hit_duration;
			strcpy(mesh, Navy.MeshName);
			posX = Navy.Ship[i]->Position.v[XDIM];
			posY = Navy.Ship[i]->Position.v[YDIM];
			posZ = Navy.Ship[i]->Position.v[ZDIM];
			speedX = Navy.Ship[i]->Speed.v[XDIM];
			speedY = Navy.Ship[i]->Speed.v[YDIM];
			speedZ = Navy.Ship[i]->Speed.v[ZDIM];
			sizeX = Navy.Ship[i]->Size.v[XDIM];
			sizeY = Navy.Ship[i]->Size.v[YDIM];//  Y and Z coordinates are not initialized at the beginning
			sizeZ = Navy.Ship[i]->Size.v[ZDIM];
			break;

		case CHARS_BUNKER:
			strcpy(cName, "BUNKER");
			health = BunkersManager.Bunker[i].Health;
			posX = BunkersManager.Bunker[i].Position.v[XDIM];
			posY = BunkersManager.Bunker[i].Position.v[YDIM];
			posZ = BunkersManager.Bunker[i].Position.v[ZDIM];
			break;

		default:
			break;
	}

	char *character = new char[512];

	sprintf(character, "\t\t<CHARACTER>\n");
	sprintf(character, "%s\t\t\t<NAME>%s</NAME>\n", character, cName);
	sprintf(character, "%s\t\t\t<HEALTH>%0.0f</HEALTH>\n", character, health);
	// Position
	sprintf(character, "%s\t\t\t<POSITION>\n", character);
	sprintf(character, "%s\t\t\t\t<X>%.2f</X>\n", character, posX);
	sprintf(character, "%s\t\t\t\t<Y>%.2f</Y>\n", character, posY);
	if(posZ > -1000)	// To avoid non-defined Z positions
		sprintf(character, "%s\t\t\t\t<Z>%.2f</Z>\n", character, posZ);
	else
		sprintf(character, "%s\t\t\t\t<Z>0.0</Z>\n", character, posZ);
	sprintf(character, "%s\t\t\t</POSITION>\n", character);

	// Only NON-BUNKER characters
	if(type != CHARS_BUNKER){
		sprintf(character, "%s\t\t\t<EXPLOSION>%0.0f</EXPLOSION>\n", character, explosion);
		sprintf(character, "%s\t\t\t<MESH>%s</MESH>\n",character, mesh);
		sprintf(character, "%s\t\t\t<SPEED>\n", character);
		sprintf(character, "%s\t\t\t\t<X>%.3f</X>\n", character, speedX);
		sprintf(character, "%s\t\t\t\t<Y>%.3f</Y>\n", character, speedY);
		sprintf(character, "%s\t\t\t\t<Z>%.3f</Z>\n", character, speedZ);
		sprintf(character, "%s\t\t\t</SPEED>\n", character);
	}

	// Only PLAYER
	if(type == CHARS_PLAYER){ sprintf(character, "%s\t\t\t<LIVES>%i</LIVES>\n",character, lives); }

	// Only SHIP
	if(type == CHARS_SHIP){ sprintf(character, "%s\t\t\t<SUBTYPE>%i</SUBTYPE>\n",character, Navy.Ship[i]->SubType);
		sprintf(character, "%s\t\t\t<SIZE>\n", character);
		sprintf(character, "%s\t\t\t\t<X>%.1f</X>\n", character, sizeX);
		sprintf(character, "%s\t\t\t\t<Y>%.1f</Y>\n", character, sizeY);
		sprintf(character, "%s\t\t\t\t<Z>%.1f</Z>\n", character, sizeZ);
		sprintf(character, "%s\t\t\t</SIZE>\n", character);
	}

	// Only BUNKER
	if(type == CHARS_BUNKER){
		int x, y;
		for(x=0; x<CBNK_MAX_LINES; x++){
			sprintf(character, "%s\t\t\t<LINE>", character);
			for(y=0; y<CBNK_MAX_COLUMNS; y++){
				if(BunkersManager.Bunker[i].Brick[x][y].Alive)
					sprintf(character, "%s1", character);
				else
					sprintf(character, "%s0", character);
			}
			sprintf(character, "%s</LINE>\n", character);
		}
	}
	
	sprintf(character, "%s\t\t</CHARACTER>\n", character);

	return character;
}

void CSIGame::Display()
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Makes the buffers dark
	CFontsManager FontsManager;

	// Showing all the log strings
	FontsManager.SetFontSize(8);								// taille du texte des logs
	if (Application.Window.Width >= CGC_SVGA_HOR_RES)								// logs affichés si résolution >= 800x600
		for(int i=0; i<Log.LogIndex; i++)							// pour toutes les entrées de log
		{
			FontsManager.SetFontSize ();
			if (Application.Window.Width >= CGC_XGA_HOR_RES) 

					FontsManager.DefaultFontColor(); 
			else	FontsManager.SetFontColor(0.8f);
			//fontDrawString (10, Heigth -30 -(FontsManager.GetFontSize()+2)*i, "%s",  LogStrings[i]);
			FontsManager.DrawFont(10, Application.Window.Height -30 -(FontsManager.GetFontSize()+2)*i, "%s",  Log.LogStrings[i]);

/*					fontColor (1.0f, 1.0f, 1.0f); 
			else	fontColor (0.8f, 0.8f, 0.8f);
			fontDrawString (10, Heigth -30 -(FontsManager.GetFontSize+2)*i, "%s",  LogStrings[i]);
			*/
		}

	// Showing the image "loading..."
	LoadingBMPPosition = UGKCL_LOADINGBMP_FINAL_POSITION;

	Application.Window.Refresh();
}


void CSIGame::RunMainLoop(){
	
	CK_ACTIVE_TOKENS::iterator it;

	#ifdef DEF_CONTINUOUS_WITH_SIMULATE_TIME
	bool SynWithRealTime = false;
	#endif

	switch (AI->GetState())
	{
		case CSIG_INITIAL:
			//Passing from CSIG_INITIAL state to CSIG_CONFIGURE
			Application.Window.CreateRenderingContext();
			GUI.Fading = 0.0;
			GameEvent(CSIG_STARTING);//v Controlado 0->1
			break;
		case CSIG_CONFIGURE:	///Space Invaders loading image at the very beginning of the game
			GUI.FadingInAnimation();// Screen Fading In
			// When animation is finished, a game state change to LOADING state is performed
			break;
		case CSIG_FADING_IN:
			if(!LevelReaded){
				Log.Add("Reading next level of the game...");
				LoadLevel(Level);
				Log.Add("Loading simulation kernel...");
				LoadRTDESK();
				LauncherRTDESK();				
				Sync2RT=true;
				LevelReaded	= true;
			}	
			ChangeRenderMode(NextRenderMode);
			Loading();		
							
			#ifdef CHW_USE_KINECT
			status = initKinect();// initialize kinect parameters
			if (status != 0) 
			{
				printf("Couldn't initialize the Session Manager: %s\n", xnGetStatusString(status));
				exit(-1);
			}
			#elif  CHW_USE_WIIMOTE
			WiimotesManager.Init();
			#endif
			break;
		case CSIG_FADING_OUT2PLAY:
			GUI.FadingOutLoading();			// Screen Fading Out
			break;
		case CSIG_PLAYING:
			if (DiscreteSimulation){
				if (Sync2RT){
					RTDESK_Engine->SynchSim2RealTime(0.0);
					Sync2RT=false;
				}
				TimerManager.Timers[TmrRTDesk].EndCounting();
				if(NextSimulation <= TimerManager.Timers[TmrRTDesk].GetLastPeriod()){
					#ifdef DEF_IDL_TIME
					EndAccCounting(TmrIdl);
					#endif
			
					NextSimulation=RTDESK_Engine->Simulate();

					TimerManager.Timers[TmrRTDesk].InitCounting();
					#ifdef DEF_IDL_TIME
					TimerManager.Timers[TmrIdl].InitCounting();
					#endif			
				}
			}
			else{
				#ifdef DEF_IDL_TIME
				EndAccCounting(TmrIdl);
				#endif
				
				//Ships Update Polling (Time is supervised by each one)
				Navy.UpdateShips();

				//SupplyShips Update Polling (Time is supervised by each one)
				Navy.UpdateSupplyShips();

				//CircleShips Update Polling (Time is supervised by each one)
				Navy.UpdateCircleShips();

				//Update Active Shoots (Time is supervised by each one)
				ShootsManager.UpdateActiveShoots();

				//Update Active Bonus (Time is supervised by each one)
				BonusManager.UpdateActiveBonus();

				//Player Update
				Player[CurrentPlayer].Update();

				#ifdef DEF_IDL_TIME
				TimerManager.Timers[TmrIdl].InitCounting();
				#endif

				//General Game Update phase (Game Lists Maintenance and Check Win)				
				nowTime = TimerManager.Timers[TmrApp].TicksUntilNow();
				elapTimeUpd = nowTime-lastTimeUpd;
				if(elapTimeUpd >= TicksToUpdate){
					#ifdef DEF_IDL_TIME
					EndAccCounting(TmrIdl);
					#endif
					#ifndef DEF_CONTINUOUS_WITH_SIMULATE_TIME
					lastTimeUpd = nowTime;
					Update();
					#else
					lastTimeUpd += TicksToUpdate;
					while(!SynWithRealTime){
						Update();

						nowTime = TimerManager.Timers[TmrApp].TicksUntilNow();
						elapTimeUpd = nowTime-lastTimeUpd;
						if(elapTimeUpd >= 0LL) 
								SynWithRealTime=true;
							else
								SynWithRealTime=false;
					}
					SynWithRealTime=false;
					#endif
					#ifdef DEF_IDL_TIME
					TimerManager.Timers[TmrIdl].InitCounting();
					#endif
				}

				//Render Phase
				nowTime = TimerManager.Timers[TmrApp].TicksUntilNow();
				elapTimeRnd = nowTime-lastTimeRnd;
				if(elapTimeRnd >= TicksToRender){
					#ifdef DEF_IDL_TIME
					EndAccCounting(TmrIdl);
					#endif
					#ifndef DEF_CONTINUOUS_WITH_SIMULATE_TIME
					lastTimeRnd = nowTime;
					Render();
					#else
					lastTimeRnd += TicksToRender;
					while(!SynWithRealTime){
						Render();
						nowTime = Game.TimerManager.Timers[TmrApp].TicksUntilNow();
						elapTimeRnd = nowTime - LastUpdTime;
						if(elapTimeRnd >= 0LL) 
							SynWithRealTime=true;
						else
							SynWithRealTime=false;
					}
					SynWithRealTime=false;
					#endif
					#ifdef DEF_IDL_TIME
					TimerManager.Timers[TmrIdl].InitCounting();
					#endif
				}		

			}

			break;
		case CSIG_SAVING:
			Save(CSIG_SAVED_GAME);
			GameEvent(CSIG_SAVED);		//v Controlado  5->4   ///<Return to playing state
			break;
		case CSIG_PASSING_2_3D:
			Game.ReshapeGL();
			passing_2D_2_3D = true;
			//Render Phase
				nowTime = TimerManager.Timers[TmrApp].TicksUntilNow();
				elapTimeRnd = nowTime-lastTimeRnd;
				if(elapTimeRnd >= TicksToRender){
					#ifdef DEF_IDL_TIME
					EndAccCounting(TmrIdl);
					#endif

					#ifndef DEF_CONTINUOUS_WITH_SIMULATE_TIME
					lastTimeRnd = nowTime;
					Render();
					#else
					lastTimeRnd += TicksToRender;
					while(!SynWithRealTime){

						Render();

						nowTime = Game.TimerManager.Timers[TmrApp].TicksUntilNow();
						elapTimeRnd = nowTime - LastUpdTime;
						if(elapTimeRnd >= 0LL) 
							SynWithRealTime=true;
						else
							SynWithRealTime=false;
					}
					SynWithRealTime=false;
					#endif

					#ifdef DEF_IDL_TIME
					TimerManager.Timers[TmrIdl].InitCounting();
					#endif
				}

			break;
		case CSIG_INTERMISSION:
			//StartCameraMov();
			GameEvent(CD_INTERMISS_FINISHED);	//v Controlado  7->8 
			break;
		case CSIG_LOST_LIFE:
			if (--Player[CurrentPlayer].Lives <= 0) {
				GameEvent(CSIG_FADING2LOST); //v Controlado 8->13
			}
			else
			{
				//ACHTUNG: Do something here before restarting to play
				GameEvent(CSIG_PLAY);  //v Controlado 8->4
			}
			break;
		case CSIG_WON:		
			PromoteLevel();
			break;
		case CSIG_LOST_GAME:
			Level = -1;
			PromoteLevel();
			GameEvent(CSIG_LOADING);		//v Controlado 10->2
			break;
		case CSIG_FADING_OUT2WON:
			SoundsManager.Play(CGS_APPLAUSE, UGKSND_MAX_VOLUME);
			SoundsManager.SetPosition(CGS_APPLAUSE,NULL);
			StartCameraMov();
			GUI.FadingInWon();
			GameEvent(CSIG_FADED);			//v Controlado 11->9
			break;
		case CSIG_FADING_OUT2END:
			///ACHTUNG: Aquí debería incluirse el código para eliminar a TODOS los objetos del sistema, no sólo a las SS
			///Esto debería además ser incluido como parte de la acción del estado LOST del juego
			
			//StartCameraMov();
			Deinitialize();
			GameEvent(CSIG_2END);			//v Controlado 12->16
			break;
		case CSIG_FADING_OUT2LOST:
			GameEvent(CSIG_FADED);			//v Controlado 13->10
			break;
		case CSIG_PAUSED:
			if(!Paused)
				Game.GameEvent(CSIG_CONTINUE);		//v Controlado 14->4
			break;
		case CSIG_WAIT_KEY:			
			GUI.DisplayPressKey();
			GameEvent(CGS_KEYPRESSED);  //v Controlado 15->4
														
			#ifdef CHW_USE_KINECT
			if (waitKinect() == true)
				GameEvent(CGS_KEYPRESSED);// start to play (key pressed or "wave" detected)
			#endif	
			break;
		case CSIG_END:
			if(!OnlyOneLevel)
				exit(EXIT_SUCCESS);
			else
				StopAfterOneLevel = true;
			break;
	}	//switch

	#ifdef CHW_USE_KINECT
	K_SHIP_MOVE dir_kinect = updateKinect();	// update hand position
	#elif CHW_USE_WIIMOTE
	#endif

	if (CSIG_PLAYING != AI->GetState() && CSIG_PAUSED != AI->GetState())	// Game.Step function (loading) has its own swapbuffers
		Application.Window.Refresh();				// glFlush and Swap Buffers (Double Buffering)
					
}

void CSIGame::ReceiveMessage(RTDESK_CMsg *pMsg){

	switch (pMsg->Type)
	{
	case RTDESKMSG_GAME:
		cMsgGame *auxMsg;
			auxMsg = (cMsgGame*)pMsg;

			switch (auxMsg->msgSubType)
			{
			case TIPORENDER:
				
					Render();		

					if(!endByTime && !endByFrame){
						#ifdef DEF_RTD_TIME
						Game.TimerManager.Timers[TmrRTDSKMM].InitCounting();
						#endif

						SendSelfMsg(msgRender,TicksToRender);

						#ifdef DEF_RTD_TIME
						EndAccCounting(TmrRTDSKMM);
						#endif
					}
				
				break;
			case TIPOUPDATE:
				
					DiscreteUpdate();		

					if(!endByTime && !endByFrame){
						#ifdef DEF_RTD_TIME
						Game.TimerManager.Timers[TmrRTDSKMM].InitCounting();
						#endif

						SendSelfMsg(msgUpdate,TicksToUpdate);

						#ifdef DEF_RTD_TIME
						EndAccCounting(TmrRTDSKMM);
						#endif
					}
				
				break;
			default:
				break;
			}
			
		break;

	default:
		break;
	}

}


void CSIGame::LoadRTDESK(){

	//****************************************************************************************************//
	NextSimulation = RTDESKT_NOTIME;
	RTDESK_TIME Aux;

	RTDESK_Engine=new RTDESK_CEngine();	
	bool aux;	
	aux = RTDESK_Engine->StartUp();
	//1 - Todo Ok , 0 - Error
	printf("RTDESK_StartUpEngine devuelve %d\n", aux);
	

	//Para permitir que los mensajes con tiempo 0 se ejecuten directamente sin encolarse pongo 1LL.
	Aux = RTDESK_SLACK_TIME;
	RTDESK_Engine->SetStoreSlackTime(Aux);	//StoreSlackTime es longlong -> 1 representa un tick de CPU

	//Configuración del Pool Manager
	RTDESK_Engine->SetMaxMsgTypes(RTDESKMSG_MAX_TYPE);
	RTDESK_Engine->SetMsgType(RTDESKMSG_GAME, sizeof(cMsgGame));
	RTDESK_Engine->SetMsgType(RTDESKMSG_NAVY, sizeof(cMsgNavy));
	RTDESK_Engine->SetMsgType(RTDESKMSG_SHIP, sizeof(cMsgShip));
	RTDESK_Engine->SetMsgType(RTDESKMSG_SSHIP, sizeof(cMsgSShip));
	RTDESK_Engine->SetMsgType(RTDESKMSG_CSHIP, sizeof(cMsgCShip));
	
}

void CSIGame::LauncherRTDESK()
{
	//Each game object wich is going to send events messages must have asigned one RTDESK_Engine Message Dispatcher.
	//Assignment of times to forward messages.
	//Navy
	Navy.SetMsgDispatcher(RTDESK_Engine->MsgDispatcher);

	//Ships
	for(unsigned int i=0;i<Navy.Ship.size();i++){
		Navy.Ship[i]->TicksToUpdateShip = Navy.Timer.ms2Ticks(ShipTemp);
		Navy.Ship[i]->SetMsgDispatcher(RTDESK_Engine->MsgDispatcher);
	}
	
	//SupplyShips
	for(unsigned int i=0;i<Navy.SupplyShip.size();i++){
		Navy.SupplyShip[i]->TicksToUpdateSupplyShip = Navy.Timer.ms2Ticks(SupplyShipTemp);
		Navy.SupplyShip[i]->SetMsgDispatcher(RTDESK_Engine->MsgDispatcher);
	}
	
	//CircleShips
	for(unsigned int i=0;i<Navy.CircleShip.size();i++){
		Navy.CircleShip[i]->TicksToUpdateCircleShip = Navy.Timer.ms2Ticks(CircleShipTemp);
		Navy.CircleShip[i]->SetMsgDispatcher(RTDESK_Engine->MsgDispatcher);
	}

	//Players
	for(unsigned int i=0;i<CP_MAX_PLAYERS;i++){	
		Player[i].TicksToUpdatePlayer = Player[i].UpdatePlayerTimer.ms2Ticks(PlayerTemp);
		Player[i].SetMsgDispatcher(RTDESK_Engine->MsgDispatcher);
	}

	//ShootsManager
	TicksToUpdShootsMngr = UpdateShootsTimer.ms2Ticks(ShootsTemp);
	DiscreteShootsManager.timeRTdeskMsg = TicksToUpdShootsMngr;
	DiscreteShootsManager.SetMsgDispatcher(RTDESK_Engine->MsgDispatcher);

	//BonusManager
	TicksToUpdBonusMngr = UpdateBonusTimer.ms2Ticks(BonusTemp);
	DiscreteBonusManager.ticksRTdeskMsg = TicksToUpdBonusMngr;
	DiscreteBonusManager.SetMsgDispatcher(RTDESK_Engine->MsgDispatcher);
	
	//Render and General Update Game
	TicksToRender = RenderTimer.ms2Ticks(RenderTemp);
	TicksToUpdate = UpdateTimer.ms2Ticks(UpdateTemp);
	SetMsgDispatcher(RTDESK_Engine->MsgDispatcher);

	//****************************************************************************************************//
	//Launching First Event Messages

	//Update Render and General Game Update
	SendSelfMsg(msgRender,TicksToRender);
	SendSelfMsg(msgUpdate,TicksToUpdate);
	
	//Update Ships
	for(unsigned int i=0;i<Navy.Ship.size();i++){
		msg = GetMsgToFill(RTDESKMSG_BASIC_TYPE);
		msg->Type = RTDESKMSG_BASIC_TYPE;
		SendMsg(msg,Navy.Ship[i],Navy.Ship[i]->TicksToUpdateShip);
	}	
	//SendMsg(Navy.msg,&Navy,Navy.TicksToUpdateNavysShips);

	//Update SupplyShips
	for(unsigned int i=0;i<Navy.SupplyShip.size();i++){
		Navy.SupplyShip[i]->MoveTo((float)i, 4.0f, 0.05f);
		msg = GetMsgToFill(RTDESKMSG_BASIC_TYPE);
		msg->Type = RTDESKMSG_BASIC_TYPE;
		SendMsg(msg,Navy.SupplyShip[i],Navy.SupplyShip[i]->TicksToUpdateSupplyShip);
	}

	//Update CircleShips
	for(unsigned int i=0;i<Navy.CircleShip.size();i++){
		msg = GetMsgToFill(RTDESKMSG_BASIC_TYPE);
		msg->Type = RTDESKMSG_BASIC_TYPE;
		SendMsg(msg,Navy.CircleShip[i],Navy.CircleShip[i]->TicksToUpdateCircleShip);
	}

	//Update Player
	for(unsigned int i=0;i<CP_MAX_PLAYERS;i++){	
		msg = GetMsgToFill(RTDESKMSG_BASIC_TYPE);
		msg->Type = RTDESKMSG_BASIC_TYPE;
		SendMsg(msg,&Player[i],Player[i].TicksToUpdatePlayer);
	}
	
	//Execution in Game DiscreteUpdate
	//Update ShootsManager
	/*msg = GetMsgToFill(RTDESKMSG_BASIC_TYPE);
	msg->Type = RTDESKMSG_BASIC_TYPE;
	SendMsg(msg,&DiscreteShootsManager,TicksToUpdBonusMngr);*/

	//Update BonusManager
	/*msg = GetMsgToFill(RTDESKMSG_BASIC_TYPE);
	msg->Type = RTDESKMSG_BASIC_TYPE;
	SendMsg(msg,&DiscreteBonusManager,TicksToUpdShootsMngr);*/
}

void CSIGame::InitTimers(){

	TimerManager.CreateTimers(TIMER_AMOUNT);
	TimerManager.SetTimersName(TimersName);
	TimerManager.ResetSamplingFrequency();

	TimerManager.Timers[TmrApp].Reset();
	TimerManager.Timers[TmrApp].InitCounting();											//Start application timer.
	printf("Timers Frecuency = %d\n", TimerManager.Timers[TmrApp].GetSamplingFreq());
	TimerManager.Timers[TmrRTDesk].InitCounting();										//Start timer to control RT-Desk simulation

	#ifdef DEF_IDL_TIME
	TimerManager.Timers[TmrIdl].InitCounting();					//Start timer to mesasure idle time
	#endif
}


void CSIGame::Loading ()
{
	static bool LoadingTexturesLogged = false;
	unsigned int i,j;

	LoadingBMP	= true;
	ProgressBar	= true;

	#ifdef QT_USE_QUADTREE	
	for(i=0;i<CHARS_MAX_CHARTYPE;i++)
		QuadtreeRoot::Instance()->elementos[i]=0;
	#endif

	// Loading 3D objects
	Log.Add("Loading Meshes...");
	try
	{
		Display();

		//Link every character in the game to its corresponding mesh
		Log.Add("Assigning Meshes to characters");

		for(i=0;i<CP_MAX_PLAYERS;i++){
			Player[i].Mesh = MeshesManager.Meshes[Player[i].IndMesh];
			Player[i].Laser[CP_LEFT_LASER].Mesh = MeshesManager.Meshes[Player[i].Laser[CP_LEFT_LASER].IndMesh];
			Player[i].Laser[CP_RIGHT_LASER].Mesh = MeshesManager.Meshes[Player[i].Laser[CP_RIGHT_LASER].IndMesh];
		}
	
		Navy.SetMeshes();

		for(i=0;i<Navy.SupplyShip.size();i++)
			Navy.SupplyShip[i]->FitMeshIntoBoundingBox(Navy.SupplyShip[i]->Mesh);

		for(j=0;j<Navy.CircleShip.size();j++)
			Navy.CircleShip[j]->FitMeshIntoBoundingBox(Navy.CircleShip[j]->Mesh);

		bool aux=true;
		for(i=0;i<Navy.Ship.size();i++){
			Navy.Ship[i]->FitMeshIntoBoundingBox(Navy.Ship[i]->Mesh);
			#ifdef QT_USE_QUADTREE
			QuadtreeRoot::Instance()->getQuadtree()->ActualizeCharacter(Navy.Ship[i]);
			#endif
			if(BouncingMode){
				float num= (3+rand()%(21-3))*0.1f;	//0.3 <-> 2.0;
				Navy.Ship[i]->Speed.v[XDIM] *= num;
				Navy.Ship[i]->Speed.v[YDIM] *= num;
				if(aux){
					Navy.Ship[i]->Speed.v[XDIM] *= -1;
					Navy.Ship[i]->Speed.v[YDIM] *= -1;
				}
				aux=!aux;
			}
		}
	#ifdef CHAR_USE_QUADTREE
		for(i=0;i<CP_MAX_PLAYERS;i++){
			Player[i].FitMeshIntoBoundingBox(*Player[0].Mesh);
		}
	#endif
		Background.Mesh = MeshesManager.Meshes[Background.IndMesh];
		Background.Mesh->modelo.Materials->tex.texture[0] = TexturesManager.Textures[Background.IndTexture3D]->Texture.gl_id;
		

		//Bonuses, particles and bricks are billboards. Only a texture set on top of a square

		GameEvent(CSIG_FADING_OUT); //v Controlado 2->3
	}
	catch(...)
	{
		ERR("Exception: There was a problem during loading of the 3D objects.");
	}
}

// REPLACE CAMERA
void CSIGame::ReshapeGL(void)									// Reshape The Window When It's Moved Or Resized
{
	Application.Window.Reshape(45.0f);	///Reshape main Window
	Camera.Reshape(20.0f);	///Reshape the camera

	if (CHAR_2D != Game.RenderMode)
	{
		Scene.Angle.v[YDIM] = -40;
		Player[CurrentPlayer].Position.v[YDIM] = -7.00;
	}
	else 
	{
		Scene.Angle.v[YDIM] = 0;
		Player[CurrentPlayer].Position.v[YDIM] = -6.15;
	}
#ifdef CHAR_USE_AABB
	Player[CurrentPlayer].UpdateAABB();
#endif
	Scene.Angle.v[XDIM] = 0;

	if(DiscreteSimulation) Sync2RT = true;
}


/**   
   * @fn CSIGame::DisplayLoadingBMP
   * Show the Image "Loading"
   * Starts the colors in 1.0f
   * After Despues une la textura 2D con el Manegador de Texturas 
   * After the image is displayed at the coordinates given loaded
*/
void CSIGame::DisplayLoadingBMP()
{
	glColor4f (1.0f, 1.0f, 1.0f, GUI.Fading);
	TexturesManager.Textures[CTM_LOADING]->SetTexture();
	glBegin (GL_QUADS);
		glTexCoord2f (0,0);glVertex3f ( -3.0*LoadingBMPPosition, 0.50*LoadingBMPPosition, .05f);
		glTexCoord2f (1,0);glVertex3f (  3.0*LoadingBMPPosition, 0.50*LoadingBMPPosition, .05f);
		glTexCoord2f (1,1);glVertex3f (  3.0*LoadingBMPPosition, 1.70*LoadingBMPPosition, .05f);
		glTexCoord2f (0,1);glVertex3f ( -3.0*LoadingBMPPosition, 1.70*LoadingBMPPosition, .05f);
	glEnd ();
}
//Parche. Eliminar en futuros refactorings
extern int	int_sound_mp3;

/**   
   * @fn CSIGame::DisplayScore
   * Show the Image "Loading"
   * Starts the colors in 1.0f
   * After Despues une la textura 2D con el Manegador de Texturas 
   * After the image is displayed at the coordinates given loaded
*/
void CSIGame::DisplayScore(){

	
glColor4f (1.0f, 1.0f, 1.0f, GUI.Fading);
	TexturesManager.Textures[CTM_SCORE]->SetTexture();
	glBegin (GL_QUADS);
		glTexCoord2f (0,1);	glVertex2f ((0.0f - 2.0f), (2.f +.6f));
		glTexCoord2f (0,0);	glVertex2f ((0.0f - 2.0f), (2.f -.6f));
		glTexCoord2f (1,0);	glVertex2f ((0.0f + 2.0f), (2.f -.6f));
		glTexCoord2f (1,1);	glVertex2f ((0.0f + 2.0f), (2.f +.6f));
	glEnd ();

	char str_chif[7] = UGKS_EMPTY_STRING;

	_itoa(Game.Score, str_chif, 10);        // integer to string, en base 10

	int long_chif = strlen(str_chif);
	int long_0 = 6 - long_chif, i;

	for (i=0; i<= long_0 - 1; i++)
	{
		glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
		TexturesManager.Textures[CTM_NUMBER0]->SetTexture();
		glBegin (GL_QUADS);
			glTexCoord2f (0,1);	glVertex2f (-1.25f +i/1.9f - .6f, 0.f +.6f);
			glTexCoord2f (0,0);	glVertex2f (-1.25f +i/1.9f - .6f, 0.f -.6f);
			glTexCoord2f (1,0);	glVertex2f (-1.25f +i/1.9f + .6f, 0.f -.6f);
			glTexCoord2f (1,1);	glVertex2f (-1.25f +i/1.9f + .6f, 0.f +.6f);
		glEnd ();
	}
	for (i=long_0 ; i<= 5 ; i++)
	{
		int chif_texture = (int)(str_chif[i - long_0]) - 48; // como "0" = 48 en la tabla ASCII y el elenco (char *) no funciona para atoi ()

		glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
		TexturesManager.Textures[CTM_NUMBER0]->SetTexture();
		glBegin (GL_QUADS);
			glTexCoord2f (0,1);	glVertex2f (-1.25f +i/1.9f - .6f, 0.0f +.6f);
			glTexCoord2f (0,0);	glVertex2f (-1.25f +i/1.9f - .6f, 0.0f -.6f);
			glTexCoord2f (1,0);	glVertex2f (-1.25f +i/1.9f + .6f, .0f -.6f);
			glTexCoord2f (1,1);	glVertex2f (-1.25f +i/1.9f + .6f, .0f +.6f);
		glEnd ();
	}


}

//V Se ejecuta en el paso de 15->4
void *CSIG_Go2Play(LPSTR *args, CSIGame *Game)
{
	Game->ReshapeGL();
	OutputDebugString("Space Invaders [End loading]");
	if (AAx4 == Application.Window.AntialiasingMode || AAx4S == Application.Window.AntialiasingMode)
		glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);

	Navy.AI_Go2Play();

	return NULL;
}

void CSIGame::Snapshot( char* filename)
{
	int windowWidth=Application.Window.Width;
	int windowHeight=Application.Window.Height;

	byte* bmpBuffer = (byte*)malloc(windowWidth*windowHeight*3);
	if (!bmpBuffer)
	return;

	glReadPixels((GLint)0, (GLint)0,
	(GLint)windowWidth-1, (GLint)windowHeight-1,
	GL_BGR, GL_UNSIGNED_BYTE, bmpBuffer);

	FILE *filePtr = fopen(filename, "wb");
	if (!filePtr)
	return;

	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPINFOHEADER bitmapInfoHeader;

	bitmapFileHeader.bfType = 0x4D42; //"BM"
	bitmapFileHeader.bfSize = windowWidth*windowHeight*3;
	bitmapFileHeader.bfReserved1 = 0;
	bitmapFileHeader.bfReserved2 = 0;
	bitmapFileHeader.bfOffBits =
	sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	bitmapInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfoHeader.biWidth = windowWidth-1;
	bitmapInfoHeader.biHeight = windowHeight-1;
	bitmapInfoHeader.biPlanes = 1;
	bitmapInfoHeader.biBitCount = 24;
	bitmapInfoHeader.biCompression = BI_RGB;
	bitmapInfoHeader.biSizeImage = 0;
	bitmapInfoHeader.biXPelsPerMeter = 0; // ?
	bitmapInfoHeader.biYPelsPerMeter = 0; // ?
	bitmapInfoHeader.biClrUsed = 0;
	bitmapInfoHeader.biClrImportant = 0;

	fwrite(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	fwrite(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	fwrite(bmpBuffer, windowWidth*windowHeight*3, 1, filePtr);
	fclose(filePtr);

	free(bmpBuffer);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void EndAccCounting(unsigned int indTmr){

	Game.TimerManager.Timers[indTmr].EndCounting();
	Game.TimerManager.Timers[indTmr].Accumulate();

	#ifdef DEF_EXP_CSV_FILE	
	if(!initReg){
		Game.TimerManager.Timers[TmrApp].EndCounting();	
		if (DEF_MIN_TICKS < Game.TimerManager.Timers[TmrApp].GetLastPeriod()) 
			initReg=true;
	}else{
		switch (indTmr)
		{
		case TmrUpd:
			msTotUpd += Game.TimerManager.Timers[TmrUpd].GetLastPeriodms();
			#ifdef DEF_LEC_PARTIAL
				muestrasTotUpd[indTotUpd]=Game.TimerManager.Timers[TmrUpd].GetLastPeriodms();
				indTotUpd++;
			#endif
			break;

		case TmrRnd:
			msTotRnd += Game.TimerManager.Timers[TmrRnd].GetLastPeriodms();
			#ifdef DEF_LEC_PARTIAL
				muestrasTotRnd[indTotRnd]=Game.TimerManager.Timers[TmrRnd].GetLastPeriodms();
				indTotRnd++;
			#endif
			break;

		case TmrIdl:
			msTotIdl += Game.TimerManager.Timers[TmrIdl].GetLastPeriodms();
			#ifdef DEF_LEC_PARTIAL
				muestrasTotIdl[indTotIdl]=Game.TimerManager.Timers[TmrIdl].GetLastPeriodms();
				indTotIdl++;
			#endif
			break;

		case TmrRTDSKMM:
			msTotRTDSKMM += Game.TimerManager.Timers[TmrRTDSKMM].GetLastPeriodms();
			#ifdef DEF_LEC_PARTIAL
				muestrasTotRtd[indTotRtd]=Game.TimerManager.Timers[TmrRTDSKMM].GetLastPeriodms();
				indTotRtd++;
			#endif
			break;
		}
	}

	#endif
}


void MusicFadingIn	(void *dummy){Game.MusicFadeIn();};
void MusicFadingOut	(void *dummy){Game.MusicFadeOut();};