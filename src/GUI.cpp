/*	Definition of the class Interface

	Prefix: CI_

	@author Ramon Molla
	@version 2011-11
	In this file all the functions of interface (display windows) player
*/

//#define	DEBUG_PASSAGE_3D
#include <GUI.h>
#include <Global.h>
#include "FontsManager.h"
#include <Meshes\MeshManager.h>
#include <glext.h>
#include <UGKString.h>
#include <UGKalgebra.h>

char  CGUI_ParserMsgs[CGUI_MAX_MSGS][CGUI_MSG_MAX_LONG] = 
{
	"Invalid command",
	"Type a variable name",
	"Average error 3D.01 (3D transition)"
};

extern CPlayer	Player[CP_MAX_PLAYERS];	// PLAYERS AVAILABLE

// extern because of ship FSMs
extern CSIGame	Game; // LLamar a la funcion global juego
extern int		id_tir;
extern int		CurrentPlayer;  //llamar al jugador actual

CConsole::CConsole()
{
	Line			= 0;	// CONSOLE INDEX DE CONSOLE_TEXT[][]
	
	strcpy(Command[0], UGKS_EMPTY_STRING);
	strcpy(Command[1], UGKS_EMPTY_STRING);

	strcpy(Output[0], UGKS_EMPTY_STRING);
	strcpy(Output[1], UGKS_EMPTY_STRING);

	ViewportPush	= -999.0;	// CONSOLE VIEWPORT MINI-SCREEN

	Active		= false;	// CONSOLE inactive
	ComeIn		= true;		// CONSOLE on screen
	Moving		= false;	// CONSOLE not moving
	Start		= false;	// CONSOLE not starting
}

/**   
   * Initializer for the Interfaces  
*/
void CGUI::Init ()
{
	Fading		= 1.0;
	UGK::Vector origin;
	
	origin.v[XDIM] = 4.0;
	origin.v[YDIM] = -6.5;
	boometer = new CBoometer(origin, 0.5, 3, CTM_HEALTH_BAR);
}

/**   
   * Show Spectrum 3D
*/
void CGUI::display_spectrum(void)
{
/*
	static int		choix_anim		= 0;
	float			beat_detection	= 0;
	static float	beat_scale		= 0;
	float			beat_sum		= 0;
	static float	xrot			= 0;
	static float	yrot			= 0;
	static float	zrot			= 0;

	// on récupère le spectrum
	float* spec = FSOUND_DSP_GetSpectrum();

	for (int i=0;i<=70;i++)
		beat_detection += spec[i];
	beat_detection /= 70 - 0.0;

	if (beat_detection > beat_scale + 0.0)
		beat_scale = beat_detection;

	beat_scale -= 0.007;

	char my_string[300]=UGKS_EMPTY_STRING;
	_ltoa(beat_scale, my_string, 10);
	//OutputDebugString(_ltoa(beat_scale, my_string, 10));
	//OutputDebugString(_ltoa(10.0, my_string, 10));
	//fontDrawString (100, 300, "beat float =  %f, beat string = %s", beat_scale,my_string);
	//

	if (CHAR_2D != Game.RenderMode && choix_anim == 0)
	{
		glPushMatrix();

		glLoadIdentity ();
		gluLookAt(	0.0, 0.0, 20.0,										// l'oeil
					0.0, 0.0,  0.0,										// le centre
					0.0, 1.0,  0.0	);									// le haut

		glTranslatef ( 0.0, 0.0, 8 );

		beat_sum = 0.012f + beat_scale * 0.07f;

		MeshesManager.Mesh[MESH2PLAYER].scale = beat_sum;

		// textes
		FontsManager.SetFontSize();fontDrawString (260, Application.Window.Height - FontsManager.FontVerticalSpace*2.9 , "beat detec =  %f", beat_detection);
		FontsManager.SetFontSize();fontDrawString (260, Application.Window.Height - FontsManager.FontVerticalSpace*4.2 , "beat scale =  %f", beat_scale);
		FontsManager.SetFontSize();fontDrawString (260, Application.Window.Height - FontsManager.FontVerticalSpace*5.5 , "beat sum   =  %f", beat_sum);
	
		// tambourin
		MeshesManager.Mesh[MESH2PLAYER].pos.v[XDIM] =  5.2;
		MeshesManager.Mesh[MESH2PLAYER].pos.v[YDIM] = -3.7;

		MeshesManager.Mesh[MESH2PLAYER].rot.v[XDIM] +=  0.8f;
		MeshesManager.Mesh[MESH2PLAYER].rot.v[YDIM] +=  0.4f;
		MeshesManager.Mesh[MESH2PLAYER].rot.v[ZDIM] +=  0.6f;

		// LUMIERE
		static float ambient[] =			{0.8, 0.8, 0.8, 1.0}; //0.9
		static float diffuse[] =			{0.2, 0.2, 0.2, 1.0}; //0.5
		glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);

		// LUMIERE (MATERIAU)
		static float mat_ambient[] = {0.5, 0.5, 0.5, 1.0}; //0.7 ou 0.8
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
		static float mat_diffuse_ships[] = {0.1, 0.1, 0.1, 1.0};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse_ships);

		// LUMIERE (POSITION)
		static float position_player[] = {1.0, 1.0, 15.0, 0.0};
		glLightfv(GL_LIGHT1, GL_POSITION, position_player);

		MeshesManager.Mesh[MESH2PLAYER].Draw();

		glPopMatrix();
	}
*/
}

