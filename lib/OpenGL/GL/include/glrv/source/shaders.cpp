#include <iostream>									 
#include <fstream>									 
#include <sstream>									 
#include <cmath>									 
#include <glrv/include/shaders.h>

using namespace std;								//Espacio de nombres de la Standard Library

GLuint buildShader(const char* shaderFile, GLenum stype)
//Construye el shader de vertices con el codigo que hay en el fichero y devuelve su id
{
	ifstream file;								//Fichero de codigo fuente GLSL
	ostringstream contents;

	fstream logfile;							//Fichero de log para errores
	logfile.open("error.log",fstream::out);

	//Abrir fichero y leer
	file.open(shaderFile,fstream::in);
	if(file.fail()){
		logfile << "buildShader:: Error abriendo el fichero: " << shaderFile << endl;
		exit( EXIT_FAILURE );
	}
	contents<< file.rdbuf()<<ends;
	file.close();
	string strcode(contents.str());

	//Código fuente
	GLchar* sourcecode;
	sourcecode = (char *)strcode.c_str();//Convierte la tira a C -con ultimo char \0 -
	if ( sourcecode == NULL ) {
		logfile << "buildShader:: Codigo fuente nulo " << shaderFile << endl;
		logfile.close();
		exit( EXIT_FAILURE );
	}
	logfile << "SHADER CODE: ----------------------" << endl << sourcecode << endl;
	logfile << "buildShader:: Shader " << shaderFile << " cargado." << endl;

	//Crea el shader
	GLuint shaderid = glCreateShader( stype );
	if(!shaderid){
		logfile << "buildShader:: Falló creando shader: " << shaderFile << endl;
		logfile.close();
		exit( EXIT_FAILURE );
	}
	glShaderSource( shaderid, 1, (const GLchar**) &sourcecode, NULL );

	//Compila
	glCompileShader( shaderid );

	logfile << "********Shader compilando no: " << shaderid << endl;

	GLint  compiled;
	glGetShaderiv( shaderid, GL_COMPILE_STATUS, &compiled );
	if ( !compiled ) {
		logfile << "buildShader:: Falló la compilación: " << shaderFile << endl;
		GLint  logSize;
		glGetShaderiv( shaderid, GL_INFO_LOG_LENGTH, &logSize );
		char* logMsg = new char[logSize];
		glGetShaderInfoLog( shaderid, logSize, NULL, logMsg );
		logfile << "buildShader:: ERRORES: " << endl << logMsg << endl;
		cerr << "buildShader:: ERRORES: " << endl << logMsg << endl;
		delete [] logMsg;
		logfile.close();
		exit( EXIT_FAILURE );
	}
	logfile << "********Shader comprobado no: " << shaderid << endl;

	logfile << "buildShader:: Shader " << shaderFile << " compilado." << endl;
	logfile.close();
	return shaderid;
}

GLuint buildProgramShader(const char* shaderFile, GLenum stype)
//Construye un programa de un único shader
{
	fstream logfile;							//Fichero de log para errores
	logfile.open("error.log",fstream::out);

	//Obtiene el id del programa
    GLuint program = glCreateProgram();
    
	//Compila
	GLuint sh= buildShader(shaderFile,stype);

	//Enlaza
	glAttachShader(program, sh);

    //Montaje del programa y comprobación de errores
    glLinkProgram(program);

    GLint  linked;
    glGetProgramiv( program, GL_LINK_STATUS, &linked );
    if ( !linked ) {
		logfile << "buildProgramShader:: Falló el montaje del programa" << endl;
		GLint  logSize;
		glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logSize);
		char* logMsg = new char[logSize];
		glGetProgramInfoLog( program, logSize, NULL, logMsg );
		logfile << "buildProgramShader::ERRORES DE MONTAJE" << endl << logMsg << endl;
		cerr << "buildProgramShader::ERRORES DE MONTAJE" << endl << logMsg << endl;
		delete [] logMsg;
		logfile.close();
		exit( EXIT_FAILURE );
    }
	logfile.close();
    return program;
}
GLuint buildProgramShaders(const char* vShaderFile, const char* fShaderFile)
//Construye un programa GLSL con los shaders de vertice y fragmento en ficheros y devuelve el id
//Basado en codigo de E.Angel
{
	fstream logfile;							//Fichero de log para errores
	logfile.open("error.log",fstream::out);

	//Obtiene el id del programa
    GLuint program = glCreateProgram();
    
	//Para cada shader
	GLuint vertexsh= buildShader(vShaderFile,GL_VERTEX_SHADER);
	GLuint fragmentsh= buildShader(fShaderFile,GL_FRAGMENT_SHADER);

	//Enlaza los shaders
	glAttachShader(program, vertexsh);
	glAttachShader(program, fragmentsh);

    //Montaje del programa y comprobación de errores
    glLinkProgram(program);

    GLint  linked;
    glGetProgramiv( program, GL_LINK_STATUS, &linked );
    if ( !linked ) {
		logfile << "buildProgramShader:: Falló el montaje del programa" << endl;
		GLint  logSize;
		glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logSize);
		char* logMsg = new char[logSize];
		glGetProgramInfoLog( program, logSize, NULL, logMsg );
		logfile << "buildProgramShader::ERRORES DE MONTAJE" << endl << logMsg << endl;
		cerr << "buildProgramShader::ERRORES DE MONTAJE" << endl << logMsg << endl;
		delete [] logMsg;
		logfile.close();
		exit( EXIT_FAILURE );
    }
	logfile.close();
    return program;
}
