/*/////////////////////////////////////////////////////////////////////////////////////
//
// Explosion declarations
//

	@author Ramon Molla
	@version 2011-09-11
*/
#include <math.h>
#include <UGKPhysics.h>
#include "Explosion.h"
#include <Textures\TexturesManager.h>
#include "SIGame.h"

extern CSIGame Game; /// call the global variable of the game
using namespace UGK;

/**
	* Constructor for the explosion of a character
    * Starts the life of a Character
    * This Explosion is for any given time.
    * @param CChraracter C: is an input to the function
    * @param float Life represents the amount of life Character
    * @param Particle used in a particle system for a Character.
    * It takes a Loop to recognize the speed of C and assign to the particle velocity
    * It takes a Loop to recognize the position of C and assign to the particle velocity
    * @ The load function based on the Particle Character
*/
void CExplosion::Init(CCharacter *C)
{
	Init();
	Life	= float((rand()%100)+500); // vida de la particula
	
	Position = C->Position;
	Speed = C->Speed;
	for (int loop=0;loop<CPS_MAX_PARTICLES;loop++)	/// Initializes all the particles
	{
		Particle[loop].Speed	= Speed;			/// Speed and position for all the particles in the particle system is the same as the character they belong to
		Particle[loop].Position = Position;
	}
}

/**
	* Constructor for the Explosion
    * Default parameters starts disabled
    * @param Active: represents the default state is false
    * @param Alive: is alive and if default is false
    * @param Life: represents the amount of life by default starts-1.0f
    * @param PhysicMode: which is the explosion that is visible but not touchable
    * This function Explosion Starts a Character
*/
void CExplosion::Init(void)
{
	Alive		= true;
	Active		= false;
	Life		= -1.0f;
	PhysicMode	= UGKPHY_LIGHT;	//Visible but not touchable
	///No AABB initialization is required since explosions do not have to collide to anything. They are a consequence of a character collision
}

/**
	* Function for updating when is a Explosion
    * This function depends on the living state of the particle if true
    * @param float V: represents particle velocity for the new Particle
    * @param float Angle: represents the angle for the new particle Particle
    * It traces the life of particles and if they are alive is greater than 0.0f
    * If the particle is alive move the position of the particle in the xy plane, and then slow down depending on RenderMode Particle.counteri 2D
    * After that life is reduced to the particle and if life is a burning creates new particle
    * After the initial V and 2D RenderMode depending Angle for partiuclas
    * And finally reduces the life of Character
    * @The function recalculates the particles of a Character and is destroying the once
*/
void CExplosion::Update(void)
{
	float V, Angle;
	unsigned int loop;

	if (Alive)
	{
		Alive = false;
		for (loop=0; loop<CPS_MAX_PARTICLES; loop++)				// Loop Through All The Particles
		{
			//Check for particles alive. If no one is alive, explosion is finished
			if (Particle[loop].Life > 0.0f)
			{
				Alive = true;

				// Move the particles
				Particle[loop].Position.v[XDIM] += (Particle[loop].Speed.v[XDIM] /250.0f) * 0.075f * (float)Game.RenderTemp;// Move On The X Axis By X Speed
				Particle[loop].Position.v[YDIM] += (Particle[loop].Speed.v[YDIM] /250.0f) * 0.075f * (float)Game.RenderTemp;// Move On The Y Axis By Y Speed
				//Explosion.Particle[loop].Position.v[ZDIM]+=Explosion.Particle[loop].zi/250;				// Move On The Z Axis By Z Speed

				// Slow down the particles
				Particle[loop].counteri += (3 * (float)Game.RenderTemp) / 40.0f ;
				if ( Particle[loop].counteri > 0.99)
				{
					if (CHAR_2D == RenderMode)
					{
						Particle[loop].Speed.v[XDIM] *= 0.88f;//.90
						Particle[loop].Speed.v[YDIM] *= 0.88f;
						//Particle[loop].zi*=.99;
					}
					else
					{
						Particle[loop].Speed.v[XDIM] *= 0.90f;//.94
						Particle[loop].Speed.v[YDIM] *= 0.90f;
						//Particle[loop].zi*=.99;
					}
					Particle[loop].counteri = 0;
				}

				// Particle life
				Particle[loop].Life -= Particle[loop].FadingSpeed * 0.075f * (float)Game.RenderTemp;// Reduce Particles Life By 'Fade'

				// Creation of new Particle, if Particle is burned out
				if (Particle[loop].Life < 0.0f && Life > 0.0f)
				{
					Particle[loop].Life				= 1.0f;				// Give It New Life
					Particle[loop].FadingSpeed		= float(rand()%100)/10000 + 0.015f;// Random Fade Value
					Particle[loop].Position.v[XDIM]	= Position.v[XDIM];	// Center On X Axis
					Particle[loop].Position.v[YDIM]	= Position.v[YDIM];	// Center On Y Axis
					Particle[loop].Position.v[ZDIM]	= Position.v[ZDIM];	// Center On Z Axis
				}
				V = (float((rand()%3)+2));								// Velocity of the particles
				if (CHAR_2D != RenderMode)
					V *= 1.5;
				Angle = float(rand()%360);								// Angle of the particles

				Particle[loop].Speed.v[XDIM] = sin(Angle) * V;		// x acceleration
				Particle[loop].Speed.v[YDIM] = cos(Angle) * V;		// y acceleration
				//Particle[loop].zi = ((rand()%10)-5)/5;
				// Explosion life duration diminishes
				Life-= ((3 * (float)Game.RenderTemp) /40.0f);
			}	//If Particle Life > 0.0f
		}	//For every Particle
	}	//Alive
}
