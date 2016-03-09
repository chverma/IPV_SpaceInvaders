/***************************************************
Funciones de utilidad de shaderss para OpenGL 2.1 
Roberto Vivó, 2012 (v1.0)

Dependencias: 
+GLEW.h,GLUT.h
+glew32.lib,GLUT32.lib,glew32.dll,GLUT32.dll
***************************************************/

#ifndef RV_SHADERS
#define RV_SHADERS

#include <GL/GLEW.h>								//Librería de extensiones a OpenGL 2.1
#include <GL/GLUT.h>								//Librería de utilidades estandar sobre OpenGL

GLuint buildShader(const char* shaderFile, GLenum stype);
/* Carga código GLSL desde un fichero de texto, lo compila y devuelve el
identificador. Escribe por consola y en el fichero "error.log" para debug.
Parámetros:
	-shaderFile: Nombre de fichero fuente GLSL
	-stype: GL_VERTEX_SHADER | GL_FRAGMENT_SHADER según el tipo de shader
*/

GLuint buildProgramShader(const char* shaderFile, GLenum stype);
/* Construye un programa listo para usar si todo va bien con un sólo shader de
vértice o fragmento en código fuente GLSL. Devuelve el identificador si no
se producen errores. Usa el fichero "error.log" para escribir errores.
Parámetros:
	-shaderFile: Nombre del fichero fuente GLSL del shader
	-stype: GL_VERTEX_SHADER | GL_FRAGMENT_SHADER según el tipo de shader
*/

GLuint buildProgramShaders(const char* vShaderFile, const char* fShaderFile);
/* Construye un programa listo para usar si todo va bien con los shaders de
vértice y fragmento en código fuente GLSL. Devuelve el identificador si no
se producen errores. Usa el fichero "error.log" para escribir errores.
Parámetros:
	-vShaderFile: Nombre del fichero fuente GLSL del shader de vértices
	-fShaderFile: Nombre del fichero fuente GLSL del shader de fragmentos
*/

#endif