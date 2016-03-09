/*Definition of the class Texture Manager
  General class to manage all the textures for 2D and 3D
  Prefix: CTM_

	@author Ramon Molla
	@version 2011-10
*/

#ifndef CTM_TEXTURES
#define CTM_TEXTURES

#include "Texture.h"

/**	
	@class CTexturesManager
	* Manages all the textures used in the application

	@author Ramon Molla
	@version 2011-10
*/

class CTexturesManager
{
	///Rendering attributes 
	bool			MipMap;				///<MIPMAP available
	unsigned int	MaxTextures;		///<Amount of textures to manage
public:

	///Constructor
	CTexturesManager();

	CTexture *Texture;
	unsigned int LogIndex;

	void Load        (char* file, USHORT resource_id);
	
	inline void SetTexturesAmount	(unsigned int Amount)	{MaxTextures = Amount; if (NULL != Texture) delete[] Texture; Texture = new CTexture[Amount];};
	inline void SetMipMap			(bool Activate)			{MipMap = Activate;for(unsigned int i=0;i<MaxTextures;i++) Texture[i].MipMap = MipMap;};
	inline bool GetMipMap			()						{return MipMap;};
};

#endif