/**   
	* @fn display_ring_2d_to_3d
	* Display changes 2D to 3D textures
    * This function allows you to see the changes undergone by the texture to get a bonus
    * For this function gives the player and the type of bonus that was
    * Bonus Depending on the texture changes from 2D to 3D or not the change is made
    * You define which is also sound for such a change
*/
void CGUI::display_ring_2d_to_3d()
{
	// The bonus has been catched and the transition to 3D view mode is on
	if (CSIG_PASSING_2_3D == Game.AI->GetState())	
	{
		// on gère le tremblement du vaisseau du joueur et du cercle
		static float player_yi_acc = 0.05;

		if (BonusManager.Bonus[CBN_BONUS_3D].Explosion.Life > 2.0f)
			Player[CurrentPlayer].yi_speed += player_yi_acc;
		else Player[CurrentPlayer].yi_speed -= player_yi_acc;

		if (Player[CurrentPlayer].yi_speed >= 0.6)
			Player[CurrentPlayer].yi_speed = 0.6;

		else if (Player[CurrentPlayer].yi_speed <= -0.4 && CHAR_2D != Game.RenderMode)
			Player[CurrentPlayer].yi_speed = -0.4;

		Player[CurrentPlayer].yi_cos += Player[CurrentPlayer].yi_speed;

		if (Player[CurrentPlayer].yi_cos >= 3.14 || Player[CurrentPlayer].yi_cos <= -3.14)
			Player[CurrentPlayer].yi_speed *= -1;

		if (BonusManager.Bonus[CBN_BONUS_3D].Explosion.Life >= 1.0f)
			Player[CurrentPlayer].yi = cosf(Player[CurrentPlayer].yi_cos * 50) / 40;	// cosf(a * b) / c - d : c=amplitude, d=position haut-bas
		else Player[CurrentPlayer].yi = cosf(Player[CurrentPlayer].yi_cos * 50) / 60;	// l'amplitude est réduite vers la fin

		Player[CurrentPlayer].Mesh->modelo.pos.y = Player[CurrentPlayer].Position.v[YDIM] + Player[CurrentPlayer].yi;

		// Affichage du cercle
		static float x, y, z;
		
		x = Player[CurrentPlayer].Position.v[XDIM];
		y = Player[CurrentPlayer].Position.v[YDIM] + Player[CurrentPlayer].yi;
		z = Player[CurrentPlayer].Position.v[ZDIM];

		float tx = 0;

		// cercle game 2D
		if (CHAR_2D == Game.RenderMode)
		{
			// ring
			glPushMatrix();
				tx = 0.05; //0.1
				glTranslatef(x, y, 0.01);
				TexturesManager.Textures[CTM_PARTICLE3]->SetTexture(); //43(gif) 51(png)
				glColor4f (1.0f, 1.0f, 1.0f, 0.6f);
				{
				glBegin (GL_QUADS);
					glTexCoord2f (0,1);glVertex2f (-tx - 0.5f, +tx + 0.5f);
					glTexCoord2f (1,1);glVertex2f (+tx + 0.5f, +tx + 0.5f);
					glTexCoord2f (1,0);glVertex2f (+tx + 0.5f, -tx - 0.5f);
					glTexCoord2f (0,0);glVertex2f (-tx - 0.5f, -tx - 0.5f);
				glEnd ();
				}
			glPopMatrix();
		}

		// cercle game 3D
		else
		{
/*			// sphere
			glPushMatrix();
				glEnable(GL_CULL_FACE);
				glFrontFace(GL_CCW);
				glTranslatef(MeshesManager.Mesh[MESH2PLAYER].pos.v[XDIM] , MeshesManager.Mesh[MESH2PLAYER].pos.v[YDIM], MeshesManager.Mesh[MESH2PLAYER].pos.v[ZDIM]);
				glTranslatef(0, 0.2, 0);
				glRotatef(90, 0, 1, 0);
				static float angle_sphere = 0;
				angle_sphere += 1;
				glRotatef(angle_sphere, 1, 0, 0);
				glBindTexture (GL_TEXTURE_2D, texture[53]);
				//glColor4f (1.0f, 1.0f, 1.0f, Bonus[CBN_BONUS_LASER].Explosion.Life);
				glColor4f (1.0f, 1.0f, 1.0f, Bonus[CBN_BONUS_3D].Explosion.Life > 0.2 ? 0.2 : Bonus[CBN_BONUS_3D].Explosion.Life);
				//gluSphere(quadratic,0.73f,2,32); //0.78 0.75
				gluSphere(quadratic,0.78f,32,32);//0.71
			glPopMatrix();
*/
			// ring
			static float angle_ring = 0;
			if (BonusManager.Bonus[CBN_BONUS_3D].Explosion.Life > 1.5)
				angle_ring += Game.RenderTemp*1.5; // /1.5
			else if (BonusManager.Bonus[CBN_BONUS_3D].Explosion.Life > 1.0f)
				//angle_ring += Time.ms10 * Bonus[CBN_BONUS_3D].Explosion.Life/1.5;
				angle_ring += Game.RenderTemp*1.0; // /5
			else if (BonusManager.Bonus[CBN_BONUS_3D].Explosion.Life > 0.5)
				angle_ring += Game.RenderTemp /5; // /10
			else
				angle_ring += Game.RenderTemp /8;// /15
			
			if (angle_ring > 360)
				angle_ring = fmod(angle_ring, 360);
/*			for (int i=0; i<=5; i++)
			{
			glPushMatrix();
				glDisable(GL_DEPTH_TEST);
				tx = -0.05; //0.08
				glDisable(GL_CULL_FACE);
				glTranslatef(MeshesManager.Mesh[MESH2PLAYER].pos.v[XDIM] , MeshesManager.Mesh[MESH2PLAYER].pos.v[YDIM], MeshesManager.Mesh[MESH2PLAYER].pos.v[ZDIM]);
				glTranslatef(0, 0.2, 0);
				//glRotatef(angle_ring*3, 1, 1.2, 1.3);
				//glRotatef(90, 0, 0, 1);
				if (i < 10)
					glRotatef(angle_ring+i*30, 1, 0, 0); //60
					//glRotatef(angle_ring, 1, 1, 0);
				else if (i == 1)
					//glRotatef(angle_ring/2, -1.1, -1.1, -1.1);
					glRotatef(angle_ring+45, -1, -1, 0);
				else if (i == 2)
					//glRotatef(angle_ring/3, -1.2, 1, 1);
					glRotatef(angle_ring+90, 1, -1, 0);
				else if (i == 3)
					//glRotatef(angle_ring/4, 1, -1.1, -1.1);
					glRotatef(angle_ring+135, -1, 1, 0);

				glBindTexture (GL_TEXTURE_2D, TexturesManager.Textures[CTM_PARTICLE3_3D].Id);//52 43
				//glColor4f (1.0f, 1.0f, 1.0f, Bonus[CBN_BONUS_3D].Explosion.Life*1);
				//glColor4f (1.0f, 1.0f, 1.0f, Bonus[CBN_BONUS_3D].Explosion.Life > 0.15 ? 0.15 : Bonus[CBN_BONUS_3D].Explosion.Life);
				if (Bonus[CBN_BONUS_3D].Explosion.Life > 2.0f)
					glColor4f (1.0f, 1.0f, 1.0f, 0.3);
				else
					glColor4f (1.0f, 1.0f, 1.0f, Bonus[CBN_BONUS_3D].Explosion.Life *0.15);
				{
				glBegin (GL_QUADS);
					glTexCoord2f (0,1);glVertex2f (-tx - 1.0f, +tx + 1.0f);//1.2
					glTexCoord2f (1,1);glVertex2f (+tx + 1.0f, +tx + 1.0f);//1.2
					glTexCoord2f (1,0);glVertex2f (+tx + 1.0f, -tx - 1.0f);//0.8
					glTexCoord2f (0,0);glVertex2f (-tx - 1.0f, -tx - 1.0f);//0.8
				glEnd ();
				glEnable(GL_DEPTH_TEST);
				}
			glPopMatrix();
			}*/

			// ring
			glPushMatrix();
				glDisable(GL_DEPTH_TEST);
				tx = 0.03; //0.08
				glDisable(GL_CULL_FACE);
				glTranslatef(MeshesManager.Meshes[Player[CurrentPlayer].Mesh->IndexInList]->modelo.pos.x, MeshesManager.Meshes[Player[CurrentPlayer].Mesh->IndexInList]->modelo.pos.y, MeshesManager.Meshes[Player[CurrentPlayer].Mesh->IndexInList]->modelo.pos.z);
				glTranslatef(0, 0.2, 0);

				TexturesManager.Textures[CTM_PARTICLE3_3D]->SetTexture();//52 43
				if (BonusManager.Bonus[CBN_BONUS_3D].Explosion.Life > 2.0f)
					glColor4f (1.0f, 1.0f, 1.0f, 0.3);
				else
					glColor4f (1.0f, 1.0f, 1.0f, BonusManager.Bonus[CBN_BONUS_3D].Explosion.Life *0.15);
				//glColor4f (1.0f, 1.0f, 1.0f, Bonus[CBN_BONUS_LASER].Explosion.Life);
				glColor4f (1.0f, 1.0f, 1.0f, BonusManager.Bonus[CBN_BONUS_3D].Explosion.Life > 0.4 ? 0.4 : BonusManager.Bonus[CBN_BONUS_3D].Explosion.Life);
				{
				glBegin (GL_QUADS);
					glTexCoord2f (0,1);glVertex2f (-tx - 1.0f, +tx + 1.0f);//1.2
					glTexCoord2f (1,1);glVertex2f (+tx + 1.0f, +tx + 1.0f);//1.2
					glTexCoord2f (1,0);glVertex2f (+tx + 1.0f, -tx - 1.0f);//0.8
					glTexCoord2f (0,0);glVertex2f (-tx - 1.0f, -tx - 1.0f);//0.8
				glEnd ();
				glEnable(GL_DEPTH_TEST);
				}
			glPopMatrix();

			// sphere
			// Anti-aliasing (DEBUT)
/*			if (Application.Window.Antialiasing_active)
				glEnable(GL_MULTISAMPLE_ARB);
			glPushMatrix();
				glEnable(GL_CULL_FACE);
				glFrontFace(GL_CCW);
				glTranslatef(MeshesManager.Mesh[MESH2PLAYER].pos.v[XDIM] , MeshesManager.Mesh[MESH2PLAYER].pos.v[YDIM], MeshesManager.Mesh[MESH2PLAYER].pos.v[ZDIM]);
				glTranslatef(0, 0.2, 0);
				glRotatef(90, 0, 1, 0);
				static float angle_sphere = 0;
				angle_sphere += 1;
				glRotatef(angle_sphere, 1, 0, 0);
				//glRotatef(angle_ring, 1, 0, 0);
				glBindTexture (GL_TEXTURE_2D, texture[53]);
				glColor4f (1.0f, 1.0f, 1.0f, Bonus[CBN_BONUS_3D].Explosion.Life);
				//glColor4f (1.0f, 1.0f, 1.0f, Bonus[CBN_BONUS_3D].Explosion.Life > 0.4 ? 0.4 : Bonus[CBN_BONUS_3D].Explosion.Life);
				gluSphere(quadratic,0.78f,32,32); //0.78 0.75
			glPopMatrix();
				glDisable(GL_CULL_FACE);
			// Anti-aliasing (FIN)
			if (Application.Window.Antialiasing_active)
				glDisable(GL_MULTISAMPLE_ARB);*/

		}

		// reset when ending
		//ACHTUNG: NO LO ENTIENDO
		/*if (BonusManager.Bonus[CBN_BONUS_3D].Explosion.Life <= 0.0f)
		{
			BonusManager.Bonus[CBN_BONUS_3D].Active			= false;
			BonusManager.Bonus[CBN_BONUS_3D].Explosion.Alive	= false;
			Player[CurrentPlayer].yi				= 0.00;
			Player[CurrentPlayer].yi_speed		= 0.01;
			Player[CurrentPlayer].yi_cos			= 0;

			Game.Sound2D3DPassed = false;
		}*/

		// Going to game 3D
		if (BonusManager.Bonus[CBN_BONUS_3D].Explosion.Life < 2.0 && CHAR_2D == Game.RenderMode) 
		{
			try
			{
				Game.ChangeRenderMode(CHAR_3D);
				Game.passing_2D_2_3D = false;
				Game.GameEvent(CSIG_3D_CHANGED);	//v Controlado 6->4  //Turning back from CSIG_PASSING_2_3D state to CSIG_PLAYING state
				//Game.Level = 1;
				//MeshesManager.Meshes[Player[CurrentPlayer].Mesh->IndexInList]->modelo.pos.x = Player[CurrentPlayer].Position.v[XDIM];									// spécifie la position du Player 3D car sinon l'anneau serait affiché au centre lors du passage dans la prochaine boucle.
				//Player[CurrentPlayer].Speed.Reset();
			}
			catch(...)
			{
				ERR(CGUI_ParserMsgs[CIP_AVERAGE_ERROR]);
			}
		}

		// tiembla la escena
/*		if (Bonus[CBN_BONUS_3D].Explosion.Life <= 3.5 && Bonus[CBN_BONUS_3D].Explosion.Life >=0.5)
		{
			//glRotatef(-Bonus[CBN_BONUS_LASER].Shake2, -0.3f, 0.5f, 1.0f);

			if (Bonus[CBN_BONUS_3D].Explosion.Life >= 1.5f)
			{
				Bonus[CBN_BONUS_3D].Shake2 += Bonus[CBN_BONUS_3D].Shake1;
				Bonus[CBN_BONUS_3D].Shake1 *= 1.05;
			}
			else
			{
				Bonus[CBN_BONUS_3D].Shake2 -= Bonus[CBN_BONUS_3D].Shake1;
				Bonus[CBN_BONUS_3D].Shake1 *= 0.99;
			}

			if (Bonus[CBN_BONUS_3D].Shake1 >= 1.2)
				Bonus[CBN_BONUS_3D].Shake1 = 1.2;

			else if (Bonus[CBN_BONUS_3D].Shake1 <= 0.5)
				Bonus[CBN_BONUS_3D].Shake1 = 0.5;

			Bonus[CBN_BONUS_3D].Shake2 *= rand()%4*0.1 - 0.2;

			//glRotatef(Bonus[CBN_BONUS_3D].Shake2, -0.3f, 0.5f, 1.0f);
		}
*/
// que disminuye la vida de la "explosión" de Bono
#ifdef DEBUG_PASSAGE_3D
		if (Bonus[CBN_BONUS_LASER].Explosion.Life - 0.01*Game.RenderTemp <= 0)			// no se exceda el límite de 0
			Bonus[CBN_BONUS_LASER].Explosion.Life = 0.0f;						// tenemos 0
		else
			Bonus[CBN_BONUS_LASER].Explosion.Life -= 0.01*Game.RenderTemp;
#else
		if (BonusManager.Bonus[CBN_BONUS_3D].Explosion.Life - 0.001*Game.RenderTemp <= 0)			// no se exceda el límite de 0
			BonusManager.Bonus[CBN_BONUS_3D].Explosion.Life = 0.0f;						// tenemos 0
		else
			BonusManager.Bonus[CBN_BONUS_3D].Explosion.Life -= 0.001*Game.RenderTemp;
#endif
		// su CROSS (2D -> 3D)
		if (!Game.Sound2D3DPassed && BonusManager.Bonus[CBN_BONUS_3D].Explosion.Life <= 2.4 && BonusManager.Bonus[CBN_BONUS_3D].Explosion.Life > 0.0)//...3.0...
		{
			Game.Sound2D3DPassed = true;

			SoundsManager.Play(CGS_CROSS,UGKSND_MAX_VOLUME);
			//int sound = FSOUND_PlaySoundEx(FSOUND_FREE,SoundsManager.Sound[CGS_CROSS].Sound, SoundsManager.dspUnitSounds, false);
		}
	}
}

