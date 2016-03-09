/*	Declaration of the class Progress Bar

	Prefix: CPB_

	@author Ramon Molla
	@version 2011-10
*/

#include "Algebra.h"

#ifndef CPB_PROGRESSBAR
#define CPB_PROGRESSBAR

#define CPB_MAX_VALUE	    100.0f

#define CPB_FRAME_THICKNESS  0.1f
#define CPB_PBAR_LENGTH		10.0f
#define CPB_PBAR_HEIGHT		 0.8f

class CPBar
{
	//Atributes
public:

	float	Value,	//Absolute value for the progressive bar at a given moment
			Height,	//Height of the progressive bar. Initialy including the frame
			Length,	//Lenght of the progressive bar. Initialy including the frame
			Fading;	//Interface Fading percentage

	Vector	Min,	//Left down corner
			Max;	//Right up corner

	//Methods
	CPBar();			//Constructor of the class

	void CalculateMinMax(void);
	void Reset			(void);

	void Render			();

private:
	void RenderRect		();	//Renders the internal rectangle or the frame of the progressive bar
};

#endif