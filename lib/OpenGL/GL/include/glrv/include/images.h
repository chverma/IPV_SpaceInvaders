/***************************************************
Imagenes
Roberto Vivó, 2012 (v1.0)

Carga de imágenes BMP desde fichero.

Dependencias:
glut.h
***************************************************/
#ifndef RV_IMAGES
#define RV_IMAGES

#include <GL/GLUT.h>

//Lee un BMP y lo carga en tex. Si va bien devuelve 1.
//Devuelve dimensiones del bitmap y formato(GL_RGB)
int leerFicheroBMP(const char* nombre,unsigned int &numCols, 
				   unsigned int &numFilas,GLenum &formato,
				   GLubyte* &tex );


//Lee un BMP de 24 y 8 bpp
int read_image (char * filename, unsigned int *width, unsigned int *height, unsigned char **buffer);
#endif