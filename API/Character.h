/** Header with definitions needed for the management of the characters

*	Prephix = CHAR_

*	@author Ramon Molla
*	@version 2011-10
*/

#ifndef CHAR_INITIALIZATION /// Macro character is erased
#define CHAR_INITIALIZATION /// Macro character is defined

//#include "Copyright.h"
#include <UGKSound.h>
#include <AI\FSM.h>
#include <UGKPhysics.h>
#include <UGKAlgebra.h>
#include <RTDeskEntity.h>
#include <Meshes\MeshManager.h>

#ifndef DEQUE_LOADED /// Macro "Loaded" is erased
#define DEQUE_LOADED /// Macro "Loaded" is defined
#include <deque>			// Uses Standard Template Library Double Ended Queue
using namespace std;
#endif

#define GCHAR_TAG_MAX_LONG		16			///<Maximun length of a sensitive tag
#define CHAR_MAX_MESH_FILE_NAME	64
#define CHAR_DEFAULT_ZPOS		0.05f	///<Maximun length of a sensitive tag

/**
	* We define new types of variable for the 2d and 3d animation
*/
typedef enum {
	CHAR_NO_RENDER,			//No render for this character: camera, collision objects,...
	CHAR_2D,
	CHAR_LINES3D,
	CHAR_3D,
	CHAR_MAX_RENDER_MODE	//For management purpouses only. Always at the end of the enumeration
} CHAR_RENDER_MODE;

/**
*	\typedef CHAR_INTERPOLATION_STATE
*	The amount of samples used for interpolation
*/
typedef enum {
	CHAR_CURRENT,			///<Current state. t0
	CHAR_LAST,				///<Previous state. t-1
	CHAR_LAST_LAST,				///<The state previous to the previous state. t-2
	CHAR_MAX_INTERPOLATION_STATES	///<For management purpouses only. Always at the end of the enumeration
} CHAR_INTERPOLATION_STATE;

/**
	* We define new types of variable to the sound of the game
*/
typedef enum{
	CHAR_SOUND_HIT,			// Sound emitted when the character has collided with something: bullet, another object
	CHAR_SOUND_EXPLOIDING,	// Sound emitted when the character is exploiding
	CHAR_SOUND_NORMAL,		// Sound emitted when the character by default during its common life
	CHAR_SOUND_MAX_SOUNDS
}CHAR_SoundType;

#define CHAR_NO_AABB		-1
#define CHAR_TYPE_UNKNOWN	-1
#define CHAR_DEFAULT_INDEXES	-1

#define CHAR_USE_QUADTREE	1
//#define CHAR_USE_AABB	2

#ifdef CHAR_USE_AABB
/**
	* We define new types of variable coordinate ranges
*/
typedef enum {
	CHAR_BBMIN,
	CHAR_BBMAX,
	CHAR_BBMAXCOORDS,
} CHAR_BBCOORD;

/**
	* Partially defined CCharacter class for reference only
*/
class CCharacter;

/**
*	@class CBBPoint
*	Class point for a Bounding Box 
*	An AABB needs 2 points: min and max values for every dimension. Typically 3D, total 3x2 = 6 points for character in game
*/
class CBBPoint
{
public:
	/// Position index inside the dimension array. For management purpouses
	int			Position;
	/// Axis Aligned Bounding Box in absolute coordinates. It may be minimun or maximun values for the AABB at that dimension
	float		Value;
	/// Character that has this limit
	CCharacter	*Character;
};

#elif CHAR_USE_QUADTREE
typedef struct{
	float minX,
		minY,
		minZ,
		maxX,
		maxY,
		maxZ,
		centerX,
		centerY,
		centerZ;
}CHAR_BB_Attributes;
#endif

/**
	* Define the class for which the start of the game
	* States of the player dead, alive
	* Game Graphics 2D, 3D
	* Video Game
	* Weapons told in the Game
	* Enemies Artificial Intelligence
	* Moves player
*/

using namespace UGK;

class CCharacter:public RTDESK_CEntity
{
	//Attributes
public:

	/// Type of character: bunker, ship, supplyship,...
	int Type;

	/// The character is active. Implicitly it needs to be alive in order to be active
	bool			Active,
	///The character is alive
					Alive;

	/// TODO Amount of time when the character is touched. ACHTUNG: NO PONER EN CHARACTER
	float			Hit_duration;

