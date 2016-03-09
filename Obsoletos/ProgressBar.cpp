/*	Definition of the class Progress Bar

	Prefix: CPB_

	@author Ramon Molla
	@version 2011-10
*/

#include "ProgressBar.h"
#ifndef WINDOWS_LOADED
#define WINDOWS_LOADED
#include <windows.h>
#endif
#include <gl/gl.h>

//Constructor
CPBar::CPBar()
{
	Value = 0.0f;
	Fading = 1.0;	//Maximun brightness
	Reset();
}

void CPBar::CalculateMinMax (void)
{
	Max.v[X3D] =  Length / 2;
	Max.v[Y3D] =  Height / 2;
	Min.v[X3D] = -Max.v[X3D];
	Min.v[Y3D] = -Max.v[Y3D];
}

//Initializer
void CPBar::Reset (void)
{
	Height	 = CPB_PBAR_HEIGHT;
	Length	 = CPB_PBAR_LENGTH;

	CalculateMinMax ();
}

void CPBar::RenderRect()
{
	glVertex2f(Min.v[X3D],	Max.v[Y3D]);
	glVertex2f(Max.v[X3D],	Max.v[Y3D]);
	glVertex2f(Max.v[X3D],	Min.v[Y3D]);
	glVertex2f(Min.v[X3D],	Min.v[Y3D]);
}

void CPBar::Render()
{
	//glDisable(GL_TEXTURE_2D);
	//glDisable(GL_BLEND);

	Reset();

	// Frame
	glColor4f (0.5f, 0.5f, 0.5f, 1.0f);
	glLineWidth(2.0f);
	glBegin(GL_LINE_LOOP);
		RenderRect();
	glEnd();

	// progressive rectangle inside
	Length -= CPB_FRAME_THICKNESS*2;	//One thickness for the right frame and another one for the left one
	Height -= CPB_FRAME_THICKNESS*2;	//One thickness for the up frame and another one for the bottom one
	CalculateMinMax ();

	Max.v[X3D] = ((Value/CPB_MAX_VALUE)*2-1)*Length/2;
	glColor4f (0.97f, 0.97f, 0.85f, 1.0f);
	glBegin(GL_QUADS);
		RenderRect();
	glEnd();

	Value++;

	Length += CPB_FRAME_THICKNESS;
	Height += CPB_FRAME_THICKNESS;
	
	//glEnable(GL_TEXTURE_2D);
	//glEnable(GL_BLEND);
}