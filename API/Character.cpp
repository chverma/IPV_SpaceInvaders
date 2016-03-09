/**	Character.cpp - Clases, types, definitions, constants,..
	related with any kind of characters that may appear in the game.

	@author Ramon Molla
	@version 2011-09
*/

#include <UGKPhysics.h>
#include "CollisionDetect.h"
#include <float.h>

extern CCollisionDetection CollisionDetector;	/// The collision detector used to verify collisions between objects. Singleton

/**
	* Artificial Intelligence starts
	* It starts the game map checking that there is no collision with the ship and call CCharacter :: Init function to start the Ship
*/
CCharacter::CCharacter()
{
	///Initializes the AABB
#ifdef CHAR_USE_AABB

	for (unsigned int i=XDIM;i<WDIM;i++)
	{
		///Initially there is no collision detector position assigned
		AABB[CHAR_BBMIN][i].Position = CHAR_NO_AABB;
		AABB[CHAR_BBMAX][i].Position = CHAR_NO_AABB;

		AABB[CHAR_BBMIN][i].Character	= this;
		AABB[CHAR_BBMAX][i].Character	= this;
	}

#endif
	AI = NULL;
	AIInitialized = false;

	LastUpdTime = 0LL;

	Init(); /// It starts the player's ship
}	

/**
 * Destroys the object.
 */
