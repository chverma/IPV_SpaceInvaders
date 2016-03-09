/*Class Texture Manager
  General class to manage all the textures for 2D and 3D
  Prefix: CTM_

	@author Ramon Molla
	@version 2013-06
*/

#include <windows.h>
#include <gl.h>

#include "SITexturesResources.h"


extern CTexturesManager	TexturesManager;
extern CMeshesManager	MeshesManager;
extern CTextureAnimationManager	AnimationsManager;

/**   
   @fn LoadTextures
   Load basic application textures.
   IMPORTANT: This function may be programed before "initialization.html" parser.
*/
void LoadTextures(void)
{
	TexturesManager.Textures.clear();

	//TexturesManager.SetFilesPath(".\\images\\");

	for (unsigned int indexList=0; indexList<CTM_MAX_TEXTURE; indexList++)
	{
		switch(indexList)
		{
		case CTM_3D_BAND:
			TexturesManager.CreateTexture("barre3d.bmp", CTM_3D_BAND);
			break;
		case CTM_BAND:
			TexturesManager.CreateTexture("barre.bmp", CTM_BAND);
			break;
		case CTM_BONUS1:
			TexturesManager.CreateTexture("bonus1.bmp", CTM_BONUS1);
			break;
		case CTM_BRICK:
			TexturesManager.CreateTexture("pixel_vert.bmp", CTM_BRICK);
			break;
		case CTM_BRICK3D:
			TexturesManager.CreateTexture("pixel_vert3d.bmp", CTM_BRICK3D);
			break;
		case CTM_CADRE_CENTRE:
			TexturesManager.CreateTexture("cadre_centre.bmp", CTM_CADRE_CENTRE);
			break;
		case CTM_CADRE_CENTRE_LONG:
			TexturesManager.CreateTexture("cadre_centre_long.bmp", CTM_CADRE_CENTRE_LONG);
			break;
		case CTM_CADRE_ENERGY:
			TexturesManager.CreateTexture("cadre_energy.bmp", CTM_CADRE_ENERGY);
			break;
		case CTM_CADRE_ENERGY_CONTOUR:
			TexturesManager.CreateTexture("cadre_energy_contour.bmp", CTM_CADRE_ENERGY_CONTOUR);
			break;
		case CTM_EARTH_MOON_BCKG:
			TexturesManager.CreateTexture("fond_earth_moon.bmp", CTM_EARTH_MOON_BCKG);
			break;
		case CTM_EQUALIZER:
			TexturesManager.CreateTexture("equalizer.bmp", CTM_EQUALIZER);
			break;
		case CTM_EQUALIZER_BAND:
			TexturesManager.CreateTexture("equalizer_barre.bmp", CTM_EQUALIZER_BAND);
			break;
		case CTM_GALAXY_BCKG:
			TexturesManager.CreateTexture("fond_galaxy1.bmp", CTM_GALAXY_BCKG);
			break;
		case CTM_HEALTH_BAR:
			TexturesManager.CreateTexture("textHealthBar.bmp", CTM_HEALTH_BAR);
			break;
		case CTM_HISCORE:
			TexturesManager.CreateTexture("hiscore.bmp", CTM_HISCORE);
			break;
		case CTM_LIVES:
			TexturesManager.CreateTexture("lives.bmp", CTM_LIVES);
			break;
		case CTM_LOADING:
			TexturesManager.CreateTexture("game_loading.bmp", CTM_LOADING);
			break;
		case CTM_NUMBER0:
			TexturesManager.CreateTexture("chiffre0.bmp", CTM_NUMBER0);
			break;
		case CTM_NUMBER1:
			TexturesManager.CreateTexture("chiffre1.bmp", CTM_NUMBER1);
			break;
		case CTM_NUMBER2:
			TexturesManager.CreateTexture("chiffre2.bmp", CTM_NUMBER2);
			break;
		case CTM_NUMBER3:
			TexturesManager.CreateTexture("chiffre3.bmp", CTM_NUMBER3);
			break;
		case CTM_NUMBER4:
			TexturesManager.CreateTexture("chiffre4.bmp", CTM_NUMBER4);
			break;
		case CTM_NUMBER5:
			TexturesManager.CreateTexture("chiffre5.bmp", CTM_NUMBER5);
			break;
		case CTM_NUMBER6:
			TexturesManager.CreateTexture("chiffre6.bmp", CTM_NUMBER6);
			break;
		case CTM_NUMBER7:
			TexturesManager.CreateTexture("chiffre7.bmp", CTM_NUMBER7);
			break;
		case CTM_NUMBER8:
			TexturesManager.CreateTexture("chiffre8.bmp", CTM_NUMBER8);
			break;
		case CTM_NUMBER9:
			TexturesManager.CreateTexture("chiffre9.bmp", CTM_NUMBER9);
			break;
		case CTM_PARTICLE:
			TexturesManager.CreateTexture("particle.bmp", CTM_PARTICLE);
			break;
		case CTM_PARTICLE2:			//EXPLOSION
			TexturesManager.CreateTexture("particle2.bmp", CTM_PARTICLE2);
			break;
		case CTM_PARTICLE3:
			TexturesManager.CreateTexture("particle3.bmp", CTM_PARTICLE3);
			break;
		case CTM_PARTICLE3_PNG:
			TexturesManager.CreateTexture("particle3.bmp", CTM_PARTICLE3_PNG);
			break;
		case CTM_PARTICLE3_3D:
			TexturesManager.CreateTexture("particle3_3d.bmp", CTM_PARTICLE3_3D);
			break;
		case CTM_PARTICLE4:			//REACTORS
			TexturesManager.CreateTexture("particule4.bmp", CTM_PARTICLE4);
			break;
		case CTM_PAUSED:
			TexturesManager.CreateTexture("Paused.bmp", CTM_PAUSED);
			break;
		case CTM_PLANET_BCKG:		//Planet background
			TexturesManager.CreateTexture("fond_planete.bmp", CTM_PLANET_BCKG);
			break;
		case CTM_PLAYER3D:			//This texture is to count lives
			TexturesManager.CreateTexture("player3d.tga", CTM_PLAYER3D);
			break;
		case CTM_PLAYER_SHOOT:
			TexturesManager.CreateTexture("tir.bmp", CTM_PLAYER_SHOOT);
			break;
		case CTM_PLAYER_SHOOT3D:
			TexturesManager.CreateTexture("tir_3d_base.bmp", CTM_PLAYER_SHOOT3D);
			break;
		case CTM_PLAYER_SHOOT3D_DARK_BCKG:
			TexturesManager.CreateTexture("tir_3d_base_dark_backgrd.bmp", CTM_PLAYER_SHOOT3D_DARK_BCKG);
			break;
		case CTM_POWERUP:
			TexturesManager.CreateTexture("powerup.bmp", CTM_POWERUP);
			break;
		case CTM_PRESSKEY:
			TexturesManager.CreateTexture("PressKey.bmp", CTM_PRESSKEY);
			break;
		case CTM_SCORE:				//Text "Score"
			TexturesManager.CreateTexture("score.bmp", CTM_SCORE);
			break;
		case CTM_SHIP_SHOOT:
			TexturesManager.CreateTexture("tir_ship.bmp", CTM_SHIP_SHOOT);
			break;
		case CTM_SHIP1_1:			//First photogram of 2D game mode animation for the ship type 1
			TexturesManager.CreateTexture("ship1.bmp", CTM_SHIP1_1);
			break;
		case CTM_SHIP2_1:			//The same for the second type of ship
			TexturesManager.CreateTexture("ship2.bmp", CTM_SHIP2_1);
			break;
		case CTM_SHIP3_1:			//The same for the third type of ship
			TexturesManager.CreateTexture("ship3.bmp", CTM_SHIP3_1);
			break;
		case CTM_SHIP1_2:			//second photogram of 2D game mode animation for the ship type 1
			TexturesManager.CreateTexture("ship1_b.bmp", CTM_SHIP1_2);
			break;
		case CTM_SHIP2_2:
			TexturesManager.CreateTexture("ship2_b.bmp", CTM_SHIP2_2);
			break;
		case CTM_SHIP3_2:	
			TexturesManager.CreateTexture("ship3_b.bmp", CTM_SHIP3_2);
			break;
		case CTM_SHOOT_3D_BASE_DOUBLE:
			TexturesManager.CreateTexture("tir_3d_base_double.bmp", CTM_SHOOT_3D_BASE_DOUBLE);
			break;
		case CTM_SKYBOX_BCKG:		//Sky box Space background
			TexturesManager.CreateTexture("fond_skybox2.bmp", CTM_SKYBOX_BCKG);
			break;
		case CTM_SKYBOX_BCKG_JPG:	//Sky box Space background
			TexturesManager.CreateTexture("fond_skybox.bmp", CTM_SKYBOX_BCKG_JPG);
			break;
		case CTM_SPACE_BCKG:		//Space background
			TexturesManager.CreateTexture("fond_espace.bmp", CTM_SPACE_BCKG);
			break;
		case CTM_SPHERE:
			TexturesManager.CreateTexture("sphere.bmp", CTM_SPHERE);
			break;
		case CTM_SUPPLYSHIP_SHOOT:
			TexturesManager.CreateTexture("tir_supplyship.bmp", CTM_SUPPLYSHIP_SHOOT);
			break;
		case CTM_TEXT_WEAPON:
			TexturesManager.CreateTexture("text_weapon.bmp", CTM_TEXT_WEAPON);
			break;
		case CTM_TEXT_BONUS:
			TexturesManager.CreateTexture("text_bonus.bmp", CTM_TEXT_BONUS);
			break;
		
		}
	}
}

void StartAnimations(void){

	for (unsigned int i=0;i<AnimationsManager.Animations.size();i++){
		AnimationsManager.Animations[i]->Start();
	}

}
