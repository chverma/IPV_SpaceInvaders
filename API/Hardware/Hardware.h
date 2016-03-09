/**	Definition of the class CHardware. Base class for all hardware in the system

*	Prefix: CHW_

*	@author Ramon Molla
*	@version 2013-06
*/

#ifndef CHW_HARDWARE
#define CHW_HARDWARE

#include <HRTimerManager.h>
#include <list>

/**
* Definition enum type CHW_HARDWARE_TYPE for clasiffy SOUND, IN/OUT ...
*/

typedef enum {
	CHW_SOUND,		//Output sound through a sounds card
	CHW_GRAPHIC,	//Output video through a graphics card
	CHW_IMAGE,		//Input image through a typical webcam
	CHW_VIDEO,		//Input video through a typical webcam
	CHW_INPUT,		///<Generic input device. Typically a modem or a
	CHW_OUTPUT,
	CHW_MAX_HW_TYPE
} CHW_HARDWARE_TYPE;

typedef enum {
	CHW_SND_NO_SOUND,
	CHW_SND_MAX_DEVICE
} CHW_SOUND_TYPE;

typedef enum {
	CHW_GRA
} CHW_GRAPH_TYPE;

typedef enum {
	CHW_IN_KEYBOARD,		///< Device Keyboard
	CHW_IN_MOUSE,			///< Device Mouse
	CHW_IN_JOYSTICK,		///< Device Joystick
	CHW_IN_WIIMOTE,			///< Device Wiimote
	CHW_IN_KINECT,			///< Device Kinect
	CHW_IN_WEBCAM,			///< Device Web Cam
	CHW_IN_LEAP,			///< Device Leap Motion
	CHW_IN_GLOVE,			///< Device Guante
	CHW_IN_GENERIC,			///< Device Generic
	CHW_IN_MIC,				///< Device Microphone
	CHW_IN_MAX_DEVICE
} CHW_INPUT_TYPE;

typedef enum {
	CHW_OUT_LOUD_SPEAKERS,
	CHW_OUT_GRAPHIC,
	CHW_OUT_MAX_DEVICE
} CHW_OUTPUT_TYPE;

/**
*	CHardware

*	Manages all the information relative to the general game application.
*	It is implemented as a singleton. This is not the game singleton
*/

class CHardware
{
private:

public:

	cHRTimerManager HRTimerManager;		///<HR counters for delaying the sensitiveness of the device

	//Atributes
	/// Type of hardware: sound, graphics, I/O,...
	unsigned short int	SubTypeHW;
	CHW_HARDWARE_TYPE	TypeHW;
	/// The hardware is active. Implicitly it needs to be alive in order to be active
	bool				Active;

	//Methods
	void Init (void);
	CHardware(){};		///<Constructor of the class
	~CHardware(){};		///<Destructor of the class
};

#endif