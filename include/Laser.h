/**	Definition of the class Laser
	Class prefix CL_

	@author Ramon Molla
	@version 2011-09
*/

#ifndef CL_CLASS_LASER /// Macro laser weapon is erased
#define CL_CLASS_LASER /// Macro laser weapon is defined

#include <Particles\Explosion.h> /// Library to define the explosion of the laser weapon

#define CL_MAX_HEALTH			 20	///Maximun amount of health of a given Laser by default
#define CL_MAX_MAX_HIT_DUR		500	///Maximun amount of time when a given Laser is hit
#define CL_INFINITE_LIVES		 -1 ///Constant value for ethernal life

/**
	@class CLaser: public CCharacter

	* Laser class is defined with derivation or inheritance of the class CCharacter
    * Here we define the start and death of the movement of the laser IA
    * You show how the laser and the explosion of the same on the screen depending on the coordinates
	@author Ramon Molla
	@version 2011-09
*/
class CLaser: public CCharacter
{
	//Attributes
public:
	float		yi;				// cuando se mueve la nave en la coordenada y
	float		yi_speed;		// la velicidad de la nave en la coordenada y
	float		yi_cos;			// variable coseno de nave en la coordenada y
	float		zi;				// el flotamiento de la nave coordenada z para el modo 3D
	float		zi_speed;		// la velocidad de la nave coordenada z para el modo 3D
	CExplosion	Explosion;		///Cuando los barcos pierde su vida estalla. Este es el sistema de partículas para realizar la explosión
	CCharacter asignedPlayer;
	//Methods
	bool Init ();	//Used when all the values are initialized by default
					//when reading the global initialization game file
					//Returns true if the initialization of the attribute has been correctly done

	CLaser();		//Constructor

	///Character AI virtual methods rewritten
	void AI_Init	(void);	///Default Artificial Intelligence character initialization
	void AI_Dye		(void);

	void AI_Explode (void);
	
	//Physics
	void Collided (CCharacter *CollidedChar);

	///Shows the laser on the screen
	void Render				(void);
	///Change the way the player's laser is going to be rendered on the screen
	void ChangeRenderMode	(CHAR_RENDER_MODE);
	void RenderExplosion	(void);

	void setAsignedPlayer(CCharacter aPlayer){asignedPlayer=aPlayer;};
};

#endif
