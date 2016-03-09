/*	Definition of the class Explosion

	Prefix: CE_

	@author Ramon Molla
	@version 2011-11
*/

#ifndef CE_EXPLOSION   //Se borra la macro explocion
#define CE_EXPLOSION   //Se define la macro explocion

#include "ParticleSystem.h"

/**   
   * It defines the types of explosions that exist in the game
   * @param enum CE_EXPLOSION_TYPE which lists the types of explosion
*/
typedef enum
{
	CE_NO_EXPLOSION,	/// No explosion is allowed
	CE_SHIP_EXPLOSION,  /// Explosion of enemies
	CE_SUPPLYSHIP_EXPLOSION, /// Explosion of the Ship Supply
	CE_PLAYER_EXPLOSION,     /// Explosion of the character or ship
	CE_LASER_EXPLOSION,      /// Laser Gun Explosion of the Ship
	CD_MAX_EXPLOSION	///For management purpouses only
} CE_EXPLOSION_TYPE;

/**
	* Explotion class is defined with derivation or inheritance of the class CParticleSystem
    * Where is the constructor and update
    * @param subtype CE_EXPLOSION_TYPE: predefined variable for the types of explosion
*/
class CExplosion: public CParticleSystem
{
	
public:

	//Attributes
	CE_EXPLOSION_TYPE SubType;	///Type of predefined burst to be performed

	//Methods
	void Init (void);
	void Init (CCharacter *C);

	CExplosion(){Init();}		//Constructor of the class

	void Update		(void);

	///Shows the explosion on the screen
	// Uses the ones coded for the class Particle System
};

#endif