/**   
	*@fn display_score_hiscore_lives
	* Display score (punctuation) and lives on the screen
    * This function lets you see the punctuation and the lives of a character or ship
    * These are calculated and then displayed in the coordinates defined
*/
void CGUI::display_score_hiscore_lives()
{
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	//glEnable(GL_BLEND);
	// SCORE
	glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
	TexturesManager.Textures[CTM_SCORE]->SetTexture();
	glBegin (GL_QUADS);
		glTexCoord2f (0,1);	glVertex2f ((-5.0f - 1.0f), (5.7f +.3f));
		glTexCoord2f (0,0);	glVertex2f ((-5.0f - 1.0f), (5.7f -.3f));
		glTexCoord2f (1,0);	glVertex2f ((-5.0f + 1.0f), (5.7f -.3f));
		glTexCoord2f (1,1);	glVertex2f ((-5.0f + 1.0f), (5.7f +.3f));
	glEnd ();

	char str_chif[7] = UGKS_EMPTY_STRING;

	_itoa(Game.Score, str_chif, 10);        // integer to string, en base 10

	int long_chif = strlen(str_chif);
	int long_0 = 6 - long_chif, i;

	for (i=0; i<= long_0 - 1; i++)
	{
		glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
		TexturesManager.Textures[CTM_NUMBER0]->SetTexture();
		glBegin (GL_QUADS);
			glTexCoord2f (0,1);	glVertex2f (-6.3f +i/1.9f - .3f, 5.0f +.3f);
			glTexCoord2f (0,0);	glVertex2f (-6.3f +i/1.9f - .3f, 5.0f -.3f);
			glTexCoord2f (1,0);	glVertex2f (-6.3f +i/1.9f + .3f, 5.0f -.3f);
			glTexCoord2f (1,1);	glVertex2f (-6.3f +i/1.9f + .3f, 5.0f +.3f);
		glEnd ();
	}
	for (i=long_0 ; i<= 5 ; i++)
	{
		int chif_texture = (int)(str_chif[i - long_0]) - 48; // como "0" = 48 en la tabla ASCII y el elenco (char *) no funciona para atoi ()

		glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
		TexturesManager.Textures[chif_texture+CTM_NUMBER0]->SetTexture();
		glBegin (GL_QUADS);
			glTexCoord2f (0,1);	glVertex2f (-6.3f +i/1.9f - .3f, 5.0f +.3f);
			glTexCoord2f (0,0);	glVertex2f (-6.3f +i/1.9f - .3f, 5.0f -.3f);
			glTexCoord2f (1,0);	glVertex2f (-6.3f +i/1.9f + .3f, 5.0f -.3f);
			glTexCoord2f (1,1);	glVertex2f (-6.3f +i/1.9f + .3f, 5.0f +.3f);
		glEnd ();
	}

	// HISCORE
	glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
	TexturesManager.Textures[CTM_HISCORE]->SetTexture();
	glBegin (GL_QUADS);
		glTexCoord2f (0,1);	glVertex2f (-0.1f - 2.0f, 5.7f +.3f);
		glTexCoord2f (0,0);	glVertex2f (-0.1f - 2.0f, 5.7f -.3f);
		glTexCoord2f (1,0);	glVertex2f (-0.1f + 2.0f, 5.7f -.3f);
		glTexCoord2f (1,1);	glVertex2f (-0.1f + 2.0f, 5.7f +.3f);
	glEnd ();

	//if (Game.HiScore > Game.Score)
		_itoa(Game.HiScore, str_chif, 10);			// integer to string, en base 10
	//else
	//	_itoa(Game.Score, str_chif, 10);				// integer to string, en base 10

	long_chif = strlen(str_chif);
	long_0 = 6 - long_chif;


	for (i=0; i<= long_0 - 1; i++)
	{
		glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
		TexturesManager.Textures[CTM_NUMBER0]->SetTexture();
		glBegin (GL_QUADS);
			glTexCoord2f (0,1);	glVertex2f (-1.5f +i/1.9f - .3f, 5.0f +.3f);
			glTexCoord2f (0,0);	glVertex2f (-1.5f +i/1.9f - .3f, 5.0f -.3f);
			glTexCoord2f (1,0);	glVertex2f (-1.5f +i/1.9f + .3f, 5.0f -.3f);
			glTexCoord2f (1,1);	glVertex2f (-1.5f +i/1.9f + .3f, 5.0f +.3f);
		glEnd ();
	}
	for (i=long_0 ; i<= 5 ; i++)
	{
		int chif_texture = (int)(str_chif[i - long_0]) - 48; // car "0" = 48 dans la table ASCII et le casting (char *) veut pas marcher pour atoi()

		glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
		TexturesManager.Textures[CTM_NUMBER0]->SetTexture();
		glBegin (GL_QUADS);
			glTexCoord2f (0,1);	glVertex2f (-1.5f +i/1.9f - .3f, 5.0f +.3f);
			glTexCoord2f (0,0);	glVertex2f (-1.5f +i/1.9f - .3f, 5.0f -.3f);
			glTexCoord2f (1,0);	glVertex2f (-1.5f +i/1.9f + .3f, 5.0f -.3f);
			glTexCoord2f (1,1);	glVertex2f (-1.5f +i/1.9f + .3f, 5.0f +.3f);
		glEnd ();
	}

	// LIVES
	glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
	TexturesManager.Textures[CTM_LIVES]->SetTexture();
	glBegin (GL_QUADS);
		glTexCoord2f (0,1);	glVertex2f (4.55f - 1.0f, 5.7f +.3f);
		glTexCoord2f (0,0);	glVertex2f (4.55f - 1.0f, 5.7f -.3f);
		glTexCoord2f (1,0);	glVertex2f (4.55f + 1.0f, 5.7f -.3f);
		glTexCoord2f (1,1);	glVertex2f (4.55f + 1.0f, 5.7f +.3f);
	glEnd ();

	_itoa(Player[CurrentPlayer].Lives, str_chif, 10);        // integer to string, en base 10

	long_chif = strlen(str_chif);
	long_0 = 3 - long_chif;

	for (i=0; i<= long_0 - 1; i++)
	{
		glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
		TexturesManager.Textures[CTM_NUMBER0]->SetTexture();
		glBegin (GL_QUADS);
			glTexCoord2f (0,1);	glVertex2f (4.0f +i/1.9f - .3f, 5.0f +.3f);
			glTexCoord2f (0,0);	glVertex2f (4.0f +i/1.9f - .3f, 5.0f -.3f);
			glTexCoord2f (1,0);	glVertex2f (4.0f +i/1.9f + .3f, 5.0f -.3f);
			glTexCoord2f (1,1);	glVertex2f (4.0f +i/1.9f + .3f, 5.0f +.3f);
		glEnd ();
	}
	for (i=long_0 ; i<= 2 ; i++)
	{
		int chif_texture = (int)(str_chif[i - long_0]) - 48; // car "0" = 48 dans la table ASCII et le casting (char *) veut pas marcher pour atoi()

		glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
		TexturesManager.Textures[chif_texture+CTM_NUMBER0]->SetTexture();
		glBegin (GL_QUADS);
			glTexCoord2f (0,1);	glVertex2f (4.0f +i/1.9f - .3f, 5.0f +.3f);
			glTexCoord2f (0,0);	glVertex2f (4.0f +i/1.9f - .3f, 5.0f -.3f);
			glTexCoord2f (1,0);	glVertex2f (4.0f +i/1.9f + .3f, 5.0f -.3f);
			glTexCoord2f (1,1);	glVertex2f (4.0f +i/1.9f + .3f, 5.0f +.3f);
		glEnd ();
	}
	//glDisable(GL_BLEND);
}


