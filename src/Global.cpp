/** 
*	Global variables and singletons are declared here

*	Prefix: No prefix by default

*	@author Ramon Molla
*	@version 2012-08
*/

#define WIN32_LEAN_AND_MEAN				// Compile faster (excludes automatic processing of some rarely-used headers)
#include "Global.h"						// Header File For General Parameters of the Game

#ifndef CDS_FULLSCREEN					// CDS_FULLSCREEN Is Not Defined By Some
#define CDS_FULLSCREEN 4				// Compilers. By Defining It This Way,
#endif									// We Can Avoid Errors

///Declarations of 3DS models
///All the characters of one type use the same geometry. So there is no reason to replicate
///the same mesh for evryone. Every character has a pointer to a given mesh type.
///There is a need to have a common repository for every possible character mesh
//CMesh3d Mesh		[MAX_NUM_MESH];	

/**	The collision detector used to verify collisions between objects.
*	This is the highest level of collision detection. Coarse detection
*	Objects included here: Bunkers manager, Navy, bonus and shoots
*	Heuristics used for speeding up the collision detection:
*	1.- No shoot can collide with another shoot, even if they come from the enemy
*	2.- No Navy shoot can hit any navy (ship or suply ship) or bonus
*	3.- The navy cannot collide with any bunker
*/
CCollisionDetection CollisionDetector;

///Amount of players simultaneously available when playing
CPlayer		 Player[CP_MAX_PLAYERS];	// PLAYERS AVAILABLE
CPlayer2	 Player2[CP_MAX_PLAYERS];	// PLAYERS AVAILABLE
int CurrentPlayer	= CP_NO_PLAYER;		///<Initially, the player by default is the one before the first one


/**
*	\defgroup Managers Managers Singletons
*	@{
*/
CAIManager				AIManager;			///<For managing the Artificial Intelligence of any kind of character
CTextureAnimationManager	AnimationsManager;
CBonusManager			BonusManager;
CBunkersManager			BunkersManager;
CBonusManager			DiscreteBonusManager;
CShootsManager			DiscreteShootsManager;
CFontsManager			FontsManager;
CMeshesManager			MeshesManager;
CScoresManager			ScoresManager;
CShootsManager			ShootsManager;
CSoundsManager			SoundsManager;
CTexturesManager		TexturesManager;

#ifdef CHW_USE_WIIMOTE
extern CWiimoteManager	WiimoteManager;
#endif

/**
*	\defgroup GSingletons General game singletons 
*
*	@{
*/
CApplication	Application;
CSIGame			Game;			///<Singleton declaration to save the general configuration of the game
CNavy			Navy;			///<Singleton to save the general configuration of the enemy Navy
CGUI			GUI;			///<Manages all the windows, msgs,... for the player
CCamera			Camera;			///<Singleton declaration of the game camera
CScene			Scene;			///<Singleton to manage the Scene
CLog			Log;			///<For logging performance, tracing,...
CCharacter		Background;		
CCharacterFactory	CharacterFactory;	///<Singleton declaration of the character factory that creates as many given characters as demanded

/**
*	\defgroup GSingletons HW game singletons 
*
*	@{
*/
CSIKeyboard		SIKeyboard;		///<Singleton to manage the application keyboard
CGraphicCard	GraphicsCard;	///<Singleton to manage the application Graphics Card
CSoundSystem	SoundSystem;

///@}

bool	 DiscreteSimulation	= false;
bool	 BouncingMode		= false;
bool	 Sync2RT			= false;

GLUquadric * quadratic;

#ifdef DEF_EXP_CSV_FILE
double msTotRnd=0;									//Total tiempo utilizado para render (ms)
double msTotUpd=0;									//Total tiempo utilizado para update (ms)
double msTotIdl=0;									//Total tiempo sobrante (ms)
double msTotRTDSKMM=0;								//Total tiempo utilizado para gestión de memoria y mensajes por RT-DESK
	#ifdef DEF_LEC_PARTIAL
		double muestrasTotRnd[DEF_LONG_ARRAYS];
		int indTotRnd=0;
		double muestrasTotUpd[DEF_LONG_ARRAYS];
		int indTotUpd=0;
		double muestrasTotIdl[DEF_LONG_ARRAYS];
		int indTotIdl=0;
		double muestrasTotRtd[DEF_LONG_ARRAYS];
		int indTotRtd=0;
	#endif
#endif

MAIN_END_TYPES WayToEnd=NO_END;
bool endByTime	= false;
bool endByFrame	= false;
bool initReg	= false;
unsigned int contDeUpd=0;

