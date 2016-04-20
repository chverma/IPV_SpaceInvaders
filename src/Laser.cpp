/**

* Laser class

* These values are by default. They have to be overwritten in the initializatio phase
* when reading the default values from the "initialization.html" file

*	@author Ramon Molla
*	@version 2011-11
*/

#include <math.h>
#include "Laser.h"  /// Call the library functions defined in the Laser
#include "Global.h"  /// definition of global variables is handled in the game

/**   
   * Constructor for Laser
*/
CLaser::CLaser()
{
	Init();
};		//Constructor

/**   
	* @fn bool CLaser::Init ()
	* Start the Laser
    * It starts after the laser has been taken in the course of the game
    * Also starts life variables, number of fire, explosion
    * The position and speed of the laser with respect to the ship
*/
bool CLaser::Init ()
{
	Type			=	CHARS_LASER; //se asigna el tipo de bonus que en este caso es el laser
	Health			=	CL_MAX_HEALTH; //Cantidad de vida del arma
	Hit_duration	=	CL_MAX_MAX_HIT_DUR; //la cantidad maxima de disparos que se puede realizar con el laser
	Alive			=	false;				///The very first time, when created at the beginning of the game, this device is not available
	
	Explosion.Init();  //Se inicia la explocion por defecto deshabilitado   

	//Space position and AABB
	Size.v[XDIM]			=	0.20;	//+ MeshesManager.Mesh[CHARS_LASER].rot.v[ZDIM]/250
	Size.v[YDIM]			=	0.25f;	//0.45f
	Size.v[ZDIM]			=	0.0f;

	RenderMode		= CHAR_3D; //Cambiar la nave en 3D
#ifndef CHAR_USE_QUADTREE
	InitializeAABB();			///Update by default the AABB relative to local coordinates
#endif	
	MoveTo(0.0f, 0.0f, 0.05f);	///Implicitly updates by default the AABB relative to local coordinates and collision detector also

	yi				=	0.00;  // movimiento de la nave en la coordenada y
	yi_speed		=	0.01f; // velocidad de la nave en la coordenada y
	yi_cos			=	0;     // variable coseno de nave en la coordenada y
	zi				=	0.00;  // flotamiento de la nave coordenada z para el modo 3D
	zi_speed		=	0.01f; // velocidad de la nave coordenada z para el modo 3D
	Speed.Reset();	//Sets all values to 0

	return true;
}

/**   
   * @fn AI_Init
   * Start Laser Artificial Intelligence
*/
void CLaser::AI_Init(void)
{
}

/**   
	* @fn AI_Dye
	* Death of Artificial Intelligence Laser
    * Deactivate the Laser and dies when it is destroyed or terminates the ammunition
*/
void CLaser::AI_Dye (void)
{
	Active = Alive  = false;
}

/**   
	* @fn AI_Explode
	* Artificial Intelligence Explosion of Laser
    * Explosion starts class Laser type
    * After Killing Laser Artificial Intelligence
*/
void CLaser::AI_Explode (void)
{
	Explosion.Init();
	Explosion.Active=true;
	AI_Dye(); //se mata a la IA del Laser
}	

/**   
	* @fn Collided
	* Shock
    * We define this function for a collision when a Character
    * @ Param CCharacter CollidedChar Shock represents the type Character
    * Depending on the type of shock that has produced 
	* (or firing of the enemy ship) is given a specific sound and global variables changes and game states
*/
void CLaser::Collided (CCharacter *CollidedChar)
{
	switch (CollidedChar->Type) //Crash type
	{
	case CHARS_SHIP: //Ship crash
	case CHARS_MISSIL: //Enemy missil impact
		if (Alive && !Immortal)
		{
			#ifdef DEBUG_MORTAL
			// Sound effect for the laser explosion
			SoundsManager.Play(CGS_EXPLOSION,UGKSND_VOLUME_80);				
			if (CSIG_PASSING_2_3D == Game.AI->GetState())	return;		// Laser is protected by the Bonus upgrade explosion
			AI_Explode();
			#endif
		}
		break;
	}
}

