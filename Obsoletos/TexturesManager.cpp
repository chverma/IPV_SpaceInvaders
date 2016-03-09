/*Class Texture Manager
  General class to manage all the textures for 2D and 3D
  Prefix: CTM_

	@author Ramon Molla
	@version 2011-08
*/

#include "TexturesManager.h"

CTexturesManager::CTexturesManager()
{
	//Rendering
	MipMap			= true;		// MIPMAP available
	Texture			= NULL;
	MaxTextures		= 0;		//No available textures
}


