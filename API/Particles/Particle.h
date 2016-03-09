/**
 *	PROJECT - Particle system management
 *
 *	Particle.h
 *	
 *	Copyright (c) 2011. All rights reserved.
 *
 *	@author Ramon Molla <rmolla@dsic.upv.es>
 *	Prefix: CP_
 *	HISTORY:
 *	@version 1.0 (Dec, 2011)
 */

#ifndef CP_PARTICLE
#define CP_PARTICLE

#include "Character.h"

/**
 * CParticle
 * This class supports only a single particle that belongs to a given particle system
 *
 * @todo Finish to implement basic functionality
 */
class CParticle: public CCharacter
{
public:
	/// Particle Life
	float	Life;
	/// Fading Speed
	float	FadingSpeed;
	/// Counter to know when to reduce acceleration
	float	counteri;
	
	//Methods
	void Init (void);

	CParticle(){Init();};	//Constructor of the class

	///Shows the particle on the screen
	void		Render				(void);
	///Change the way the particle is going to be rendered on the screen
	inline void ChangeRenderMode	(CHAR_RENDER_MODE Mode) {RenderMode = Mode;};
};
#endif