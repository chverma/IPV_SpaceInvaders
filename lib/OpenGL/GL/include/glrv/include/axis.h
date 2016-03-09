/***************************************************
Ejes
Roberto Vivó, 2012 (v1.0)

Construye unos ejes en el origen de longitud 1 con una
esfera opcional en el origen. Usa inmediato para dibujar.

Dependencias:
glut.h
***************************************************/
#ifndef RV_AXIS_H
#define RV_AXIS_H

#include <GL/glut.h>

// EJES -------------------------------------
class Ejes{
protected:
    GLuint id;										//Id de la Display List
    bool esfera;									//true si se quiere una esfera en el origen
    void construir();								//Construye la Display List
public:
    Ejes(bool esferaEnOrigen=true):esfera(esferaEnOrigen){construir();}
    void dibujarInmediato(){ glCallList(id); }		//Dibuja en inmediato llamando a la DL
};
#endif // RV_AXIS_H
