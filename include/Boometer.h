/**
*	Defines the class of a boometer control that draws a level of some magnitude

*	@author 
*	@version 2014-01
*/

#ifndef BOOMETER_H
#define BOOMETER_H
#include "SITexturesResources.h"
#include <UGKAlgebra.h>

class CBoometer
 {
 private:
	 	 
	 ///Position of the lower left corner of the control
	 UGK::Vector Origin;

	 ///Height of the control
	 float Height;

	 ///Width of the control
	 float Width;

	 //Texture reference
	 CTM_Textures TextureRef;

 public:
	 ///Constructor of the class
	 CBoometer(UGK::Vector, float, float, CTM_Textures);

	 ///Destructor of the class
	 ~CBoometer();

	 ///Renders the boometer with the level indicated, should be ratio level/maxLevel
	 void Render(float);


 };
#endif