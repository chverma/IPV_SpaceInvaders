/*/////////////////////////////////////////////////////////////////////////////////////
//
// Camera class declarations
//

	@author Ramon Molla
	@version 2007-09-11
*/

#include "Physics.h"
#include "Camera.h"
#include "glError.h"

/**
   * @fn CCamera:: Init
   * Constructor for the object CCamera
*/
void CCamera::Init(void)
{
	RenderMode = CHAR_NO_RENDER;
	PhysicMode = CHAR_PHANTOM;		//No render, No touchable. No AABB initialization is required

	zoom = zoom_total= zoom_total_bonus	= 0; //Se Inician las variables en zoom, zoom_total, zoom_total_bonus en 0
}


/**
    * Define function LookAt line that starts the camera position and objectives, in addition to the top of the camera
*/
inline void CCamera::LookAt()
{
	gluLookAt(	Position.v[X3D], Position.v[Y3D], Position.v[Z3D],	/// From the position of the camera
				PointsTo.v[X3D], PointsTo.v[Y3D], PointsTo.v[Z3D],	/// Objetive
				0.0, 1.0,  0.0	);									/// Height
}


/**
    * Define function LookAt for objectives with coordinates x, y, z
	* @param x: type int coordinate x in space
	* @param float y: coordinate y in space
	* @param z: coordinate z in space	
*/
void CCamera::LookAt	(float x, float y, float z)
{
	PointsTo.v[X3D] = x; /// updates the lens x
	PointsTo.v[Y3D] = y; /// updates the lens y
	PointsTo.v[Z3D] = z; /// updates the lens z

	LookAt(); // Se llama a la funcion de linea LookAt para actualizar la camara con las coordenadas x,y,z
}

/**
    * @fn CCamera::LookFrom
	* Define function LookFrom for the position of the camera with the coordinates x, y, z
	* @param x: coordinate x in space
	* @param y: coordinate y in space
	* @param z: coordinate z in space	
*/
void CCamera::LookFrom(float x, float y, float z)
{
	Position.v[X3D] = x; /// updates posision x
	Position.v[Y3D] = y; /// updates posision y
	Position.v[Z3D] = z; /// updates posision z

	LookAt(); // Se llama a la funcion de linea LookAt para actualizar la camara con las coordenadas x,y,z
}

/**
    * @fn CCamera::LookFrom
	* Reshape function is defined that changes the shape of the window when you move or resize with the z coordinate
	* @param z: coordinate z in space	
*/
void CCamera::Reshape(float Z)
{
	Position.v[X3D] = Position.v[Y3D] = 0.0f; /// updates position x and y
	Position.v[Z3D] = Z; ///updates position z

	PointsTo.v[X3D] = PointsTo.v[Y3D] = PointsTo.v[Z3D]= 0.0f; /// updates objectives x,y,z 

	Rotation.v[X3D] = Rotation.v[Y3D] = Rotation.v[Z3D] = 0.0f; /// updates the rotation of the coordinates x, y and z

	zoom = zoom_total = 0.0f; /// updates the zoom
	
	LookAt(); 
	///There is no need for upadating the AABB since the camera does not collide to any object in the scene
}

