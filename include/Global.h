/**	Global definitions, types, includes and so on

*	Prefix: SIGLB_

*	@author Ramon Molla
*	@version 2013-06
*/


#ifndef SIGLB_GLOBAL_H
#define SIGLB_GLOBAL_H

#ifdef  _MSC_VER
#pragma warning (disable:4305 4244 4005)	// Disable warnings on precision truncations/conversions
											// 4305 : truncation
											// 4244 : conversion, possible loss of data
#endif

//#include "Copyright.h"
//#include <Physics.h>
#include <CollisionDetect.h>
#include <math.h>				// Header File For sin(), cos(), _hypot()
#include <process.h>			// Header File For _beginthread, _endthread 
#include <SIGame.h>
#include "GUI.h"
#include "Scene.h"

#include <AI/AIManager.h>
#include <BonusManager.h>	/// Header File class Bonus Manager
#include <Meshes\MeshManager.h>
#include "FontsManager.h"
#include "ScoreManager.h"
#include "ShootsManager.h"	/// Header File class Manager for the shoots
#include "BunkersManager.h"	/// Header File class Manager for the Bunkers. Loads Bunker and it loads Brick
#include <Textures\TexturesManager.h>/// Header File class Manager for the Textures.

#include <UGKMusicsManager.h>
#include <UGKLog.h>

#include "QuadtreeRoot.h"
#include <Application.h>
#include "Player.h"
#include "TextoGL.h"
#include "fpsGL.h"
#include <SIMessage.h>
#include <vector>
#include "SITexturesResources.h"

#include "Player.h"			/// Header File class Player
#include "Navy.h"			/// Header File class Navy. Ships and supply ships that fight in the enemy navy
#include <Camera.h>			/// Header File class Camera

//HW
#include <SIKeyboard.h>
#include <GraphicsCard.h>

#define M_PI       3.14159265358979323846

//Avoids warnings of deprecated functions
#define _CRT_SECURE_NO_WARNINGS

#define MAXSSxLEV 10	//Maximun amount of Supply Ships per level
#define SIGLB_MAX_MUSIC_NAME_LENGTH	1024

// DEBUG TOOLS
#define DEBUG_MORTAL					// DEBUG
#define DEBUG_MUSIC						// DEBUG
#define DEBUG_BUNKERS					// DEBUG
#define DEBUG_FPS						// DEBUG

//Macro that eases the writing for a string in the language selected by the user
#define SIGLB_GetText(Name) Application.Language->GetText(Name).c_str()

typedef enum
{
	NO_END,
	END_BY_TIME,
	END_BY_FRAME,
	END_MAX_TYPES
} MAIN_END_TYPES;

using namespace UGK;

/**
	* Define all variables and global functions of the game
	* Equalizer, Sound, Windows, Character (Nav), Bonus, enmity, Time, 3D, 2D
*/

// Music
extern Playlist* my_playlist;			// Shoutcast playlist

extern float			game_3d_ajust;			// 3D adjust

extern GLUquadricObj*	quadratic;		// Storage For Our Quadratic Objects

///General game singletons
extern CApplication		Application;
extern CNavy			Navy;			///<Singleton to save the general configuration of the enemy Navy
extern CSIGame			Game;			///<Singleton declaration to save the general configuration of the game
extern CGUI				GUI;			///<Singleton declaration to render the general configuration of the game interface
extern CCamera			Camera;			///<Singleton declaration of the game camera
extern CCharacterFactory	CharacterFactory;	///<Singleton declaration of the character factory that creates as many given characters as demanded

/**
*	\defgroup GSingletons HW game singletons 
*
*	@{
*/
extern CSIKeyboard		SIKeyboard;		///<Singleton to manage the application keyboard
extern CGraphicCard		GraphicsCard;	///<Singleton to manage the application Graphics Card
extern CSoundSystem		SoundSystem;

extern CAIManager		AIManager;
extern CBonusManager	BonusManager;
extern CBonusManager	DiscreteBonusManager;
extern CBunkersManager	BunkersManager;
extern CFontsManager	FontsManager;
extern CMeshesManager	MeshesManager;
extern CMusicsManager	MusicsManager;
extern CScoresManager	ScoresManager;
extern CShootsManager	ShootsManager;
extern CShootsManager	DiscreteShootsManager;
extern CSoundsManager	SoundsManager;
extern CTextureAnimationManager	AnimationsManager;
extern CTexturesManager	TexturesManager;


#ifdef CHW_USE_WIIMOTE
extern CWiimoteManager	WiimotesManager;
#endif

// Default characters for initialization purpouses only
// Updated when the initialization parser is running
extern CBonus			defaultBonus;
extern CBrick			defaultBrick;
extern CBunker			defaultBunker;
extern CPlayer			defaultPlayer;
extern CShip			defaultShip;
extern CSupplyShip		defaultSShip;
extern CCircleShip		defaultCShip;

extern CScene			Scene;			//Scene
extern CCharacter		Background;
extern CLog				Log;				///<For logging performance, tracing,...

///Amount of players simultaneously available when playing
extern CPlayer			Player[CP_MAX_PLAYERS];	// PLAYERS AVAILABLE
extern int				CurrentPlayer;

extern float			UGKALG_NullMatrix[16];

/// The collision detector used to verify collisions between objects. Highest level collision detector
extern CCollisionDetection CollisionDetector;

extern bool DiscreteSimulation;
extern bool BouncingMode;
extern bool Sync2RT;

#ifdef DEF_EXP_CSV_FILE
extern double msTotRnd;									//Total tiempo utilizado para render (ms)
extern double msTotUpd;									//Total tiempo utilizado para update (ms)
extern double msTotIdl;									//Total tiempo sobrante (ms)
extern double msTotRTDSKMM;								//Total tiempo utilizado para gestión de memoria y mensajes por RT-DESK
	#ifdef DEF_LEC_PARTIAL
		extern double muestrasTotRnd[DEF_LONG_ARRAYS];
		extern int indTotRnd;
		extern double muestrasTotUpd[DEF_LONG_ARRAYS];
		extern int indTotUpd;
		extern double muestrasTotIdl[DEF_LONG_ARRAYS];
		extern int indTotIdl;
		extern double muestrasTotRtd[DEF_LONG_ARRAYS];
		extern int indTotRtd;
	#endif
#endif
extern MAIN_END_TYPES WayToEnd;
extern bool endByTime;
extern bool endByFrame;
extern bool initReg;
extern unsigned int contDeUpd;

extern void EndAccCounting(unsigned int indTmr);

#endif
