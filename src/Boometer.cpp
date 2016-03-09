/**
*	CBoometer
*	Defines some methods of the class of a boometer

*	@author 
*	@version 2014-05
*/

#include <Boometer.h>
#include <gl.h>

extern CTexturesManager TexturesManager;
using namespace UGK;

CBoometer::CBoometer( UGK::Vector orig, float h, float w, CTM_Textures textRef)
{
	Origin.v[XDIM] = orig.v[XDIM];
	Origin.v[YDIM] = orig.v[YDIM];
	Height = h;
	Width = w;
	TextureRef = textRef;
}

void CBoometer::Render(float levelRatio)
{
	//Disable all test
	GLboolean switchBlend	= glIsEnabled(GL_BLEND);

	GLboolean switchZ		= glIsEnabled(GL_DEPTH_TEST);
	if(switchBlend)	glDisable(GL_BLEND);
	if(switchZ)		glDisable(GL_DEPTH_TEST);
	
	glPushMatrix();

	float innerWidth = (Width*levelRatio);
	
	//drawing frame of boometer
	glBegin(GL_LINE_STRIP);
	glColor3f( 0.906f, 0.906f, 0.094f);	// golden color
	glVertex2f(Origin.v[XDIM], Origin.v[YDIM]);//vertex 1
	glVertex2f(Origin.v[XDIM] + Width, Origin.v[YDIM]);//vertex 2
	glVertex2f(Origin.v[XDIM] + Width, Origin.v[YDIM] + Height); //vertex 3
	glVertex2f(Origin.v[XDIM], Origin.v[YDIM] + Height); //vertex 4
	glVertex2f(Origin.v[XDIM], Origin.v[YDIM]);//vertex 1, closing the frame
	glEnd();

	//drawing inner rectangle of boometer
	if(levelRatio >0)
	{
		TexturesManager.Textures[TextureRef]->SetTexture();
		glBegin(GL_QUADS);
		glTexCoord2f (0, 0);			glVertex2f(Origin.v[XDIM], Origin.v[YDIM]);//vértice 1
		glTexCoord2f (levelRatio, 0);	glVertex2f(Origin.v[XDIM] + innerWidth, Origin.v[YDIM]);//vértice 2
		glTexCoord2f (levelRatio, 0.9);	glVertex2f(Origin.v[XDIM] + innerWidth, Origin.v[YDIM] + Height); //vértice 3
		glTexCoord2f (0, 0.9);			glVertex2f(Origin.v[XDIM], Origin.v[YDIM] + Height); //vértice 4
		glEnd();
	}

	glPopMatrix();

	glColor4f(1, 1, 1, 1);
	//Enable if they were enabled
	if(switchBlend)	glEnable(GL_BLEND);
	if(switchZ)		glEnable(GL_DEPTH_TEST);
}