/**	
*	Prefix: 
*	@Author: Juan Cerrón, Cédric Marco
*	@version: 2013-14
**/

#ifndef CGESTURETREATMENT_H
#define CGESTURETREATMENT_H

#include <UGKLeapGestureRecognizer.h>

namespace UGK
{
	class UGK_API CGestureTreatment : public CGestureRecognizer {

	public:
		CGestureTreatment(void);
		~CGestureTreatment(void);

		void GestureTreatment(GR_GESTURE_TYPE GT);
	};
}
#endif