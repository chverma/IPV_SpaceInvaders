/**	Character.cpp - Clases, types, definitions, constants,..
	related with any kind of characters that may appear in the game.

	@author Ramon Molla
	@version 2011-09
*/

#include "Physics.h"

extern CCollisionDetection CollisionDetector;	/// The collision detector used to verify collisions between objects. Singleton

/**
	* Artificial Intelligence starts
	* It starts the game map checking that there is no collision with the ship and call CCharacter :: Init function to start the Ship
*/
CCharacter::CCharacter()
{
	///Initializes the AABB
	for (unsigned int i=X3D;i<ALG_DIMENSION;i++)
	{
		///Initially there is no collision detector position assigned
		AABB[CHAR_BBMIN][i].Position = CHAR_NO_AABB;
		AABB[CHAR_BBMAX][i].Position = CHAR_NO_AABB;

		AABB[CHAR_BBMIN][i].Character	= this;
		AABB[CHAR_BBMAX][i].Character	= this;
	}

	AI = NULL;
	AIInitialized = false;

	Init(); /// It starts the player's ship
}	

/**
 * Destroys the object.
 */
CCharacter::~CCharacter()
{
	AI = NULL;  ///<It does'n perform actions to destroy what AI points. It is responsability of the AIManager.
}


/**
	* Constructor for the object or the Game CCharacter
	* Start the type of ship you choose, the size and position the living state
	* Starts the ship in 2D
*/
void CCharacter::Init ()
{
	//Different character that may be loaded during the parsing
	///During the character initialization, the type of character is not known
	Type	=	CHAR_TYPE_UNKNOWN;
	///The character is not active by default
	Active	=	false;
	/// By default when a character is created, is not active but it is alive
	Alive	=	true;
	//The caracter is always set at a neutral position (0,0,0) and with no size
	Position.v[X3D]	= Position.v[Y3D] = Position.v[Z3D]	= 0.0f;
	Size.v[X3D]		= Size.v[Y3D]	  = Size.v[Z3D]		= 0.0f;

	///Do not perform action MoveTo (0.0f, 0.0f, 0.0f);	at this very moment because no insertion of character AABB has been performed in the collision detector
	///A move will call updateAABB which will produce an error
	Speed.Reset();

	///By default, render is always in 2D
	RenderMode	=	CHAR_2D;
	///By default, it is a normal object, so it can be rendered and touched
	PhysicMode	=	CHAR_PHYSICAL;

	///No AABB initialization is required here. Every inherited class manages if it has AABB or not and if it has to be initialized
}

void CCharacter::AI_Init (void)
{

}

///Removes any behaviour from the character. No matter which one may be. It is the same for all characters in the game
void CCharacter::AI_Reset (void)
{
	if (NULL != AI)
		AI->Init();

	AI = NULL;
	AIInitialized = false;
}

/**
	* Disables the character
*/
void CCharacter::AI_Dye (void)
{
	Active = Alive  = false;
}

void CCharacter::Update (void)	///What the character has to do on every time tick 
{
    	//By the moment, nothing to do on every time tick. Every character type has to implement it 
}

void CCharacter::ManageEvent (unsigned int Event)	//Manages all the events received
{
}

/**
	* Recalculates the minimum and maximum values ??of the absolute coordinates of the bounding box
*/
void CCharacter::CalculateAABB (void)
{
	float HalfSize;
	unsigned int i;

	for (i=X3D;i<ALG_DIMENSION;i++)
	{
		HalfSize = Size.v[i] / 2.0f;
		AABB[CHAR_BBMIN][i].Value = Position.v[i] - HalfSize;
		AABB[CHAR_BBMAX][i].Value = Position.v[i] + HalfSize;
		//Attribute Position is given by the collision detector
	}
}
/**
	* Updates the collision detection deques
*/
void CCharacter::UpdateAABB (void)
{	
	switch (PhysicMode)
	{
		case CHAR_INVISIBLE:	//No render, touchable: collidable
		case CHAR_PHYSICAL:		//Renderable, touchable: collidable
			///Recalculates the min and max values of the absolute coordinates of the bounding box
			CalculateAABB();
			CollisionDetector.Update(this);
	}
}

/**
	* Recalculates the min and max values of the absolute coordinates of the bounding box and inserts the character into the the collision detector
*/
void CCharacter::InitializeAABB	(void)			
{
	///Updates the collision detection deques
	switch (PhysicMode)
	{
		case CHAR_INVISIBLE:	//No render, touchable: collidable
		case CHAR_PHYSICAL:		//Renderable, touchable: collidable
			///Recalculates the min and max values of the absolute coordinates of the bounding box
			CalculateAABB();
 			CollisionDetector.Insert(this);
	}
}

