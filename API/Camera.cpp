/*/////////////////////////////////////////////////////////////////////////////////////
//
// Camera class declarations
//
	Prefix: CC_
	@author Ramon Molla
	@version 2007-09-11
*/

#include <UGKPhysics.h>
#include "Camera.h"
//#include <gl\glError.h>

#define CC_OGL
//#define CC_DX

#ifdef	CC_OGL
#elif	CC_DX
#endif

using namespace UGK;

/**
   * @fn CCamera:: Init
   * Constructor for the object CCamera
*/
void CCamera::Init(void)
{
	RenderMode = CHAR_NO_RENDER;
	PhysicMode = UGKPHY_PHANTOM;		//No render, No touchable. No AABB initialization is required

	zoom = zoom_total= zoom_total_bonus	= 0; //Se Inician las variables en zoom, zoom_total, zoom_total_bonus en 0

}


/**
    * Define function LookAt line that starts the camera position and objectives, in addition to the top of the camera
*/
inline void CCamera::SetLook()
{
	#ifdef	CC_OGL
	gluLookAt(	Position.v[XDIM], Position.v[YDIM], Position.v[ZDIM],	/// From the position of the camera
				PointsTo.v[XDIM], PointsTo.v[YDIM], PointsTo.v[ZDIM],	/// Objetive
				Up.v[XDIM], Up.v[YDIM], Up.v[ZDIM]);					/// Height
	#elif	CC_DX
	#endif
}


/**
    * Define function LookAt for objectives with coordinates x, y, z
	* @param x: type int coordinate x in space
	* @param float y: coordinate y in space
	* @param z: coordinate z in space	
*/
/*inline*/ void CCamera::LookAt	(float x, float y, float z)
{
	PointsTo.Set(x, y, z);

	SetLook(); // Se llama a la funcion de linea LookAt para actualizar la camara con las coordenadas x,y,z
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
	Position.Set(x, y, z);

	SetLook(); // Se llama a la funcion de linea LookAt para actualizar la camara con las coordenadas x,y,z
}

/**
    * @fn CCamera::LookFrom
	* Reshape function is defined that changes the shape of the window when you move or resize with the z coordinate
	* @param z: coordinate z in space	
*/
void CCamera::Reshape(float Z)
{
	Position.Set(0.0f, 0.0f, Z); // updates positions x,y,z
	PointsTo.Reset(); /// updates objectives x,y,z
	Rotation.Reset(); /// updates the rotation of the coordinates

	zoom = zoom_total = 0.0f; /// updates the zoom
	
	Up.v[YDIM]=1.0;
	SetLook(); 
	///There is no need for upadating the AABB since the camera does not collide to any object in the scene
}
