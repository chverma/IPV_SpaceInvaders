/***************************************************
Formas geométricas desde fichero
Roberto Vivó, 2012 (v1.0)

Construye formas geométricas a partir de fichero.
Usa inmediato y retenido para dibujar.

Dependencias:
loader3ds.h
***************************************************/
#ifndef RV_SHAPEFILE_H
#define RV_SHAPEFILE_H

#include "loader3ds.h"

// FORMA -----------------------------------
// Genera una forma geométrica a partir de fichero (de momento sólo 3ds)
class Forma {
protected:
    GLuint id, vbo[2];
    void construir(char * fichero3ds);
    void cargaFormaDefecto();
    Color material;
public:
    Forma(char * nombrefichero3ds = "", Color mat= Color(0.5f,0.5f,0.5f, 1.f))  //Construye DL y VBOs para figuras cargadas desde fichero 3ds
        :material(mat){ construir(nombrefichero3ds); }
    void dibujarRetenido() const;                                       //Dibuja los VBOs
    void dibujarInmediato() const;                                      //Dibuja la DL
};
#endif // RV_SHAPEFILE_H