/**   
	* @fn Render
    * It defines all the actions to do when you get or lose a laser weapon
    * Also as changing 2D, 3D Laser with the Character or ship in the game
*/
void CLaser::Render (void)
{	
	///At this version, lasers can only be displayed when in 3D mode
	switch(RenderMode)
	{
	case CHAR_NO_RENDER:			//No render for this character
		return;
	case CHAR_2D:
		break;
	case CHAR_LINES3D:
	case CHAR_3D:
		if (Alive)
		{
			Mesh->modelo.pos.x = Position.v[XDIM];
			Mesh->modelo.pos.y = Position.v[YDIM];
			Mesh->modelo.pos.z = Position.v[ZDIM];
			Mesh->modelo.rot.x =  -270.0f;
			Mesh->modelo.rot.z =  asignedPlayer.Mesh->modelo.rot.z;
			//Mesh->modelo.scale = 0.15f;
			
			Mesh->modelo.Draw();
		}
		break;
	default: return;
	}
}

/**   
    *@fn ChangeRenderMode
	* @param CHAR_RENDER_MODE Mode represents the mode
    * Define the Game Mode in 2D, 3D, LINES3D depending on how you receive
*/
void CLaser::ChangeRenderMode(CHAR_RENDER_MODE Mode)
{
	RenderMode = Mode;
}

