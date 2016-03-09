/** Header with definitions needed for the management of the characters

*	Prephix = CHAR_

*	@author Ramon Molla
*	@version 2011-10
*/

#ifndef CHAR_INITIALIZATION /// Macro character is erased
#define CHAR_INITIALIZATION /// Macro character is defined

//#include "Copyright.h"
#include "Sound.h"
#include "FSM.h"
#include "3DS.h"

#ifndef DEQUE_LOADED /// Macro "Loaded" is erased
#define DEQUE_LOADED /// Macro "Loaded" is defined
#include <deque>			// Uses Standard Template Library Double Ended Queue
using namespace std;
#endif

#define CHAR_TAG_MAX_LONG		16			///<Maximun length of a sensitive tag
#define CHAR_MAX_MESH_FILE_NAME	64
#define CHAR_DEFAULT_ZPOS		0.05f	///<Maximun length of a sensitive tag

/**
	* We define new types of variable direction movements
*/
typedef enum {
	CHAR_NO_DIRECTION,
	CHAR_UP,
	CHAR_DOWN,
	CHAR_RIGHT,
	CHAR_LEFT,
	CHAR_FRONT,
	CHAR_BACK,
	CHAR_MAX_DIRECTIONS
} CHAR_DIRECTION;

/**
	* We define new types of variable coordinate ranges
*/
typedef enum {
	CHAR_BBMIN,
	CHAR_BBMAX,
	CHAR_BBMAXCOORDS,
} CHAR_BBCOORD;

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
*	\typedef CHAR_PHYSIC_MODE
*	The phisical way a character exists in the world
*/
typedef enum {
	CHAR_PHANTOM,			///<No render, No touchable: camera
	CHAR_INVISIBLE,			///<No render, touchable: collision objects,...
	CHAR_LIGHT,				///<Visible but not touchable
	CHAR_PHYSICAL,			///<Normal objects: can be rendered and touched
	CHAR_MAX_PHYSIC_MODE	///<For management purpouses only. Always at the end of the enumeration
} CHAR_PHYSIC_MODE;

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


/**
	* Partially defined CCharacter class for reference only
*/
class CCharacter;

///Class point for a Bounding Box 
class CBBPoint
{
public:
	/// Position index inside the array. For management purpouses
	int			Position;
	/// Axis Aligned Bounding Box in absolute coordinates. 
	float		Value;
	/// Character that has this limit
	CCharacter	*Character;
};

/**
	* Define the class for which the start of the game
	* States of the player dead, alive
	* Game Graphics 2D, 3D
	* Video Game
	* Weapons told in the Game
	* Enemies Artificial Intelligence
	* Moves player
*/

class CCharacter
{
	//Attributes
public:

	/// Type of character: bunker, ship, supplyship,...
	int Type;

	/// The character is active. Implicitly it needs to be alive in order to be active
	bool			Active,
	///The character is alive
					Alive;

	/// Amount of time when the character is touched. ACHTUNG: NO PONER EN CHARACTER
	float			Hit_duration;

	/// Collection of sounds performed during character's life cycle	
	/// Normally one sound per different state available in the AI FSM
	CSound			Sound[CHAR_SOUND_MAX_SOUNDS];

	/// Character's health. How far it is from the death
	float			Health,
	/// Top health a character can have anytime
					MaxHealth;
	/// Character cannot die
	bool			Immortal;

	///The way the character is rendered on the screen
	CHAR_RENDER_MODE RenderMode;
	///The way the character interacts to the rest of the world
	CHAR_PHYSIC_MODE PhysicMode;	
	
	/// 3D space coordinates where the character is NOW
	Vector			Position,
					/// 3D space coordinates where the character was just a main loop before. For VERLET numeric integrator purpouses
					PositionPrev,
					/// 3D speed coordinates where the character is going to
					Speed,
					/// 3D acceleration coordinates where the character is speeding up to
					Acceleration,
					/// 3D space orientation in radians
					Angle,
					/// 3D space proportions
					Scale,
					/// Axis Aligned Bounding Box width, height and deep. Bounding box in local coordinates. Implicit bounding box minimun coordinates = {0,0,0}
					Size;

	bool			AIInitialized;
	/// Character Artificial Intelligence is supported by a Finite State Machine (FSM)
	FSM				*AI;
	/// Mesh for the character
	Model_3DS		*Mesh;
	/// Name of the file mesh associated to the character's mesh. It does not hold the path to the file.
	char			MeshName[CHAR_MAX_MESH_FILE_NAME];

	///Bounding Box attributes
	///Entrances for the AABB values. Array of pointers to AABB 
	CBBPoint AABB[CHAR_BBMAXCOORDS][ALG_DIMENSION];

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
	void CalculateAABB	(void);
	///Recalculates the min and max values of the absolute coordinates of the bounding box and inserts the character into the the collision detector
	void InitializeAABB	(void);
	///Recalculates the min and max values of the absolute coordinates of the bounding box and updates the character in the the collision detector
	void UpdateAABB		(void);
	///Executed when the collision detection system detects a collison with this object although 
	///this object may not be calculating any collision. Every object has to rewrite it in order to perform its proper treatment
	virtual void Collided (CCharacter *CollidedChar);

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
	virtual void MoveTo		(Vector Move);
	///Moves x,y,z units from the to the current position
	virtual void MoveRelTo	(float x, float y, float z);
	///Moving relative to the character current Position
	virtual void MoveRelTo	(Vector Move);
};

#endif
