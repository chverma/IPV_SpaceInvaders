/**	Listener class to detect movements with Leap device
*	Prefix: GR_	Gesture Recognizer
*	@Author: Juan Cerrón, Cédric Marco
*	@version: 2013-14
**/

// If the macro of the Leap is not defined it is declared.
#ifndef CGESTURERECOGNIZER_H
#define CGESTURERECOGNIZER_H

#include <Leap.h>
#include <UGK.h>

typedef enum {
	GR_LEFT_MOVE,	// Code for left move
	GR_STOP_MOVE,	// Code for stop move
	GR_RIGHT_MOVE,	// Code for right move
	GR_TOP_MOVE, // Code for top move
	GR_FORWARD_MOVE, // Code for moving forward
	GR_BACKWARD_MOVE, // Code for moving backward
	GR_BOTTOM_MOVE, // Code for bottom move
	GR_QUICK_SHOOT_MOVE, // Code for normal shooting
	GR_SLOW_SHOOT_MOVE, // Code for special shooting
	GR_MAX_MOVE	// Amount of extra lasers the player has
} GR_GESTURE_TYPE;

using namespace Leap;

namespace UGK
{
class UGK_API CGestureRecognizer : public Listener {
  
private:
	Frame m_lastFrame;

	virtual void onInit(const Controller&);
    virtual void onExit(const Controller&);
    virtual void onConnect(const Controller&);
    virtual void onDisconnect(const Controller&);
    virtual void onFrame(const Controller&);
    virtual void onFocusGained(const Controller&);
    virtual void onFocusLost(const Controller&);

public:

	virtual void GestureTreatment (GR_GESTURE_TYPE GT) = 0;
};
}
#endif