/**   
	* @fn display_below_lives
	* Show the decline of life
    * This function allows to decrease the life of a character Nave
    * This function changes the window the texture of life
*/
void CGUI::display_below_lives()
{
	//< Display number of lives for all players
	if(Player[CurrentPlayer].Lives>0 && Player[CurrentPlayer].Lives<4){
		glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
		// la figura 0, 1, 2 ou 3 
		TexturesManager.Textures[CTM_NUMBER0 + Player[CurrentPlayer].Lives]->SetTexture();
		glBegin (GL_QUADS);
			glTexCoord2f (0,1);	glVertex2f (CSIG_PG_LEFT  - .4f, -6.9f +.25f);
			glTexCoord2f (0,0);	glVertex2f (CSIG_PG_LEFT  - .4f, -6.9f -.25f);
			glTexCoord2f (1,0);	glVertex2f (CSIG_PG_LEFT  + .4f, -6.9f -.25f);
			glTexCoord2f (1,1);	glVertex2f (CSIG_PG_LEFT  + .4f, -6.9f +.25f);
		glEnd ();
	}

	if(Player2[CurrentPlayer].Lives>0 && Player2[CurrentPlayer].Lives<4){
		glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
		//< Marca en cantidad, las vidas restantes del player 2
		TexturesManager.Textures[CTM_NUMBER0 + Player2[CurrentPlayer].Lives]->SetTexture();
		glBegin (GL_QUADS);
			glTexCoord2f (0,1);	glVertex2f (CSIG_PG_LEFT  - .4f, -7.5f +.25f);
			glTexCoord2f (0,0);	glVertex2f (CSIG_PG_LEFT  - .4f, -7.5f -.25f);
			glTexCoord2f (1,0);	glVertex2f (CSIG_PG_LEFT  + .4f, -7.5f -.25f);
			glTexCoord2f (1,1);	glVertex2f (CSIG_PG_LEFT  + .4f, -7.5f +.25f);
		glEnd ();
	}

	//<This check must be removed when Player[CurrentPlayer].Lives doesn't reach 65555.
	if(Player[CurrentPlayer].Lives>0 && Player[CurrentPlayer].Lives<4){

		// ALPHA TEST + BLEND
		glAlphaFunc(GL_GREATER, 0.3f);								// for TGA alpha test
		glEnable(GL_ALPHA_TEST);									// for TGA alpha test
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);			// for anti-aliasing

		// los buques en el verde abajo a la derecha
		for (int k=1; k <= Player[CurrentPlayer].Lives; k++)
		{
			glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
			if (CHAR_2D == Game.RenderMode)
				TexturesManager.Textures[Player[CurrentPlayer].IndTexture2D]->SetTexture();
			else
				TexturesManager.Textures[CTM_PLAYER3D]->SetTexture();

			glBegin (GL_QUADS);
				glTexCoord2f (0,1);	glVertex2f (CSIG_PG_LEFT + k/1.3f - .3f, -6.9f +.25f);
				glTexCoord2f (0,0);	glVertex2f (CSIG_PG_LEFT + k/1.3f - .3f, -6.9f -.25f);
				glTexCoord2f (1,0);	glVertex2f (CSIG_PG_LEFT + k/1.3f + .3f, -6.9f -.25f);
				glTexCoord2f (1,1);	glVertex2f (CSIG_PG_LEFT + k/1.3f + .3f, -6.9f +.25f);
			glEnd ();		
		}
		// BACK TO NON ALPHA TEST + PREVIOUS BLEND
		glDisable(GL_ALPHA_TEST);
		//glDisable(GL_BLEND);
	}

	//<This check must be removed when Player[CurrentPlayer].Lives doesn't reach 65555.
	if(Player2[CurrentPlayer].Lives>0 && Player2[CurrentPlayer].Lives<4){
		// ALPHA TEST + BLEND
		glAlphaFunc(GL_GREATER, 0.3f);								// for TGA alpha test
		glEnable(GL_ALPHA_TEST);									// for TGA alpha test
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);			// for anti-aliasing

		//< Marca en numero de iconos, la cantidad de vidas restantes del player 2
		for (int k=1; k <= Player2[CurrentPlayer].Lives; k++)
		{
			glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
			if (CHAR_2D == Game.RenderMode)
				TexturesManager.Textures[Player2[CurrentPlayer].IndTexture2D]->SetTexture();
			else
				TexturesManager.Textures[CTM_PLAYER3D]->SetTexture();

			glBegin (GL_QUADS);
				glTexCoord2f (0,1);	glVertex2f (CSIG_PG_LEFT + k/1.3f - .3f, -7.5f +.25f);
				glTexCoord2f (0,0);	glVertex2f (CSIG_PG_LEFT + k/1.3f - .3f, -7.5f -.25f);
				glTexCoord2f (1,0);	glVertex2f (CSIG_PG_LEFT + k/1.3f + .3f, -7.5f -.25f);
				glTexCoord2f (1,1);	glVertex2f (CSIG_PG_LEFT + k/1.3f + .3f, -7.5f +.25f);
			glEnd ();		
		}
		// BACK TO NON ALPHA TEST + PREVIOUS BLEND
		glDisable(GL_ALPHA_TEST);
	}
}

