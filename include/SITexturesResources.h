/*Definition of the class Texture Resurces for S.I.
  General class to manage all the textures for 2D and 3D
  Prefix: CTM_

	@author Ramon Molla
	@version 2011-10

*/

#include <resource.h>
#include <TexturesManager.h>
#include <MeshManager.h>
#include <TextureAnimationManager.h>

#ifndef SI_TEXTURES_RESOURCES
#define SI_TEXTURES_RESOURCES

typedef enum
{
	CTM_3D_BAND,
	CTM_BAND,
	CTM_BONUS1,				//(PNG)
	CTM_BRICK,				//Only one green PIXEL
	CTM_BRICK3D,
	CTM_CADRE_CENTRE,
	CTM_CADRE_CENTRE_LONG,
	CTM_CADRE_ENERGY,		//(PNG)
	CTM_CADRE_ENERGY_CONTOUR,	//(PNG)
	CTM_EARTH_MOON_BCKG,	//(JPG)
	CTM_EQUALIZER,			//(PNG)
	CTM_EQUALIZER_BAND,
	CTM_GALAXY_BCKG,		//(JPG)
	CTM_HEALTH_BAR,
	CTM_HISCORE,			//Text "Hiscores"
	CTM_LIVES,				//Text "Lives"
	CTM_LOADING,
	CTM_NUMBER0,
	CTM_NUMBER1,
	CTM_NUMBER2,
	CTM_NUMBER3,
	CTM_NUMBER4,
	CTM_NUMBER5,
	CTM_NUMBER6,
	CTM_NUMBER7,
	CTM_NUMBER8,
	CTM_NUMBER9,
	CTM_PARTICLE,
	CTM_PARTICLE2,			//EXPLOSION
	CTM_PARTICLE3,
	CTM_PARTICLE3_PNG,
	CTM_PARTICLE3_3D,
	CTM_PARTICLE4,
	CTM_PAUSED,				//Text "Paused"
	CTM_PLANET_BCKG,		//Planet background
	CTM_PLAYER3D,			//Texture to indicate lives pending
	CTM_PLAYER_SHOOT,
	CTM_PLAYER_SHOOT3D,
	CTM_PLAYER_SHOOT3D_DARK_BCKG,
	CTM_POWERUP,
	CTM_PRESSKEY,			//Text "Press any key to start"
	CTM_SCORE,				//Text "Score"
	CTM_SHIP_SHOOT,
	CTM_SHIP1_1,			//First photogram of 2D game mode animation for the ship type 1
	CTM_SHIP2_1,			//The same for the second type of ship
	CTM_SHIP3_1,			//The same for the third  type of ship
	CTM_SHIP1_2,			//second photogram of 2D game mode animation for the ship type 1
	CTM_SHIP2_2,			//The same for the second type of ship
	CTM_SHIP3_2,			//The same for the third  type of ship
	CTM_SHOOT_3D_BASE_DOUBLE,
	CTM_SKYBOX_BCKG,		//Sky box Space background
	CTM_SKYBOX_BCKG_JPG,	//Sky box Space background
	CTM_SPACE_BCKG,			//Space background
	CTM_SPHERE,
	CTM_SUPPLYSHIP_SHOOT,
	CTM_TEXT_WEAPON,
	CTM_TEXT_BONUS,
	CTM_PLAYER3D2,
	CTM_MAX_TEXTURE			//For accounting purpouses only	
} CTM_Textures;


//Enumerate according to the order that have been programed in initialization.html
//typedef enum             
//{
//	CTM_ANI2SHIP_HIGH,
//	CTM_ANI2SHIP_MIDDLE,
//	CTM_ANI2SHIP_LOW,
//	CTM_MAX_ANIMATION		//For accounting purpouses only
//} CTM_Animations;

extern void LoadTextures(void);
extern void StartAnimations(void);
extern void ReserveTexturesMem(void);
extern void AsignaTextures(void);

#endif