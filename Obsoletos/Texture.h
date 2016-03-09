/*Definition of the class Texture
  General class to support all the attribute general to all the textures for 2D and 3D
  Prefix: CT_

	@author Ramon Molla
	@version 2011-10
*/

#ifndef CT_TEXTURES
#define CT_TEXTURES

#ifndef OLECTL_LOADED
#define OLECTL_LOADED
#include <olectl.h>		// Header File For The OLE Controls Library
#endif
#include "gl/GL.h"


class CTexture
{
	//Attributes
public:
	GLuint	Id;		// texture id given by the Open GL kernel

	///Rendering attributes 
	bool	MipMap;									///<MIPMAP available

	//Methods
	BOOL Load        (char* file, USHORT resource_id);
	BOOL LoadJPG_GIF (char* file, USHORT resource_id);
	BOOL LoadBMP     (char* file, USHORT resource_id);
	BOOL LoadTGA     (char* file, USHORT resource_id);
	BOOL LoadPNG     (char* file, USHORT resource_id);

	BOOL Load        (char* file);
	BOOL LoadJPG_GIF (char* file);
	BOOL LoadBMP     (char* file);
	BOOL LoadTGA     (char* file);
	BOOL LoadPNG     (char* file);
};

#endif