/**   
	* @fn display_background
	* Display the bottom of the window
    * This function allows you to display the bottom of the window depending on whether 2D or 3D
    * Also depends on the level that is there are several funds
    * @param bool viewport2 represents the state in 3D
*/
void CGUI::display_background(bool viewport2)
{
	// GAME 2D
	if (CHAR_2D == Game.RenderMode)
	{
		glColor4f (1.0f, 1.0f, 1.0f, 1.0f);

		if (Game.Level  == 1 || Game.Level  == 0) // fond étoilé
		{
			TexturesManager.Textures[CTM_SPACE_BCKG]->SetTexture();

			glBegin (GL_QUADS);
				glTexCoord2f (0,0);	glVertex3f (-12,  12, -.05f); // -.05 pour éviter le z-fighting
				glTexCoord2f (1,0);	glVertex3f (-12, -12, -.05f);
				glTexCoord2f (1,1);	glVertex3f ( 12, -12, -.05f);
				glTexCoord2f (0,1);	glVertex3f ( 12,  12, -.05f);
			glEnd ();
		}
		else if (Game.Level == 2) // planète
		{
			glPushMatrix();

			glTranslatef(0.0f, 0.0f, -5.0f);
			TexturesManager.Textures[CTM_PLANET_BCKG]->SetTexture();
			glBegin ( GL_QUADS );
				glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.00f, -5.0f,  -.05f);
				glTexCoord2f(1.0f, 0.0f); glVertex3f( 10.00f, -5.0f,  -.05f);
				glTexCoord2f(1.0f, 1.0f); glVertex3f( 10.00f,  5.0f,  -.05f);
				glTexCoord2f(0.0f, 1.0f); glVertex3f(-10.00f,  5.0f,  -.05f);
			glEnd();

			glPopMatrix();
		}
		else if (Game.Level == 3) // earth-moon
		{
			glPushMatrix();

			glTranslatef(0.0f, 0.0f, -15.0f);
			TexturesManager.Textures[CTM_EARTH_MOON_BCKG]->SetTexture();
			glBegin ( GL_QUADS );
				glTexCoord2f(0.0f, 0.0f); glVertex3f(-13.46f, -08.0f,  -.05f);
				glTexCoord2f(1.0f, 0.0f); glVertex3f( 13.46f, -08.0f,  -.05f);
				glTexCoord2f(1.0f, 1.0f); glVertex3f( 13.46f,  12.0f,  -.05f);
				glTexCoord2f(0.0f, 1.0f); glVertex3f(-13.46f,  12.0f,  -.05f);
			glEnd();

			glPopMatrix();
		}
		else if (Game.Level == 4) // galaxy1
		{
			glPushMatrix();

			TexturesManager.Textures[CTM_GALAXY_BCKG]->SetTexture();
			glBegin ( GL_QUADS );
				glTexCoord2f(0.0f, 0.0f); glVertex3f(-08.00f, -07.36f,  -.05f);
				glTexCoord2f(1.0f, 0.0f); glVertex3f( 08.00f, -07.36f,  -.05f);
				glTexCoord2f(1.0f, 1.0f); glVertex3f( 08.00f,  07.36f,  -.05f);
				glTexCoord2f(0.0f, 1.0f); glVertex3f(-08.00f,  07.36f,  -.05f);
			glEnd();

			glPopMatrix();
		}
		else if (Game.Level == 5) // skybox
		{
			TexturesManager.Textures[CTM_SKYBOX_BCKG]->SetTexture();// 5 = vaisseau rouge
			glBegin ( GL_QUADS );
				// Back Face
				glTexCoord2f(1.0f, 0.0f); glVertex3f(-15.0f, -15.0f, -51.0f);
				glTexCoord2f(1.0f, 1.0f); glVertex3f(-15.0f,  15.0f, -51.0f);
				glTexCoord2f(0.0f, 1.0f); glVertex3f( 15.0f,  15.0f, -51.0f);
				glTexCoord2f(0.0f, 0.0f); glVertex3f( 15.0f, -15.0f, -51.0f);
			glEnd();
			TexturesManager.Textures[CTM_SKYBOX_BCKG_JPG]->SetTexture();
			glBegin ( GL_QUADS );
				// Top Face
				glTexCoord2f(0.0f, 1.0f); glVertex3f(-15.0f,  15.0f, -51.0f);
				glTexCoord2f(0.0f, 0.0f); glVertex3f(-15.0f,  15.0f,  -21.0f);
				glTexCoord2f(1.0f, 0.0f); glVertex3f( 15.0f,  15.0f,  -21.0f);
				glTexCoord2f(1.0f, 1.0f); glVertex3f( 15.0f,  15.0f, -51.0f);
			glEnd();
			TexturesManager.Textures[CTM_SKYBOX_BCKG_JPG]->SetTexture();
			glBegin ( GL_QUADS );
				// Bottom Face
				glTexCoord2f(1.0f, 1.0f); glVertex3f(-15.0f, -15.0f, -51.0f);
				glTexCoord2f(0.0f, 1.0f); glVertex3f( 15.0f, -15.0f, -51.0f);
				glTexCoord2f(0.0f, 0.0f); glVertex3f( 15.0f, -15.0f,  -21.0f);
				glTexCoord2f(1.0f, 0.0f); glVertex3f(-15.0f, -15.0f,  -21.0f);
			glEnd();
			TexturesManager.Textures[CTM_SKYBOX_BCKG_JPG]->SetTexture();
			glBegin ( GL_QUADS );
				// Right face
				glTexCoord2f(1.0f, 0.0f); glVertex3f( 15.0f, -15.0f, -51.0f);
				glTexCoord2f(1.0f, 1.0f); glVertex3f( 15.0f,  15.0f, -51.0f);
				glTexCoord2f(0.0f, 1.0f); glVertex3f( 15.0f,  15.0f,  -21.0f);
				glTexCoord2f(0.0f, 0.0f); glVertex3f( 15.0f, -15.0f,  -21.0f);
			glEnd();
			TexturesManager.Textures[CTM_SKYBOX_BCKG_JPG]->SetTexture();
			glBegin ( GL_QUADS );
				// Left Face
				glTexCoord2f(0.0f, 0.0f); glVertex3f(-15.0f, -15.0f, -51.0f);
				glTexCoord2f(1.0f, 0.0f); glVertex3f(-15.0f, -15.0f,  -21.0f);
				glTexCoord2f(1.0f, 1.0f); glVertex3f(-15.0f,  15.0f,  -21.0f);
				glTexCoord2f(0.0f, 1.0f); glVertex3f(-15.0f,  15.0f, -51.0f);
			glEnd();
		}

		// texture barre du bas
		TexturesManager.Textures[CTM_BAND]->SetTexture();
	}
	
	// GAME 3D (background)
	else
	{
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW);
		
		if (!viewport2)
			glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
		else
			glColor4f (1.0f, 1.0f, 1.0f, 0.7f);

		Background.Mesh->modelo.rot.x = 45;
		Background.Mesh->modelo.rot.y = 90;

		if (!viewport2)
			Background.Mesh->modelo.rot.z -= 0.07 * Game.UpdateTemp;	// rotation effectuée qu'une seule fois, en mode normal (pas dans la console)

		Background.Mesh->modelo.Draw();

		glDisable(GL_CULL_FACE);

		// texture barre du bas
		TexturesManager.Textures[CTM_3D_BAND]->SetTexture();
	}

	// Anti-aliasing (DEBUT)
	if (Application.Window.Antialiasing_active)
		glEnable(GL_MULTISAMPLE_ARB);
	
	// la ligne du bas
	glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
	glBegin (GL_QUADS);
		glTexCoord2f (0,1);	glVertex3f (-8, -6.5, -.05f);
		glTexCoord2f (0,0);	glVertex3f (-8, -6.6, -.05f);
		glTexCoord2f (1,0);	glVertex3f ( 8, -6.6, -.05f);
		glTexCoord2f (1,1);	glVertex3f ( 8, -6.5, -.05f);
	glEnd ();

	// Anti-aliasing (FIN)
	if (Application.Window.Antialiasing_active)
		glDisable(GL_MULTISAMPLE_ARB);
}

/**   
	* @fn display_cube
	* Display the Cube
    * @param texture_id int represents the number of texture
    * @param float size representing the size of the cube
    * This function lets you draw a cube texture in the window  
*/
void CGUI::display_cube(int texture_id, float size)
{
	glPushMatrix();

	//glTranslatef ( 0.0, 0.0, -700.0 );
	//glRotatef ( xrot, 1.0, 0.0, 0.0 );
	//glRotatef ( yrot, 0.0, 1.0, 0.0 );
	//glRotatef ( zrot, 0.0, 0.0, 1.0 );

	//glEnable (GL_TEXTURE_2D);
	glColor4f (1.0f, 1.0f, 1.0f, 1.0f);

	// on applique la texture[4]...
	TexturesManager.Textures[texture_id]->SetTexture();// 9

	// A continuacion a dibujar...
	glBegin ( GL_QUADS );
		// Front Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, -size,  size);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( size, -size,  size);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( size,  size,  size);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-size,  size,  size);
	glEnd();
	TexturesManager.Textures[texture_id]->SetTexture();// 5 = red ship
	glBegin ( GL_QUADS );
		// Back Face
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, -size, -size);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-size,  size, -size);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( size,  size, -size);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( size, -size, -size);
	glEnd();
	TexturesManager.Textures[texture_id]->SetTexture();
	glBegin ( GL_QUADS );
		// Top Face
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-size,  size, -size);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-size,  size,  size);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( size,  size,  size);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( size,  size, -size);
	glEnd();
	TexturesManager.Textures[texture_id]->SetTexture();
	glBegin ( GL_QUADS );
		// Bottom Face
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-size, -size, -size);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( size, -size, -size);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( size, -size,  size);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, -size,  size);
	glEnd();
	TexturesManager.Textures[texture_id]->SetTexture();
	glBegin ( GL_QUADS );
		// Right face
		glTexCoord2f(1.0f, 0.0f); glVertex3f( size, -size, -size);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( size,  size, -size);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( size,  size,  size);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( size, -size,  size);
	glEnd();
	TexturesManager.Textures[texture_id]->SetTexture();
	glBegin ( GL_QUADS );
		// Left Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, -size, -size);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, -size,  size);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-size,  size,  size);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-size,  size, -size);
	glEnd();
	glPopMatrix();

	//xrot+=0.2f;
	//yrot+=0.1f;
	//zrot+=0.3f;
}

/**   
	* @fn display_splashscreen
	* Show splash screen
    * If this option is activated, disables 2D texture
    * And is assigned to the splash screen
*/
void CGUI::display_splashscreen()
{
	if (!Game.active_splash)
		return;

	static float splash_alpha = 1;

	//glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA); // doesn't work with ARB_MULTISAMLE
	glDisable(GL_TEXTURE_2D);
	glColor4f(1.0, 1.0, 1.0, splash_alpha);

	// Dessine le  splash screen
	glBegin(GL_QUADS);
		glVertex3f (-20, -20, 1);// -12 a 12
		glVertex3f ( 20, -20, 1);
		glVertex3f ( 20,  20, 1);
		glVertex3f (-20,  20, 1);
	glEnd();

	glEnable(GL_TEXTURE_2D);
	glColor4f (1, 1, 1, 1);

	//splash_alpha -= 0.0025 * Time.ms10;
	splash_alpha -= 0.0025 *Game.RenderTemp;
	if (splash_alpha <= 0)
	{
		splash_alpha = 1.0;
		Game.active_splash = false;
	}
}