	/// Collection of sounds performed during character's life cycle	
	/// Normally one sound per different state available in the AI FSM
	CSound* Sound;

	/// Character's health. How far it is from the death
	float			Health,
	/// Top health a character can have anytime
					MaxHealth;
	/// Character cannot die
	bool			Immortal;

	///The way the character is rendered on the screen
	CHAR_RENDER_MODE RenderMode;
	///The way the character interacts to the rest of the world
	UGKPHY_PHYSIC_MODE PhysicMode;	
	
	/// 3D space coordinates where the character is NOW
	UGK::Vector			Position,
					/// 3D space coordinates where the character was just a main loop before. For UGKPHY_VERLET numeric integrator purpouses. Not used on UGKPHY_EULER numeric integrators.
					PositionPrev,
					/// 3D speed coordinates where the character is going to
					Speed,
					/// 3D acceleration coordinates where the character is speeding up to
					Acceleration,
					/// 3D space orientation in radians
					Angle,
					/// 3D space proportions
					Scale,
					/// Axis Aligned Bounding Box Width, Height and deep. Bounding box in local coordinates. Implicit bounding box minimun coordinates = {0,0,0}
					Size;

	bool			AIInitialized;
	/// Character Artificial Intelligence is supported by a Finite State Machine (FSM)
	FSM				*AI;
	/// Meshe for the character
	CMesh3d			*Mesh;
	/// Possible Meshes to use
	int				IndMesh;
	/// Textures for the character
	int				IndTexture2D;
	int				IndTexture3D;
	/// Animations for the character
	int				IndAnimation2D;	
	/// Name of the file mesh associated to the character's mesh. It does not hold the path to the file.
	char			MeshName[CHAR_MAX_MESH_FILE_NAME];

	RTDESK_TIME LastUpdTime;

#ifdef CHAR_USE_QUADTREE
	///Bounding Box attributes
	CHAR_BB_Attributes BoundingBox;
#elif CHAR_USE_AABB
	CBBPoint	AABB[CHAR_BBMAXCOORDS][UGKALG_DIMENSION];
#endif

	//Methods
	void Init ();

	///Constructor of the class
	CCharacter();
    
    ///Destructor of the class
	~CCharacter();

	///AI
	void			AI_Reset(void);
	///Default Artificial Intelligence character initialization
	virtual void	AI_Init	(void);
	virtual void	AI_Dye	(void);

	///What the character has to do on every time tick 
	virtual void Update (void);
	///Manages all the events received
	virtual void ManageEvent (unsigned int Event);

	//Physics
	///Recalculates the min and max values of the absolute coordinates of the bounding box
	void CalculateAABB	(CMesh3d &model);
	void CalculateAABB	();
	///Recalculates the min and max values of the absolute coordinates of the bounding box and inserts the character into the the collision detector
	void InitializeAABB	(void);
	///Recalculates the min and max values of the absolute coordinates of the bounding box and updates the character in the the collision detector
	void UpdateAABB		(void);
	void UpdateAABB		(UGK::SpaceCoords Coord);	///< The same only on a given coord
	///Executed when the collision detection system detects a collison with this object although 
	///this object may not be calculating any collision. Every object has to rewrite it in order to perform its proper treatment
	virtual void Collided (CCharacter *CollidedChar);
	void FitMeshIntoBoundingBox(CMesh3d &model);
	inline void FitMeshIntoBoundingBox(CMesh3d *model) {FitMeshIntoBoundingBox(*model);}
	void FitMeshIntoBoundingBox();
	void FitBoundingBoxArroundMesh(CMesh3d &model);

	///Shows the character on the screen
	virtual void Render				(void);
	///Change the way the character is going to be rendered on the screen
	virtual void ChangeRenderMode	(CHAR_RENDER_MODE);

	//Transformation methods
	///Moving by default depending on the miliseconds passed by
	void		 Move		(float ms);
	///Moves to the absolute coordinate x,y,z
	virtual void MoveTo		(float x, float y, float z);
	///Moving to an absolute position. It does not matter where the character is
	virtual void MoveTo		(Vector &Move);
	///Moves x,y,z units from the to the current position
	virtual void MoveRelTo	(float x, float y, float z);
	///Moving relative to the character current Position
	virtual void MoveRelTo	(Vector &Move);

	//RTDesk virtual Function
	void ReceiveMessage(RTDESK_CMsg *pMsg);
};

#endif