CCharacter::~CCharacter()
{
	AI = NULL;  ///<It does'n perform actions to destroy what AI points. It is responsability of the AI Manager.
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
	Position.Reset();
	Size.Reset();
	Sound = NULL;

	///Do not perform action MoveTo (0.0f, 0.0f, 0.0f);	at this very moment because no insertion of character AABB has been performed in the collision detector
	///A move will call updateAABB which will produce an error
	Speed.Reset();

	///By default, render is always in 2D
	RenderMode	=	CHAR_2D;
	///By default, it is a normal object, so it can be rendered and touched
	PhysicMode	=	UGKPHY_PHYSICAL;

	///Default values for Bounding Box. Every inherited class manages if it has AABB.
#ifdef CHAR_USE_QUADTREE
	BoundingBox.minX=FLT_MAX;
	BoundingBox.minY=FLT_MAX;
	BoundingBox.minZ=FLT_MAX;
	BoundingBox.maxX=FLT_MIN;
	BoundingBox.maxY=FLT_MIN;
	BoundingBox.maxZ=FLT_MIN;
	BoundingBox.centerX=0;
	BoundingBox.centerY=0;
	BoundingBox.centerZ=0;
#endif

	IndTexture2D	= CHAR_DEFAULT_INDEXES;
	IndTexture3D	= CHAR_DEFAULT_INDEXES;
	IndMesh			= CMM_NO_MESH;
	IndAnimation2D	= CHAR_DEFAULT_INDEXES;
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
	* Recalculates the minimum and maximum values of the absolute coordinates of the bounding box
*/
void CCharacter::CalculateAABB ()
{
#ifdef CHAR_USE_AABB
	unsigned int i;
	float HalfSize;

	for (i=XDIM;i<WDIM;i++)
	{
		HalfSize = Size.v[i] / 2.0f;
		AABB[CHAR_BBMIN][i].Value = Position.v[i] - HalfSize;
		AABB[CHAR_BBMAX][i].Value = Position.v[i] + HalfSize;
		//Attribute Position is given by the collision detector
	}
#elif CHAR_USE_QUADTREE
#endif
}

void CCharacter::CalculateAABB (CMesh3d &model)
{
#ifdef CHAR_USE_AABB
#elif CHAR_USE_QUADTREE

	/*	unsigned int quantity=0;
	
	for(int i=0;i<model.modelo.numObjects;i++){
		for(int j=0;j<model.modelo.Objects[i].numVerts*3;j++){
	
			switch (j%3)
			{
			case 0:
				//X
				centerX+=model.modelo.Objects[i].Vertexes[j];
				break;
			case 1:
				//Y
				centerY+=model.modelo.Objects[i].Vertexes[j];
				
				break;
			case 2:
				//Z
				centerZ+=model.modelo.Objects[i].Vertexes[j];
				
				break;
			}
			quantity++;
		}
	
	}

	quantity/=3;

	centerX/=quantity;
	centerY/=quantity;
	centerZ/=quantity;
	*/
	
	for(int i=0;i<model.modelo.numObjects;i++){
		for(int j=0;j<model.modelo.Objects[i].numVerts*3;j++){
			
			switch (j%3)
			{
			case 0:
				//X
				if(model.modelo.Objects[i].Vertexes[j]>BoundingBox.maxX) BoundingBox.maxX=model.modelo.Objects[i].Vertexes[j];
				if(model.modelo.Objects[i].Vertexes[j]<BoundingBox.minX) BoundingBox.minX=model.modelo.Objects[i].Vertexes[j];
				break;
			case 1:
				//Y
				if(model.modelo.Objects[i].Vertexes[j]>BoundingBox.maxY) BoundingBox.maxY=model.modelo.Objects[i].Vertexes[j];
				if(model.modelo.Objects[i].Vertexes[j]<BoundingBox.minY) BoundingBox.minY=model.modelo.Objects[i].Vertexes[j];	
				
				break;
			case 2:
				//Z
				if(model.modelo.Objects[i].Vertexes[j]>BoundingBox.maxZ) BoundingBox.maxZ=model.modelo.Objects[i].Vertexes[j];
				if(model.modelo.Objects[i].Vertexes[j]<BoundingBox.minZ) BoundingBox.minZ=model.modelo.Objects[i].Vertexes[j];
				
				
				break;
			}	
		}
	}
	//X Horizontal
	//Y Profundidad
	//Z Vertical

	//model.modelo.scaleX=Size.v[XDIM]/(maxX-minX);
	//model.modelo.scaleZ=Size.v[YDIM]/(maxY-minY);
	//model.modelo.scaleY=model.modelo.scaleZ;

	BoundingBox.centerX=(BoundingBox.maxX-BoundingBox.minX)/2.0f;
	BoundingBox.centerY=(BoundingBox.maxY-BoundingBox.minY)/2.0f;
	BoundingBox.centerZ=(BoundingBox.maxZ-BoundingBox.minZ)/2.0f;
#endif
}

#ifdef CHAR_USE_AABB
/**
	* Updates the collision detection deques
*/
void CCharacter::UpdateAABB (void)
{	
	switch (PhysicMode)
	{
		case UGKPHY_INVISIBLE:	//No render, touchable: collidable
		case UGKPHY_PHYSICAL:		//Renderable, touchable: collidable
			///Recalculates the min and max values of the absolute coordinates of the bounding box
			CalculateAABB();
			break;
	}
	CollisionDetector.Update(this);
}

/**
	* Updates the collision detection deques
*/
void CCharacter::UpdateAABB (UGK::SpaceCoords Coord)
{	
	float HalfSize;

	switch (PhysicMode)
	{
		case UGKPHY_INVISIBLE:	//No render, touchable: collidable
		case UGKPHY_PHYSICAL:		//Renderable, touchable: collidable
			///Recalculates the min and max values of the absolute coordinates of the bounding box
		HalfSize = Size.v[Coord] / 2.0f;
		AABB[CHAR_BBMIN][Coord].Value = Position.v[Coord] - HalfSize;
		AABB[CHAR_BBMAX][Coord].Value = Position.v[Coord] + HalfSize;
	
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
		case UGKPHY_INVISIBLE:	//No render, touchable: collidable
		case UGKPHY_PHYSICAL:		//Renderable, touchable: collidable
			///Recalculates the min and max values of the absolute coordinates of the bounding box

			CalculateAABB();
			CollisionDetector.Insert(this);
	}
}

#endif

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

void CCharacter::FitMeshIntoBoundingBox(CMesh3d &model)
{
#ifdef CHAR_USE_QUADTREE
	CalculateAABB(model);

	/*model.modelo.scaleX = Size.v[XDIM]/(BoundingBox.maxX-BoundingBox.minX);
	model.modelo.scaleY = Size.v[YDIM]/(BoundingBox.maxY-BoundingBox.minY);
	model.modelo.scaleZ = Size.v[ZDIM]/(BoundingBox.maxZ-BoundingBox.minZ);*/

	model.modelo.scaleX = Size.v[XDIM]/(BoundingBox.maxX-BoundingBox.minX);
	model.modelo.scaleY = 0.1f/(BoundingBox.maxY-BoundingBox.minY);
	model.modelo.scaleZ = Size.v[YDIM]/(BoundingBox.maxZ-BoundingBox.minZ);
#elif CHAR_USE_AABB
#endif
}

void CCharacter::FitMeshIntoBoundingBox()
{
#ifdef CHAR_USE_QUADTREE
#elif CHAR_USE_AABB
	CalculateAABB();
	for (unsigned int i=XDIM;i<WDIM;i++)
		Scale.v[i] = AABB[CHAR_BBMAX][i].Value - AABB[CHAR_BBMIN][i].Value;
#endif
}

#ifdef CHAR_USE_QUADTREE
void CCharacter::FitBoundingBoxArroundMesh(CMesh3d &model){

	CalculateAABB(model);

	Size.Set(BoundingBox.maxX-BoundingBox.minX,
			 BoundingBox.maxY-BoundingBox.minY,
			 BoundingBox.maxZ-BoundingBox.minZ);
}
#endif

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

	#ifdef CHAR_USE_QUADTREE	
	Vector PositionPrev(Position);
	#endif

#ifdef UGKPHY_EULER
	Speed	 += Acceleration*ms;
	Position += Speed*ms;

#elif UGKPHY_VERLET
	// Recalculate the new position of the character
	Aux = Position;

	//P(t+At) = 2*P(t)
	Position += Position;

	//P(t+At) = 2*P(t) - P(t-At)
	Position -= PositionPrev;
	PositionPrev = Aux;

	Aux = Acceleration*ms;
	Speed += Aux;

	//P(t+At) = 2*P(t) - P(t-At) + P''(t)*At*At
	Aux *= ms;
	Position += Aux;
#endif
}