/**   
	* @fn display_console
	* Display the console
    * This is the window where it will launch and display the game
    * Fits according to the resolution of the screen
    * In addition to establishing the various parts of the console as panels and separations of the same
*/
void CGUI::display_console()
{
	// condiciones necesarias
	if (Application.Window.Width != CGC_XGA_HOR_RES || Application.Window.Height != CGC_XGA_VER_RES)
		return;
	if (!Console.Start && !Console.Moving && !Console.Active)	
		return;
	
	glDisable(GL_TEXTURE_2D);

	// colores (con transparencia)
	if (CHAR_2D == Game.RenderMode)
		glColor4f (1.0f, 1.0f, 1.0f, 0.05f);
	else
		glColor4f (1.0f, 1.0f, 1.0f, 0.05f);
	
	// las diapositivas del panel de izquierda a derecha (de derecha a izquierda)
	static float panel_push;

	if (Console.Moving)
	{
		if (Console.ComeIn)
		{
			if (Console.Start)
				panel_push = 5.3;
			if (panel_push - (0.05 *Game.RenderTemp) > 0)							// no deberá exceder la posición de destino
				panel_push -= 0.05 *Game.RenderTemp;
			else
				panel_push = 0;											// llega a 0
		}
		else if (!Console.ComeIn)
		{
			if (Console.Start)
				panel_push = 0;
			if (panel_push < 5.3)
				panel_push += 0.05 *Game.RenderTemp;
		}
	}
	// panel de visualización
	glPushMatrix();
		glLoadIdentity ();
		Camera.LookFrom (panel_push, 0.0, 20.0);
		Camera.LookAt	(panel_push, 0.0,  0.0);

		// parte inferior del panel
		if (CHAR_2D != Game.RenderMode)
			glColor4f (1.0f, 1.0f, 1.0f, 0.03f);//0.15 0.05
		else
			glColor4f (0.7f, 0.7f, 1.0f, 0.03f);

		glBegin (GL_QUADS);
			glTexCoord2f (0,1);	glVertex2f (-11.50,  11.50);
			glTexCoord2f (0,0);	glVertex2f (-11.50, -11.50);
			glTexCoord2f (1,0);	glVertex2f (- 5.80, -11.50);
			glTexCoord2f (1,1);	glVertex2f (- 5.80,  11.50);
		glEnd ();

		// consola de fondo
		glDisable(GL_BLEND);
		switch(Game.RenderMode)
		{case CHAR_2D:
			glColor4f (0.04f, 0.04f, 0.03f, 1.0f);//0.05
			break;
		case CHAR_3D:
			glColor4f (0.00f, 0.384f, 0.651f, 0.05f);//0.05
			break;
		case CHAR_LINES3D:
			glColor4f (0.01f, 0.01f, 0.01f, 1.0f);
		}

		glBegin (GL_QUADS);
			glTexCoord2f (0,1);	glVertex2f (-10.50, 4.65);
			glTexCoord2f (0,0);	glVertex2f (-10.50, 2.50);
			glTexCoord2f (1,0);	glVertex2f (- 6.30, 2.50);
			glTexCoord2f (1,1);	glVertex2f (- 6.30, 4.65);
		glEnd ();
		
		// bordes
		glEnable(GL_BLEND);
		glLineWidth( 2.0f);
		glEnable(GL_LINE_STIPPLE);
		glLineStipple (3, 0xADAD);

		// bordes de la consola
		switch(Game.RenderMode)
		{case CHAR_2D:
			glColor4f (0.5f, 0.5f, 0.5f, 0.2f);//1.0
			break;
		case CHAR_3D:
			glColor4f (0.9f, 0.9f, 0.9f, 0.1f);//1.0
			break;
		case CHAR_LINES3D:
			glColor4f (0.7f, 0.7f, 0.8f, 0.15f);
		}
		glBegin(GL_LINE_LOOP);
			glVertex2f (-10.50, 4.65);
			glVertex2f (-10.50, 2.50);
			glVertex2f (- 6.30, 2.50);
			glVertex2f (- 6.30, 4.65);
		glEnd();
		// mini-screen bordes (viewport2)
		glBegin(GL_LINE_LOOP);
			glVertex2f (-10.50, -5.58);
			glVertex2f (-10.50, -2.45);
			glVertex2f (- 6.30, -2.45);
			glVertex2f (- 6.30, -5.58);
		glEnd();

		// borde del panel
		switch(Game.RenderMode)
		{case CHAR_2D:
			glColor4f (0.3f, 0.3f, 0.3f, 0.2f);//0.2
			break;
		case CHAR_3D:
			glColor4f (0.9f, 0.9f, 0.9f, 0.05f);//0.05
		}

		glBegin(GL_LINES);
			glVertex2f (-5.80,  11.50);
			glVertex2f (-5.80, -11.50);
		glEnd();

	glPopMatrix();

	float FontVerticalSpace_temp = FontsManager.GetFontVerticalSpace() *1.1;									// mayor espacio

	// INPUT UTILISATEUR (LA LIGNE DE COMMANDE)
	// input ligne 1
	char temp[21];
	char line1[21];
	strncpy(temp, Console.Text[Console.Line], CGUI_MAX_CONSOLE_MSG_COLS);
	temp[CGUI_MAX_CONSOLE_MSG_COLS] = '\0';
	strcpy(line1, ">");
	strcat(line1, temp);

	// cursor parpadeante
	double integer;
	double blink = modf((double)timeGetTime()/1000, &integer);
	if (blink > 0.3)
		if (strlen(Console.Text[Console.Line]) < CGUI_MAX_CONSOLE_MSG_COLS)
			strcat(line1, "_");
	
	FontsManager.SetFontSize(FontsManager.GetFontSize()-1);
	FontsManager.DrawFont(30-Console.ViewportPush, 580+Console.Line*FontVerticalSpace_temp, line1);
	//fontDrawString(30-ViewportPush, 580+Console.Line*FontVerticalSpace_temp, line1);

	// input ligne 2
	if (strlen(Console.Text[Console.Line]) >= CGUI_MAX_CONSOLE_MSG_COLS)
	{
		char* temp;
		char line2[21];
		temp = &Console.Text[Console.Line][CGUI_MAX_CONSOLE_MSG_COLS];
		strcpy(line2, temp);
		if (blink > 0.3)
			strcat(line2, "_");
		FontsManager.SetFontSize(FontsManager.GetFontSize()-1);
		FontsManager.DrawFont(30-Console.ViewportPush, 580-1*FontVerticalSpace_temp, line2);
		//fontDrawString(30-ViewportPush, 580-1*FontVerticalSpace_temp, line2);
	}

	// ÉVALUATION DE LA COMMANDE 1
	if (strlen(Console.Command[0]) > 0)
	{
		// Console.Command (1)
		char temp[45] = UGKS_EMPTY_STRING;
		strcat(temp, "\r");
		strcat(temp, (const char*) Console.Command[0]);
		temp[CGUI_MAX_CONSOLE_MSG_COLS]='\0';
		if (CHAR_2D == Game.RenderMode)
			FontsManager.SetFontColor(0.7);
		else
			FontsManager.SetFontColor(0.9);

		FontsManager.SetFontSize(FontsManager.GetFontSize()-1);
		//fontDrawString(30-ViewportPush, 580-2*FontVerticalSpace_temp, temp);
		FontsManager.DrawFont(30-Console.ViewportPush, 580-2*FontVerticalSpace_temp, temp);

		// Console.Output (1)
		if (CHAR_2D == Game.RenderMode)
			FontsManager.SetFontColor(0.7,0.5,0.5);
		else
			FontsManager.SetFontColor(0.952f, 0.937f, 0.804f);

		if (strcmp(Console.Command[0], "HELP") == 0)					// help
			strcpy(Console.Output[0], CGUI_ParserMsgs[CGUI_TYPE_VBLE_NAME]);
		else if (strcmp(Console.Command[0], "CAMERAX") == 0)			// variables
			_ltoa(Camera.Position.v[XDIM], Console.Output[0], 10);
		else if (strcmp(Console.Command[0], "CAMERAY") == 0)
			_ltoa(Camera.Position.v[YDIM], Console.Output[0], 10);
		else if (strcmp(Console.Command[0], "CAMERAZ") == 0)
			_ltoa(Camera.Position.v[ZDIM], Console.Output[0], 10);
		else if (strcmp(Console.Command[0], "ANGLEX") == 0)
			_ltoa(Scene.Angle.v[XDIM], Console.Output[0], 10);
		else if (strcmp(Console.Command[0], "ANGLEY") == 0)
			_ltoa(Scene.Angle.v[YDIM], Console.Output[0], 10);
		else if (strcmp(Console.Command[0], "MUSICTYPE") == 0)
			_ltoa(SoundsManager.music_type, Console.Output[0], 10);
		else if (strcmp(Console.Command[0], "MUSICENCOURS") == 0)
			strncpy(Console.Output[0], Game.CurrentMusic, 39);
		else if (strcmp(Console.Command[0], "MUSICAFFICHEE") == 0)
			strncpy(Console.Output[0], Game.music_affichee, 39);
		else if (strcmp(Console.Command[0], "STREAMNAME") == 0)
			strncpy(Console.Output[0], Game.stream_name, 39);
		else if (strcmp(Console.Command[0], "IDMYMUSIC") == 0)
			_itoa(Game.MusicConsoleOutput, Console.Output[0], 10);
		else														// invalid command
			strcpy(Console.Output[0], CGUI_ParserMsgs[CGUI_INVALID_COMMAND]);
		
		FontsManager.SetFontSize(FontsManager.GetFontSize()-1);
		//fontDrawString(30-ViewportPush, 580-3*FontVerticalSpace_temp, Console.Output);
		FontsManager.DrawFont(30-Console.ViewportPush, 580-3*FontVerticalSpace_temp, Console.Output[0]);
	}

	// ÉVALUATION DE LA COMMANDE 2
	if (strlen(Console.Command[1]) > 0)
	{
		// Console.Command[1]
		char temp[45] = UGKS_EMPTY_STRING;
		strcat(temp, "\r");
		strcat(temp, Console.Command[1]);
		temp[CGUI_MAX_CONSOLE_MSG_COLS]='\0';
		if (CHAR_2D == Game.RenderMode)
			FontsManager.SetFontColor(0.7);
		else
			FontsManager.SetFontColor(0.9);
		FontsManager.SetFontSize(FontsManager.GetFontSize()-1); 
		//fontDrawString(30-ViewportPush, 580-4*FontVerticalSpace_temp, temp);
		FontsManager.DrawFont(30-Console.ViewportPush, 580-4*FontVerticalSpace_temp, temp);

		// Console.Output[1]
		if (CHAR_2D == Game.RenderMode)
			FontsManager.SetFontColor(0.7,0.5,0.5);
		else
			FontsManager.SetFontColor (0.952f, 0.937f, 0.804f);

		if (strcmp(Console.Command[1], "CAMERAX") == 0)				// variables
			_ltoa(Camera.Position.v[XDIM], Console.Output[1], 10);
		else if (strcmp(Console.Command[1], "CAMERAY") == 0)
			_ltoa(Camera.Position.v[YDIM], Console.Output[1], 10);
		else if (strcmp(Console.Command[1], "CAMERAZ") == 0)
			_ltoa(Camera.Position.v[ZDIM], Console.Output[1], 10);
		else if (strcmp(Console.Command[1], "ANGLEX") == 0)
			_ltoa(Scene.Angle.v[XDIM], Console.Output[1], 10);
		else if (strcmp(Console.Command[1], "ANGLEY") == 0)
			_ltoa(Scene.Angle.v[YDIM], Console.Output[1], 10);
		else if (strcmp(Console.Command[1], "MUSICTYPE") == 0)
			_ltoa(SoundsManager.music_type, Console.Output[1], 10);
		else if (strcmp(Console.Command[1], "MUSICENCOURS") == 0)
			strncpy(Console.Output[1], Game.CurrentMusic, 39);
		else if (strcmp(Console.Command[1], "MUSICAFFICHEE") == 0)
			strncpy(Console.Output[1], Game.music_affichee, 39);
		else if (strcmp(Console.Command[1], "IDMYMUSIC") == 0)
			_itoa(Game.MusicConsoleOutput, Console.Output[1], 10);

		FontsManager.SetFontSize(FontsManager.GetFontSize()-1);
		FontsManager.DrawFont(30-Console.ViewportPush, 580-5*FontVerticalSpace_temp, Console.Output[1]);
		//fontDrawString(30-ViewportPush, 580-5*FontVerticalSpace_temp, Console.Output[1]);
	}

	glEnable(GL_TEXTURE_2D);

}

