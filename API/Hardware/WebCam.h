/** Header with definitions needed for the management of any generic web camera
*	Prefix: CWC_	for class Web Cam

*	@author Ramón Mollá
*	@version 2013-07
*/

#ifndef CGC_WEB_CAM
#define CGC_WEB_CAM

#include <Hardware.h>

/**
*	CWebCam

*	Manages all the information relative to the general game application.
*	It is implemented as a singleton. This is not the game singleton
*/

class CWebCam: public CHardware
{
private:
	
public:

	//Methods
	void Init	(void);
	CWebCam		();		///<Constructor of the class
	~CWebCam	();		///Destructor of the class

	//Public methods
};

#endif