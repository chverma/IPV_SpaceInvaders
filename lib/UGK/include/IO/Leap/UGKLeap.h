/**	
*	Prefix: 
*	@Author: Juan Cerrón, Cédric Marco
*	@version: 2013-14
**/

#ifndef USELEAP_H
#define USELEAP_H

#include <UGKLeapGestureTreatment.h>

namespace UGK
{
	class UGK_API  UseLeap{

	  public:
		int leapInit();
		int leapExit();

	private:
		Leap::Controller controller;
		CGestureTreatment detectorMovimientos;
	};
}
#endif