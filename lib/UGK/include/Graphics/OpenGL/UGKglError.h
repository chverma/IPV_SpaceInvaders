/*	Treatment of the OpenGL errors management

*	Prefix: UGKGLE_

	@author Ramon Molla
	@version 2014-10
*/

#ifndef WINDOWS_LOADED
#define WINDOWS_LOADED
#include <windows.h>
#endif
#include <gl.h>
#include <UGK.h>

#define UGKGLE_ERROR_MNG   /// It defines macros for error handling

/**   
   * DEBUG : Execution and OPENGL error management
*/
#define UGKGLE_DEBUG(command) command;fprintf(stderr,#command " ==> %s\n",gluErrorString(glGetError())); //Macrospara mostrar el error generado en ventana
#define UGKGLE_glResetError() glGetError()	//When a glGetError is called, the internal error variable is set to GL_NO_ERROR

namespace UGK
{
	UGK_API GLenum UGKGLE_glError(); /// Establishing a global function 
}