/**   
	* @fn display_console_mini_screen
	* Display a Small console
    * This window is generated when it is played in full screen
    * Where is recalculated the position of the window besides the rearrangement of the textures  
*/
void CGUI::display_console_mini_screen()
{
	// Necessary conditions
	if (Application.Window.Width != CGC_XGA_HOR_RES || Application.Window.Height != CGC_XGA_VER_RES)
		return;
	
	if (Console.Start || Console.Moving || Console.Active)
	{
		// activación: se desliza de izquierda a derecha
		if (Console.ComeIn && Console.Moving)
		{
			if (Console.Start)
			{
				Console.ViewportPush = 244.8;	//244.8 = (26+193+25.8))
				Console.Start = false;
			}
			if (Console.ViewportPush - (2.311 *Game.RenderTemp) > 0)		// no deberá exceder la posición de destino 2.311 = ((26+193+26.0)/53)/2
				Console.ViewportPush -= 2.311 *Game.RenderTemp;			//2.311 = ((26+193+26.0)/53)/2
			else
			{
				Console.Moving = false;
				Console.Active = true;
				Console.ComeIn = false;
				Console.ViewportPush = 0;										
			}
		}
		// désactivation : il coulisse de droite à gauche
		else if (!Console.ComeIn && Console.Moving)
		{
			if (Console.Start)
			{
				Console.ViewportPush = 0.0;
				Console.Start = false;
			}
			if (Console.ViewportPush < 244.8) //244.8 = (26+193+25.8))
				Console.ViewportPush += 2.311*Game.RenderTemp;	//2.311 = ((26+193+26.0)/53)/2
			else
			{
				Console.Moving = false;
				Console.Active = false;
				Console.ComeIn = true;
			}
		}

		// affichage du mini-screen
		glViewport (26 - ceil(Console.ViewportPush), 126, 193, 191*3/4);
		CGL_ResetProjection();
		gluPerspective( 45.0, (GLfloat)(CGC_XGA_HOR_RES)/(GLfloat)(CGC_XGA_VER_RES), 0.1f, 500.0 ); // Set Up Perspective Mode To Fit 1/4 The Screen (Size Of A Viewport)
		CGL_ResetModelView();												// Reset The Modelview Matrix
		Camera.SetLook();

		glClear (GL_DEPTH_BUFFER_BIT);

		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);

		// mini-écran : fond noir sans transparence (couche 1)
		glColor4f (0.0f, 0.0f, 0.0f, 1.0f);
		glBegin (GL_QUADS);
			glTexCoord2f (0,0);	glVertex3f (-12,  12, -.15f);
			glTexCoord2f (1,0);	glVertex3f (-12, -12, -.15f);
			glTexCoord2f (1,1);	glVertex3f ( 12, -12, -.15f);
			glTexCoord2f (0,1);	glVertex3f ( 12,  12, -.15f);
		glEnd ();

		// mini-écran : fond gris avec transparence (couche 2)
		glEnable(GL_BLEND);
		glColor4f (1.0f, 1.0f, 1.0f, 0.0f);//0.15 0.05
		glBegin (GL_QUADS);
			glTexCoord2f (0,0);	glVertex3f (-12,  12, -.15f);
			glTexCoord2f (1,0);	glVertex3f (-12, -12, -.15f);
			glTexCoord2f (1,1);	glVertex3f ( 12, -12, -.15f);
			glTexCoord2f (0,1);	glVertex3f ( 12,  12, -.15f);
		glEnd ();

		glRotatef(Scene.Angle.v[YDIM], 1.0, 0.0, 0.0);								// rotation utilisateur (par rapport à l'axe des x)
		glRotatef(Scene.Angle.v[XDIM], 0.0, 1.0, 0.0);								// rotation utilisateur (par rapport à l'axe des y)

		glEnable(GL_TEXTURE_2D);

		display_background(true);
		display_below_lives();											// Nombre de vies
		display_score_hiscore_lives();									// Score, Hiscore, Lives
		//v prueba, creo que no hace falta, ya esta en el render general... BunkersManager.Render();										// Bunkers
		//v prueba, creo que no hace falta, ya esta en el render general... ShootsManager.Render();

		// PARAMETRES 3D (DEBUT)
		if (CHAR_2D != Game.RenderMode)
		{
			glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
			glEnable(GL_CULL_FACE);
			glFrontFace(GL_CCW);
			glEnable(GL_DEPTH_TEST);
			glDisable(GL_BLEND);
		}
		// LUMIERE DES SHIPS (DEBUT)
		if (CHAR_2D != Game.RenderMode)
		{
			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT1);
		}

		//v prueba, creo que no hace falta, ya esta en el render general... Navy.Render();

		//v prueba, creo que no hace falta, ya esta en el render general... if (Player[CurrentPlayer].Alive)
			//Player[CurrentPlayer].Render();

		// LUMIERE DES SHIPS (FIN)
		if (CHAR_2D != Game.RenderMode)
		{
			glDisable(GL_LIGHT1);
			glDisable(GL_LIGHTING);
			
			glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
			glDisable(GL_DEPTH_TEST);
			glEnable (GL_BLEND);
			glDisable(GL_CULL_FACE);
		}
	}
}

/**   
	* @fn display_text_frames
	* Display text frames
    * This function is introduced to the window frames
    * Pear this depends on the size of the window frames will inputting the coordinates of the window
*/
void CGUI::display_text_frames()
{
	// si la résolution est non standard on n'affiche pas les cadres
	if (   (Application.Window.Width != CGC_UXGA_HOR_RES	|| Application.Window.Height != CGC_UXGA_VER_RES)
		&& (Application.Window.Width != CGC_SXGA_HOR_RES	|| Application.Window.Height != CGC_SXGA_VER_RES43)
		&& (Application.Window.Width != CGC_XGA_HOR_RES		|| Application.Window.Height != CGC_XGA_VER_RES)
		&& (Application.Window.Width != CGC_SVGA_HOR_RES	|| Application.Window.Height != CGC_SVGA_VER_RES)
		&& (Application.Window.Width != CGC_VGA_HOR_RES		|| Application.Window.Height != CGC_VGA_VER_RES) )
		return;

	glPushMatrix();
	glLoadIdentity ();
	Camera.LookFrom(0.0, 0.0, 20.0);
	Camera.LookAt(	0.0, 0.0, 0.0);

	glDisable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glColor4f (1.0f, 1.0f, 1.0f, 1.0f);

	// CADRE HAUT-DROITE  ¯|
	TexturesManager.Textures[CTM_CADRE_CENTRE]->SetTexture();
	float lw = 0;								// lang Width : must be resized depending on the language
	if (LANG_FRENCH == Application.Window.Lang)	lw = 0.2; else if (LANG_SPANISH == Application.Window.Lang) lw = 0.4;
	float lx = 0;									// lang x
//	if (LANG_FRENCH == Application.Window.Lang)	lx = -0.1; else if (LANG_SPANISH == Application.Window.Lang) lx = -0.2;
	glBegin (GL_QUADS);
		glTexCoord2f (0,1);	glVertex2f (8.7f +lx - 1.8f, +6.6f +1.20f);
		glTexCoord2f (0,0);	glVertex2f (8.7f +lx - 1.8f, +6.6f -0.9f);
		glTexCoord2f (1,0);	glVertex2f (8.7f +lx + 1.8f + lw, +6.6f -0.9f);
		glTexCoord2f (1,1);	glVertex2f (8.7f +lx + 1.8f + lw, +6.6f +1.20f);
	glEnd ();

	// CADRE BAS-GAUCHE  |_
	if (LANG_FRENCH == Application.Window.Lang)	lw = 0.18; else if (LANG_SPANISH == Application.Window.Lang) lw = -0.25; else lw = 0;
	glBegin (GL_QUADS);
		glTexCoord2f (0,1);	glVertex2f (-8.7f - 1.80f, -8.4f +1.13f);
		glTexCoord2f (0,0);	glVertex2f (-8.7f - 1.80f, -8.4f +0.30f);
		glTexCoord2f (1,0);	glVertex2f (-8.7f + 1.25f + lw, -8.4f +0.30f);
		glTexCoord2f (1,1);	glVertex2f (-8.7f + 1.25f + lw, -8.4f +1.13f);
	glEnd ();

	// CADRE HAUT-GAUCHE  |¯
	TexturesManager.Textures[CTM_CADRE_CENTRE_LONG]->SetTexture();
	glBegin (GL_QUADS);
		glTexCoord2f (0,1);	glVertex2f (-8.7f - 1.8f, +6.6f +1.30f);
		glTexCoord2f (0,0);	glVertex2f (-8.7f - 1.8f, +6.6f +0.20f);
		glTexCoord2f (1,0);	glVertex2f (-8.7f + 5.4f, +6.6f +0.20f);
		glTexCoord2f (1,1);	glVertex2f (-8.7f + 5.4f, +6.6f +1.30f);
	glEnd ();

	glPopMatrix();
}

