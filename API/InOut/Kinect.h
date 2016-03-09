/**	Methods for using a kinect device
*	Prefix: KNT_
*	@Author: Jordi Mansanet
*	@version: 2011-12
**/

#ifndef KINECT_H //Se borra la macro del Kinect
#define KINECT_H //Se define la macro del Kinect
#include <Hardware\Hardware.h>

#ifdef CHW_USE_KINECT

//-----------------------------------------------------------------------------
// Headers
//-----------------------------------------------------------------------------

// OpenNI headers
#include <XnOpenNI.h> //Librerio para Kinect NI(Interaccion Humana)
// NITE headers
#include <XnCppWrapper.h>
#include <XnVHandPointContext.h>
#include <XnVSessionManager.h>
#include <XnVFlowRouter.h>
#include <XnVSelectableSlider1D.h>

#include "signal_catch.h"

/**   
	* It defines the types of movements kinect
    * @param enum K_SHIP_MOVE which lists the moves allowed the kinect
*/
typedef enum {
	K_LEFT_MOVE,	// Code for left move
	K_STOP_MOVE,	// Code for stop move
	K_RIGHT_MOVE,	// Code for right move
	K_MAX_MOVE	// Amount of extra lasers the player has
} K_SHIP_MOVE;

#define KNT_MSJ_MAX_LONG 128	//Maximun length of a msj for the user

typedef enum {
	KNT_BAD_ITEM,
	KNT_SLIDER_BAD_DIR,
	KNT_WRONG_DIR,
	KNT_LEFT,
	KNT_CENTER,
	KNT_RIGHT,
	KNT_WRONG_VALUE_SLIDER,
	KNT_INIT_FROM_XML,
	KNT_INIT_FROM_XML_FILE,
	KNT_FIND_DEPTH_GEN,
	KNT_COULD_NOT_INIT_SESSION,
	KNT_WAVE_GESTURE,
	KNT_HIT_ANY_2_EXIT,
	KNT_ERROR_MAX_MSG
} KNT_KinectMsg;

/**   
   * Starts an example of kinect to use
*/
//#define SAMPLE_XML_FILE "C:/Program Files/PrimeSense/NITE/Data/Sample-Tracking.xml"
//#define SAMPLE_XML_FILE_LOCAL "Sample-Tracking.xml"
#define SAMPLE_XML_FILE "C:/Program Files/PrimeSense/NITE/Data/SamplesConfig.xml"
#define SAMPLE_XML_FILE_LOCAL "SamplesConfig.xml"

/**   
   * We define a function of connectivity Kinect review
*/
#define CHECK_RC(rc, what)											\
	if (rc != XN_STATUS_OK)											\
{																\
	printf("%s failed: %s\n", what, xnGetStatusString(rc));		\
	return rc;													\
}

/**   
   * We define a function for review of errors that are generated with the Kinect
*/
#define CHECK_ERRORS(rc, errors, what)		\
	if (rc == XN_STATUS_NO_NODE_PRESENT)	\
{										\
	XnChar strError[1024];				\
	errors.ToString(strError, 1024);	\
	printf("%s\n", strError);			\
	return (rc);						\
}

// FUNCTIONS
XnBool fileExists(const char *fn);
int initKinect();// start kinect
bool waitKinect();// kinect is waiting to person perform wave gesture
K_SHIP_MOVE updateKinect();// kinect returns new hand position
void CleanupExit();

//-----------------------------------------------------------------------------
// Callbacks
//-----------------------------------------------------------------------------
void XN_CALLBACK_TYPE MainSlider_OnSelect(XnInt32 nItem, XnVDirection dir, void* cxt);// detect selection
void XN_CALLBACK_TYPE MainSlider_OnValueChange(XnFloat fValue, void* cxt);// detect hand position inside the slider
void XN_CALLBACK_TYPE MainSlider_OnActivate(void* cxt); // Activo  
void XN_CALLBACK_TYPE MainSlider_OnDeactivate(void* cxt); // Desactivado
void XN_CALLBACK_TYPE MainSlider_OnPrimaryCreate(const XnVHandPointContext* hand, const XnPoint3D& ptFocus, void* cxt);
void XN_CALLBACK_TYPE MainSlider_OnPrimaryDestroy(XnUInt32 nID, void* cxt);


#endif

#endif
