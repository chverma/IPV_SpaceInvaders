/**	Definition physical structures and types

*	Prefix: UGKPHY_

*	@author Ramon Molla
*	@version 2013-05
*/

#ifndef UGKPHY_PHYSICS
#define UGKPHY_PHYSICS

///Determines the way the numerical integration is going to be performed
#define UGKPHY_EULER
//#define UGKPHY_VERLET
//#define UGKPHY_MIDDLEPOINT
//#define UGKPHY_RK4

/**
	* We define new types of variable direction movements
*/
typedef enum {
	UGKPHY_NO_DIRECTION,
	UGKPHY_UP,
	UGKPHY_DOWN,
	UGKPHY_RIGHT,
	UGKPHY_LEFT,
	UGKPHY_FRONT,
	UGKPHY_BACK,
	UGKPHY_MAX_DIRECTIONS
} UGKPHY_DIRECTION;

/**
*	\typedef UGKPHY_PHYSIC_MODE
*	The phisical way a character exists in the world
*/
typedef enum {
	UGKPHY_PHANTOM,			///<No render, No touchable: camera
	UGKPHY_INVISIBLE,			///<No render, touchable: collision objects,...
	UGKPHY_LIGHT,				///<Visible but not touchable
	UGKPHY_PHYSICAL,			///<Normal objects: can be rendered and touched
	UGKPHY_MAX_PHYSIC_MODE		///<For management purpouses only. Always at the end of the enumeration
} UGKPHY_PHYSIC_MODE;

/**
*	\typedef UGKPHY_CINEMATIC_ACTION
*	Different types of cinematic actions that can be peformed by the characters 
*	during the development of the game
*/
typedef enum {
	UGKPHY_TRASLATION,				///<Moving
	UGKPHY_ROTATION,				///<Rotating
	UGKPHY_SCALE,					///<Scaling
	UGKPHY_SIZE,
	UGKPHY_SPEED,					///<Change Speed value
	UGKPHY_ACCELERATION,			///<Change acceleration value
	UGKPHY_CENTER,
	UGKPHY_MAX_CINEMATIC_ACTION	///<For management purpouses only
} UGKPHY_CINEMATIC_ACTION;


#endif