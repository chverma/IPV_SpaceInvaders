/*	Definition of the class Equalizer
	General class to create and to display the equalizer
	Prefix: CEQ_

	@author Ramon Molla
	@author David Calabuig
	@version 2013-05
*/


#include "Equalizer.h"
#include "FontsManager.h"
#include <gl/glext.h>


/**   
   * @fn CEqualizer::Init
   * Start Equalizer
   * Builder starts Equalizer movement of the ship which by default is not moving
   * @param 
*/
void CEqualizer::Init()
{
	Rotation.v[X3D]	= Rotation.v[Y3D] = Rotation.v[Z3D] = Position.v[X3D] = Position.v[Y3D]= Position.v[Z3D] = 0.0;
	Moving			= false;	//No movement is performed at the beginning
}

CEqualizer::~CEqualizer()
{
	
}

/**   
   * @fn CEqualizer::display
   * Shows the equalizer on the screen
*/
void CEqualizer::display()
{
	//if (Application.Window.Width < CEQ_MIN_WINDOW_WIDTH) return;

	// mode ortho 2D
	glMatrixMode(GL_PROJECTION);
	glPushMatrix ();
	glLoadIdentity();
	glOrtho(0.0, /*(GLdouble)Application.Window.Width*/0.0, 0.0, /*(GLdouble)Application.Window.Height*/0.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix ();
	glLoadIdentity();

		// EQUALIZER

		// alpha test + blend
		glDisable(GL_ALPHA_TEST);									// alpha test
		//glAlphaFunc(GL_GREATER, 0.9f);							// alpha test
		//v glEnable(GL_BLEND);											// blending
		//v glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);			// for anti-aliasing

		// résolution
		int w = 0.0;//Application.Window.Width;
		int h = 0.0;//Application.Window.Height;

		// paliers de résolutions
		int w2;
		if (w >= 1600) w2 = 1600; else if (w >= 1280) w2 = 1280; else if (w >= 1024) w2 = 1024;	else if (w >= 800) w2 = 800; else w2 = 640;

		// on affiche le cadre de l'equalizer
		glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
		//TexturesManager.Textures[CTM_EQUALIZER]->SetTexture();
		glBegin (GL_QUADS);
			glTexCoord2f (0,1);	glVertex2f (Position.v[X3D]+w+(-183      )*w2/1024, Position.v[Y3D]+h+(-238      )*w2/1024);
			glTexCoord2f (0,0);	glVertex2f (Position.v[X3D]+w+(-183      )*w2/1024, Position.v[Y3D]+h+(-238 + 128)*w2/1024);
			glTexCoord2f (1,0);	glVertex2f (Position.v[X3D]+w+(-183 + 256)*w2/1024, Position.v[Y3D]+h+(-238 + 128)*w2/1024);
			glTexCoord2f (1,1);	glVertex2f (Position.v[X3D]+w+(-183 + 256)*w2/1024, Position.v[Y3D]+h+(-238      )*w2/1024);
		glEnd ();

		// BARRES DE L'EQUALIZER
		
		// on récupère les données du spectrum
		static float barre1[8] = {0,0,0,0,0,0,0,0};
		static float barre2[8] = {0,0,0,0,0,0,0,0};
		float temp[8] = {0,0,0,0,0,0,0,0};

		float* spec ;//= FSOUND_DSP_GetSpectrum();

		// WORK ABOUT SPECTRUM ANALYSIS
/*
		int offset;
		float xoff, step;
		signed short *src;

		//The next pcmblock (Oscblock + 1) is the one that is audible.
		offset = (OscBlock + 1) * FSOUND_DSP_GetBufferLength();
		if (offset >= FSOUND_DSP_GetBufferLengthTotal())
			offset -= FSOUND_DSP_GetBufferLengthTotal();

		src = &SoundsManager.OscBuffer[offset];

		char tempstr[255]="";
		_itoa(*src, tempstr, 10);
		OutputDebugString(tempstr);


		// xoff is the x position that is scaled lookup of the dsp block according to the graphical Window size.
		xoff = 0;
		step = (float)FSOUND_DSP_GetBufferLength() / (float)160;

		//char tempstr[255]="";
		//_itoa(step, tempstr, 10);
		//OutputDebugString(tempstr);
*/	
		// on adapte les données aux barres de l'equalizer
		if (spec)
		//if (SoundsManager.OscBuffer)
			for (int i=0; i<=7; i++)							// pour les 8 barres de l'equalizer
			{
				barre1[i] = barre2[i];							// les barres affichées (barre1[]) sont celles
																// calculées la frame précédente (barre2[])

				for (int j=0; j<=19; j++)						// on récupère les nouvelles valeurs (par tranches de 20, 1 barre = 20 valeurs)
					temp[i] += spec[(i*20)+j];
					//temp[i] += src[(i*20)+j];

				temp[i] /= 4;//3								// on redimensionne la hauteur
				//temp[i] /= 100000/3;//3								// on redimensionne la hauteur

				if (temp[i] > barre2[i])						// si la nouvelle valeur est + grande on met à jour la barre
				{				
					barre2[i] = temp[i];
					if (i != 0 && i != 1 && barre2[i] > 1)		// on limite à la taille de la texture (de 0 à 1)
					//if (barre2[i] > 1)		// on limite à la taille de la texture (de 0 à 1)
						barre2[i] = 1;							// sauf les deux 1ères barres (les basses) qui sont customisées plus bas
				}
				else											// sinon elle décroît
				{
					if (barre2[i] > 0.002/* *Time.ms*/)
						barre2[i] -= 0.002/* *Time.ms*/;
					else
						barre2[i] = 0;
				}
			}

		// customization of some strips for better looking

		barre2[0] = (barre2[0])*0.3;	// customisation pour les sons graves (les deux 1ères barres)
		//barre2[0] = (barre2[0] - 0.4)*0.7;//0.3	// customisation pour les sons graves (les deux 1ères barres)
//		barre2[0] = (barre2[0] -0.1)*0.5;//0.3	// customisation pour les sons graves (les deux 1ères barres)
		if (barre2[0] > 1)
			barre2[0] = 1;

//		barre2[1] = (barre2[1] - 0.0)* 1.0;//0.7
		barre2[1] = barre2[1]*0.5; //0.7 0.5
		barre2[1] += barre2[0]*0.3;//0.2 0.3
		if (barre2[1] > 1)
			barre2[1] = 1;

		if (barre2[2] > 1)
			barre2[2] = 1;

//		barre2[2] += barre2[0]*0.1;

		// on affiche les 8 barres de l'equalizer
		glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
		//TexturesManager.Textures[CTM_EQUALIZER_BAND]->SetTexture();
		glBegin (GL_QUADS);
			for(int i=0; i<=7; i++)
			{
				glTexCoord2f (0,barre2[i]); glVertex2f (this->Position.v[X3D]+w+(-176      + i*17)*w2/1024, this->Position.v[Y3D]+h+(-229 + 128 *barre2[i])*w2/1024);	//86
				glTexCoord2f (0,0);	        glVertex2f (this->Position.v[X3D]+w+(-176      + i*17)*w2/1024, this->Position.v[Y3D]+h+(-229                 )*w2/1024);
				glTexCoord2f (1,0);         glVertex2f (this->Position.v[X3D]+w+(-176 + 16 + i*17)*w2/1024, this->Position.v[Y3D]+h+(-229                 )*w2/1024);	//14
				glTexCoord2f (1,barre2[i]); glVertex2f (this->Position.v[X3D]+w+(-176 + 16 + i*17)*w2/1024, this->Position.v[Y3D]+h+(-229 + 128 *barre2[i])*w2/1024);
			}
		glEnd ();

		// retour aux anciens paramètres (alpha test et blending)
		glDisable(GL_ALPHA_TEST);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);

	glMatrixMode (GL_PROJECTION);
	glPopMatrix ();
	glMatrixMode (GL_MODELVIEW);
	glPopMatrix ();

	// active/désactive le DSP si la touche <M> a été appuyée
	/*if (!music_on && FSOUND_DSP_GetActive(FSOUND_DSP_GetFFTUnit()))
		for (int i=0; i<=7; i++)
		{
			if (barre2[i] > 0)
				break;
			FSOUND_DSP_SetActive(FSOUND_DSP_GetFFTUnit(), FALSE);
		}
	else if (music_on && !FSOUND_DSP_GetActive(FSOUND_DSP_GetFFTUnit()))
		FSOUND_DSP_SetActive(FSOUND_DSP_GetFFTUnit(), TRUE);*/



	//glPushMatrix();
	//	glLoadIdentity ();
	//	gluLookAt(	0.0, 0.0, 20.0,									// l'oeil
	//				0.0, 0.0,  0.0,									// le centre
	//				0.0, 1.0,  0.0	);								// le haut

	//	// equalizer
	//	glColor4f (0.6f, 0.6f, 0.6f, 1.0f);
	//	glBindTexture (GL_TEXTURE_2D, TexturesManager.Textures[CTM_EQUALIZER].Id);

	//	// ALPHA TEST + BLEND
	//	glAlphaFunc(GL_GREATER, 0.9f);								// for TGA alpha test
	//	glEnable(GL_ALPHA_TEST);									// for TGA alpha test
	//	
	//	glDisable(GL_BLEND);
	//	//glDisable(GL_DEPTH_TEST);
	//	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);			// for anti-aliasing

	//	float ratio_x = 22.0 / Application.Window.Width;					// 22.0 = largeur écran
	//	float ratio_y = 16.5 / Application.Window.Height;					// 16.5 = hauteur écran
	//	
	//	glBegin (GL_QUADS);
	//		glTexCoord2f (0,1);	glVertex2f (8.7f - 1.6f              , 3.2f + 102*ratio_y);
	//		glTexCoord2f (0,0);	glVertex2f (8.7f - 1.6f              , 3.2f + 0.00f);
	//		glTexCoord2f (1,0);	glVertex2f (8.7f - 1.6f + 147*ratio_x, 3.2f + 0.00f);
	//		glTexCoord2f (1,1);	glVertex2f (8.7f - 1.6f + 147*ratio_x, 3.2f + 102*ratio_y);
	//	glEnd ();

	//	// BACK TO NON ALPHA TEST + PREVIOUS BLEND
	//	glDisable(GL_ALPHA_TEST);
	//	
	//	// EQUALIZER BARRES
	//	glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
	//	glBindTexture (GL_TEXTURE_2D, TexturesManager.Textures[CTM_EQUALIZER_BAND].Id);
	//	for(int i=0; i<=7; i++)
	//	{
	//		glBegin (GL_QUADS);
	//			glTexCoord2f (0,1);	glVertex2f (8.7f - 1.45f              + i*17*ratio_x, 3.37f + 86*ratio_y);
	//			glTexCoord2f (0,0);	glVertex2f (8.7f - 1.45f              + i*17*ratio_x, 3.37f + 0.00f);
	//			glTexCoord2f (1,0);	glVertex2f (8.7f - 1.45f + 14*ratio_x + i*17*ratio_x, 3.37f + 0.00f);
	//			glTexCoord2f (1,1);	glVertex2f (8.7f - 1.45f + 14*ratio_x + i*17*ratio_x, 3.37f + 86*ratio_y);
	//		glEnd ();
	//	}

	//	// BACK TO PREVIOUS BLEND
	//	glEnable(GL_BLEND);
	//	glBlendFunc(GL_SRC_ALPHA,GL_ONE);

	//glPopMatrix();
}