/**
	* Executed when the collision detection system detects a collison with this object although this objectmay not be calculating any collision
*/
void CCharacter::Collided (CCharacter *CollidedChar)	
{
	//By default, substract health and test if the object has enough amount of heatlh to become alive
	Health -= CollidedChar->Health;
	if (Health < 0)
		Alive = Active = false;
}

/**
	* Function to render
*/  
void CCharacter::Render(void)
{
	switch(RenderMode)
	{
	case CHAR_NO_RENDER:			//No render for this character: camera, collision objects,...
		return;
	case CHAR_2D:                //Render 2D the character
		break;
	case CHAR_LINES3D:           //Render Lines 3D
		break;
    	case CHAR_3D:                //Render 3D the character
		break;
	default: return;
	}
}

/**
	* Function to change the render mode
*/
void CCharacter::ChangeRenderMode(CHAR_RENDER_MODE Mode)
{
	RenderMode = Mode;

	switch(RenderMode)
	{
	case CHAR_NO_RENDER:			//No render for this character: camera, collision objects,...
		return;
	case CHAR_2D:                //Render 2D the character
		break;
	case CHAR_LINES3D:           //Render Lines 3D
		break;
	case CHAR_3D:                //Render 3D the character
		break;
	default: return;
	}
}

/**
	* function to move to an absolute position to the character or ship depending on the acceleration
*/
///Geometric transformation methods
void CCharacter::Move		(float ms)	///Moves depending on the speed and acceleration
{
	Vector Aux;

	//Moving using Euler integrator
#ifdef EULER
	Speed.v[X3D] += Acceleration.v[X3D]*ms; /// axis movement speed x
	Speed.v[Y3D] += Acceleration.v[Y3D]*ms; /// axis movement speed y
	Speed.v[Z3D] += Acceleration.v[Z3D]*ms; /// axis movement speed z

	Position.v[X3D] += Speed.v[X3D]*ms;     /// character position on axis x
	Position.v[Y3D] += Speed.v[Y3D]*ms;     /// character position on axis y
	Position.v[Z3D] += Speed.v[Z3D]*ms;     /// character position on axis z
#elif VERLET
      // Recalculate the new position of the character
	Aux = Position;

	//P(t+At) = 2*P(t)
	Position += Position;

	//P(t+At) = 2*P(t) - P(t-At)
	Position -= PositionPrev;

	//P(t+At) = 2*P(t) - P(t-At) + P''(t)*At*At
	Speed.v[X3D] += Acceleration.v[X3D]*ms*ms;
	Speed.v[Y3D] += Acceleration.v[Y3D]*ms*ms;
	Speed.v[Z3D] += Acceleration.v[Z3D]*ms*ms;

	PositionPrev = Aux;

#endif
}

/**
	* function to move to an absolute position to the character or ship to a position x, y, z
*/
void CCharacter::MoveTo		(float x, float y, float z)	///Moves to the absolute coordinate x,y,z
{
	Position.v[X3D] = x;  //actualizar la posicion del caracter en el plano x
	Position.v[Y3D] = y;  //actualizar la posicion del caracter en el plano y
	Position.v[Z3D] = z;  //actualizar la posicion del caracter en el plano z

	///Updating the bounding box
	UpdateAABB ();
}

/**
	* function to move the character or ship to an absolute position from the current position
*/
void CCharacter::MoveTo		(Vector Move)				///Moving to an absolute position. It does not matter where the character is

{
	Position = Move; /// Update position vector based on the Move[x,y,z]
	UpdateAABB (); ///Updating the bounding box
}

/**
	* function to move the character or ship to a position based on x, y, z units from the current position
*/
void CCharacter::MoveRelTo	(float x, float y, float z)	///Moves x,y,z units from the to the current position
{
	Position.v[X3D] += x; /// update the position x by increasing x units of the character
	Position.v[Y3D] += y; /// update the position y by increasing y units of the character
	Position.v[Z3D] += z; /// update the position z by increasing z units of the character

	///Updating the bounding box
	UpdateAABB ();
}

/**
	* function to move the character or ship to a relative position from the current position 
*/
void CCharacter::MoveRelTo (Vector Move)	///Moving relative to the character current Position
{
	///Adding relative movement
	Position += Move; 

	///Updating the bounding box
	UpdateAABB ();
}
