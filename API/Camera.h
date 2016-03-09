/*	Definition of the class Camera

	Prefix: CC_

	@author Ramon Molla
	@version 2013-07
*/

#ifndef CC_CAMERA //Se borra la macro Camera
#define CC_CAMERA //Se define la macros Camera

#include <Character.h>

/**
	It defines new variable types enumerated (listed) with the name representing the cameras CC_CAMERA_TYPE game
*/
typedef enum {
	CC_ORTOGONAL,
	CC_PERSPECTIVE,
	CC_MAX_CAMERA_TYPE
} CC_CAMERA_TYPE;

/**
	Class is defined CCamera also derive or inherit from class CCharacter
*/
class CCamera: public CCharacter
{
	//Atributes
public:

	CC_CAMERA_TYPE	Type;		/// Camera type

	GLfloat zoom,				///<ZOOM
			zoom_total,			///<Camera Camera.zoom
			zoom_total_bonus;	///<ZOOM TOTAL while passing to 3D mode

	Vector	PointsTo,			///<Objetive point of the camera
			Rotation,			///<Angle
			Up;					///<Vertical vector


	//Methods
	void Init (void);

	inline CCamera(){Init();}	///<Constructor of the class

	inline void SetLook	(); ///<Overwrite LookAt where the camera is called
	void LookAt			(float x, float y, float z);
	void LookFrom		(float x, float y, float z);

	/// Updates the camera settings when the Window is resahaped or moved
	void Reshape		(float Z);

};
#endif
