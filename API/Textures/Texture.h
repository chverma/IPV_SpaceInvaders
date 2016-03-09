/**	General Texture class that manages general functions of any given image

*	Prefix CT_: Class Texture

*	@Author: Vicente Broseta Toribio: 2013-06
*	@author Ramon Molla
*	@version 01-2015
*/

#ifndef TEXTUREV_H
#define TEXTUREV_H

#include <sstream>
#include <string>
#include "glut.h"

#include <windows.h>	// Header File For Windows
#include <gl.h>			// Header File For The OpenGL32 Library
#include <glaux.h>		// Header File For The Glaux Library

using namespace std;

#define CT_NOT_INDEXED -1
#define CT_ERROR -1
#define CT_NO_ERROR 0

typedef struct {
	GLint tmode, // Modo de aplicación de la textura
		minMode, // Modo de minimización de textura
		maxMode, // Modo de maximización de textura
		wrap_s,  // Modos de extensión de la textura
		wrap_t;
	unsigned int width, height; // Tamaño de la textura
	GLuint gl_id;
} Ttexture;

class CTexture
{
private:
	int LoadBMP(char *name);						// Loads a bitmap file
	int LoadTGA(char *name);						// Loads a targa file
	//int Read_imageBMP (char * filename, unsigned int *width, unsigned int *height, unsigned char **buffer);

	char * FilePath;
	char * FileName;

	char		*GetPathFile (char *filename);
	inline char *GetPathFile () {return GetPathFile(FileName);}

public:
	Ttexture Texture;
	unsigned int IndexInList;
	bool Loaded;

	CTexture();
	~CTexture(){}

	void	SetTexture(void);
	void	FreeTexture(void);

	int			Load(char *filename, Ttexture *dest);
	int			Load(char *filename);
	inline int	Load(void)	{return Load(FileName,&Texture);}

	void SetFileName(char*);
	void SetFilePath(char*);

	inline char *GetFileName(){return FileName;};
	inline char *GetFilePath(){return FilePath;};
};

#endif // TEXTUREV.H