/**
	* function to move to an absolute position to the character or ship to a position x, y, z
*/
void CCharacter::MoveTo		(float x, float y, float z)	///Moves to the absolute coordinate x,y,z
{
	Position.Set(x, y, z); //actualizar la posicion del caracter

	///Updating the bounding box
#ifdef CHAR_USE_AABB
	UpdateAABB ();
#endif
}

/**
	* function to move the character or ship to an absolute position from the current position
*/
void CCharacter::MoveTo		(Vector &Move)				///Moving to an absolute position. It does not matter where the character is
{
	Position = Move; /// Update position vector based on the Move[x,y,z]

	#ifdef CHAR_USE_AABB
	UpdateAABB (); ///Updating the bounding box
	#endif
}

/**
	* function to move the character or ship to a position based on x, y, z units from the current position
*/
void CCharacter::MoveRelTo	(float x, float y, float z)	///Moves x,y,z units from the to the current position
{
	Position += Vector(x, y, z); /// update the position x by increasing x units of the character

	///Updating the bounding box
	#ifdef CHAR_USE_AABB
	UpdateAABB ();
	#endif
}

/**
	* function to move the character or ship to a relative position from the current position 
*/
void CCharacter::MoveRelTo (Vector &Move)	///Moving relative to the character current Position
{
	///Adding relative movement
	Position += Move; 

	///Updating the bounding box
	#ifdef CHAR_USE_AABB
	UpdateAABB ();
	#endif
}

void CCharacter::ReceiveMessage(RTDESK_CMsg *pMsg){
}