/**   
	* @fn RenderExplosion
	* Define all actions that must be made when the laser operates
    * Modification, reconstruction of Texture, particles of a Character in 2D or 3D
    * @return the Explosion of a character by its particles
*/
void CLaser::RenderExplosion(void)
{
	Matrix ModelView;

	float V, Angle;
	TexturesManager.Textures[CTM_PARTICLE2]->SetTexture();		//29 (créée) ou 91 (générée)

	//float UGKALG_ModelView[16];

	if (Explosion.Alive && UGKALG_3D == RenderMode)
	{
		Explosion.Alive = false;
		for (unsigned int loop=0; loop<CPS_MAX_PARTICLES; loop++)				// Loop Through All The Particles
		{
			//Check for particles alive. If no one alive, explosion is finished
			if (Explosion.Particle[loop].Life > 0.0f)
			{
				Explosion.Alive = true;

				float x = Explosion.Particle[loop].Position.v[XDIM];		// Grab Our Particle X Position
				float y = Explosion.Particle[loop].Position.v[YDIM];		// Grab Our Particle Y Position
				//float z = Explosion.Particle[loop].Position.v[ZDIM];		// Grab Our Particle Z Position
				float z = 0.05;//Navy.Ship[i]->Position.v[ZDIM];				// Grab Our Particle Z Position

				// Draw The Particle Using Our RGB Values, Fade The Particle Based On It's Life
				glColor4f(1.0f,1.0f,1.0f,Explosion.Particle[loop].Life);

				float size = cos(Explosion.Particle[loop].Life / 2) / 1.6; //1.8

				if (UGKALG_3D == RenderMode)
				{
					size *= 1.2;
					// BILLBOARDING (particles are perpendicular to the camera)
					// --------------------------------------------------------------
					if (Application.Window.Billboard)	//&& !viewport2
					{
						glPushMatrix();
						glTranslatef ( x, y, 0 );

						// get the current UGKALG_ModelView matrix
						glGetFloatv(GL_MODELVIEW_MATRIX , ModelView.M);

						ModelView.NeutralModelView();

						glBegin(GL_TRIANGLE_STRIP);							// Build Quad From A Triangle Strip
						glTexCoord2f(1,1); glVertex3f(+ size,+ size,z);		// Top Right
						glTexCoord2f(0,1); glVertex3f(- size,+ size,z);		// Bottom Right
						glTexCoord2f(1,0); glVertex3f(+ size,- size,z);		// Top Left
						glTexCoord2f(0,0); glVertex3f(- size,- size,z);		// Bottom Left
						glEnd();											// Done Building Triangle Strip

						glPopMatrix();
					}
					else // No billboarding (explosion ship)
					{
						glBegin(GL_TRIANGLE_STRIP);							// Build Quad From A Triangle Strip
						glTexCoord2f(1,1); glVertex3f(x + size,y + size,z);	// Top Right
						glTexCoord2f(0,1); glVertex3f(x - size,y + size,z);	// Bottom Right
						glTexCoord2f(1,0); glVertex3f(x + size,y - size,z);	// Top Left
						glTexCoord2f(0,0); glVertex3f(x - size,y - size,z);	// Bottom Left
						glEnd();											// Done Building Triangle Strip
					}
				}
				else // game 2D
				{
					glBegin(GL_TRIANGLE_STRIP);								// Build Quad From A Triangle Strip
					glTexCoord2f(1,1); glVertex3f(x + size,y + size,z);		// Top Right
					glTexCoord2f(0,1); glVertex3f(x - size,y + size,z);		// Bottom Right
					glTexCoord2f(1,0); glVertex3f(x + size,y - size,z);		// Top Left
					glTexCoord2f(0,0); glVertex3f(x - size,y - size,z);		// Bottom Left
					glEnd();												// Done Building Triangle Strip
				}

/*				if (viewport2) // For the Mini-screen, we don't recalculate the movements
					continue;
*/
				// Move the particles
				Explosion.Particle[loop].Position.v[XDIM] += (Explosion.Particle[loop].Speed.v[XDIM] /250) * Game.RenderTemp;// Move On The X Axis By X Speed
				Explosion.Particle[loop].Position.v[YDIM] += (Explosion.Particle[loop].Speed.v[YDIM] /250) * Game.RenderTemp;// Move On The Y Axis By Y Speed
				//Explosion.Particle[loop].Position.v[ZDIM]+=Explosion.Particle[loop].zi/250;				// Move On The Z Axis By Z Speed

				// Slow down the particles
				Explosion.Particle[loop].counteri += (3 * Game.RenderTemp) / 40 ;
				if ( Explosion.Particle[loop].counteri > 0.99)
				{
					if (UGKALG_3D == RenderMode)
					{
						Explosion.Particle[loop].Speed.v[XDIM] *= 0.90f;//.94
						Explosion.Particle[loop].Speed.v[YDIM] *= 0.90f;
						//Explosion.Particle[loop].zi*=.99;
					}
					else
					{
						Explosion.Particle[loop].Speed.v[XDIM] *= 0.88f;//.90
						Explosion.Particle[loop].Speed.v[YDIM] *= 0.88f;
						//Explosion.Particle[loop].zi*=.99;
					}
					Explosion.Particle[loop].counteri = 0;
				}

				// Particle life
				Explosion.Particle[loop].Life -= Explosion.Particle[loop].FadingSpeed * Game.RenderTemp;// Reduce Particles Life By 'Fade'

				// Creation of new Particle, if Particle is burned out
				if (Explosion.Particle[loop].Life < 0.0f && Explosion.Life > 0.0f)
				{
					Explosion.Particle[loop].Life			= 1.0f;				// Give It New Life
					Explosion.Particle[loop].FadingSpeed	= float(rand()%100)/10000 + 0.015f;// Random Fade Value
					Explosion.Particle[loop].Position.v[XDIM]		= Position.v[XDIM];	// Center On X Axis
					Explosion.Particle[loop].Position.v[YDIM]		= Position.v[YDIM];	// Center On Y Axis
					Explosion.Particle[loop].Position.v[ZDIM]		= Position.v[ZDIM];	// Center On Z Axis
				}
				V = (float((rand()%3)+2));								// Velocity of the particles
				if (UGKALG_3D == RenderMode)
					V *= 1.5;
				Angle = float(rand()%360);								// Angle of the particles

				Explosion.Particle[loop].Speed.v[XDIM] = sin(Angle) * V;		// x acceleration
				Explosion.Particle[loop].Speed.v[YDIM] = cos(Angle) * V;		// y acceleration
				//Explosion.Particle[loop].zi = ((rand()%10)-5)/5;
				// Explosion life duration diminishes
				Explosion.Life-= ((3 * Game.RenderTemp) /40);
			}	//If Particle Life > 0.0f
		}	//For every Particle
	}	//Explosion.Alive
}