/**   
	* @fn display_Health
	* Show the player's health on the screen
    * This function displays the player on screen health
    * Also being showing the life disminusicion
*/
void CGUI::display_Health()
{ //Afegir vida player 2 
	boometer->Render(Player[CurrentPlayer].Health/Player[CurrentPlayer].MaxHealth);
}

/**   
	* @fn DisplayPressKey
	* Display the buttons to press
    * This function displays the button is pressed
*/
void CGUI::DisplayPressKey()
{

	Game.ReshapeGL();
	glClear  (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
	TexturesManager.Textures[CTM_PRESSKEY]->SetTexture();
	glBegin (GL_QUADS);
		glTexCoord2f (0,0);glVertex3f ( -3.0*1.3f, 0.50*1.3f, .05f);
		glTexCoord2f (1,0);glVertex3f (  3.0*1.3f, 0.50*1.3f, .05f);
		glTexCoord2f (1,1);glVertex3f (  3.0*1.3f, 1.70*1.3f, .05f);
		glTexCoord2f (0,1);glVertex3f ( -3.0*1.3f, 1.70*1.3f, .05f);
	glEnd ();

}

/**   
   * @fn DisplayPaused
   * Show display paused
   * This function will pause the game window
*/
void CGUI::DisplayPaused()
{
	glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
	TexturesManager.Textures[CTM_PAUSED]->SetTexture();
	glBegin (GL_QUADS);
		glTexCoord2f (0,0);glVertex3f ( -3.0*1.3f, 0.50*1.3f, .05f);
		glTexCoord2f (1,0);glVertex3f (  3.0*1.3f, 0.50*1.3f, .05f);
		glTexCoord2f (1,1);glVertex3f (  3.0*1.3f, 1.70*1.3f, .05f);
		glTexCoord2f (0,1);glVertex3f ( -3.0*1.3f, 1.70*1.3f, .05f);
	glEnd ();
}

/**   
	* @fn FadingInWon
	* Show the Fading animation presentation
    * The actual score

*/
void CGUI::FadingInWon (void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				// efface les buffers

	// diminution du volume de la musique de la dialog box 'About'
	#ifdef UGKSND_FMOD375
	FMUSIC_SetMasterVolume(SoundsManager.FadingModule,(int)((1-GUI.Fading)*UGKSND_MAX_VOLUME));
	#elif UGKSND_FMODEx
	#elif UGKSND_OAL
	#elif UGKSND_SDL
	#endif

	// Showing the Loading bmp on the screen
	Game.LoadingBMPPosition = 0.3+sqrt(sqrt(1/GUI.Fading));
	//Game.DisplayLoadingBMP();
	Game.DisplayScore();

	// on gère l'éclaircissement de l'image "loading..."
	if (GUI.Fading < 1.0)
		
		GUI.Fading+= 0.0009 * Game.RenderTemp;//0.0007
	else 
	{
	#ifdef UGKSND_FMOD375
		FMUSIC_StopSong(SoundsManager.FadingModule);
		FMUSIC_FreeSong(SoundsManager.FadingModule);
	#elif UGKSND_FMODEx
	#elif UGKSND_OAL
	#elif UGKSND_SDL
	#endif

		//Passing from CSIG_CONFIGURE state to CSIG_FADING_IN state
		Game.GameEvent(CSIG_FADED);		//v Controlado 11->9, 13->10, 12->14
	}

	glFlush ();															// flush the GL rendering pipeline
}

/**   
	* @fn FadingInAnimation
	* Show the Fading animation presentation
    * The animation begins to move Logo Space Invader, while the discoloration fades
    * For this you use the buffers, which also decreases other music from the animation

*/
void CGUI::FadingInAnimation (void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				// efface les buffers

	// diminution du volume de la musique de la dialog box 'About'
	#ifdef UGKSND_FMOD375
		FMUSIC_SetMasterVolume(SoundsManager.FadingModule,(int)((1-GUI.Fading)*UGKSND_MAX_VOLUME));
	#elif UGKSND_FMODEx
	#elif UGKSND_OAL
	#elif UGKSND_SDL
	#endif

	// Showing the Loading bmp on the screen
	Game.LoadingBMPPosition = 0.3+sqrt(sqrt(1/GUI.Fading));
	Game.DisplayLoadingBMP();

	// on gère l'éclaircissement de l'image "loading..."
	if (GUI.Fading < 1.0)
		GUI.Fading+= 0.0009 * Game.RenderTemp;//0.0007
	else 
	{
		#ifdef UGKSND_FMOD375
			FMUSIC_StopSong(SoundsManager.FadingModule);
			FMUSIC_FreeSong(SoundsManager.FadingModule);
		#elif UGKSND_FMODEx
		#elif UGKSND_OAL
		#elif UGKSND_SDL
		#endif
	
		//Passing from CSIG_CONFIGURE state to CSIG_FADING_IN state
		Game.GameEvent(CSIG_LOADING);//v Controlado 1->2
	}

	glFlush ();															// flush the GL rendering pipeline
}
/**   
	* @fn FadingOutLoading
	* View the Fading out of charge
    * This is done to Fading is loaded when a game is completed loaded bar
    * For this you use the buffers
*/
void CGUI::FadingOutLoading(void)
{
	static float Fading = 1.0f;											// Maximum lighting
	static float rotation_angle_total = 0;
	
	Application.Window.CreateRenderingContext();

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				// Erase graphic buffers

	// rotation du l'image loading
	rotation_angle_total = (pow((float)1-Fading, (float)1.3)) * 90;

	glLoadIdentity	();
	Camera.LookFrom	(0.0, 0.0, 10.0);
	Camera.LookAt	(0.f, 0.f,  0.f);

	glRotatef(rotation_angle_total, 0.0f, 1.0f, 1.0f);

	// diminution de la luminosité de l'écran
	glColor4f (1.0f, 1.0f, 1.0f, Fading);
	if (Fading > 0.0)
		Fading-= 0.0015 * Game.RenderTemp;//0.0014
	else{
		///Passing to PLAYING state
		Game.GameEvent(CSIG_LOADED);		//v Controlado 3->15
	}
	// on affiche toutes les entrées du log
	FontsManager.SetFontSize(8);						// taille du texte des logs
	if (Application.Window.Width >= CGC_SVGA_HOR_RES)	// logs affichés si résolution >= 800x600
		for(int i=0; i<=Log.LogIndex; i++)				// pour toutes les entrées de log
		{
			FontsManager.SetFontSize ();
			if (Application.Window.Width >= CGC_XGA_HOR_RES) 
					FontsManager.SetFontColor (Fading);
			else	FontsManager.SetFontColor (Fading-0.2);
			//fontDrawString (10, Application.Window.Height -30 -(FontsManager.GetFontSize()+2)*i, "%s",  Log.LogStrings[i]);
			FontsManager.DrawFont (10, Application.Window.Height -30 -(FontsManager.GetFontSize()+2)*i, "%s",  Log.LogStrings[i]);
		}

		// Putting image "loading..." on the screen
		float j = 0.3 + sqrt(Fading);
		glColor4f (1.0f, 1.0f, 1.0f, Fading);
		TexturesManager.Textures[CTM_LOADING]->SetTexture();
		glBegin (GL_QUADS);
			glTexCoord2f (0,0);glVertex3f ( -3.0*j, 0.10*j, .05f);
			glTexCoord2f (1,0);glVertex3f (  3.0*j, 0.10*j, .05f);
			glTexCoord2f (1,1);glVertex3f (  3.0*j, 1.30*j, .05f);
			glTexCoord2f (0,1);glVertex3f ( -3.0*j, 1.30*j, .05f);
		glEnd ();

		glFlush ();															// flush the GL rendering pipeline
}
