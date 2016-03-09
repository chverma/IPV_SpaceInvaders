/***************************************************
Cubo RGB
Roberto Vivó, 2012 (v1.0)

Construye cubos RGB de cualquier lado centrados en el
origen. Usa inmediato y retenido para dibujo.

Dependencias:
definitions.h, glew.h|gll.h
***************************************************/
#ifndef RV_COLORCUBE_H
#define RV_COLORCUBE_H

#include <iostream>
#include "definitions.h"

using namespace std;


// CUBO COLOREADO ---------------------------
class Cubo {
protected:
        static const unsigned int triangulos[36];		//Vector de indices. Constantes de clase
        float semilado;
        unsigned int id,vbo[2];							//Id's de la Display List y los VBO's
        Punto coord[8];									//Lista de vértices
        Color colores[8];                               //Lista de colores asociados a vértices
        void construir();
public:
        Cubo(float l=1.0){								//Construye un cubo de lado l centrado en (0,0,0)
                semilado=l/2;
                construir();
        }
        void dibujarInmediato() const;					//Usa la Display List
        void dibujarRetenido() const;					//Usa VBO's en retenido
};

// -------------------------------------------

#endif // RV_COLORCUBE_H
