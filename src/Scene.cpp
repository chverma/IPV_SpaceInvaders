/*	Definition of the class Scene

	Prefix: CSC_

	@author Ramon Molla
	@version 2011-10
*/

#include "Scene.h"


void CScene::Init()
{
	Angle.v[UGK::XDIM]	= Angle.v[UGK::YDIM] = AngleStart.v[UGK::XDIM] = 0;
}

CScene::~CScene()
{
}
