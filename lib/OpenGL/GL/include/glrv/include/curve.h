/***************************************************
Curva paramétrica
Roberto Vivó, 2012 (v1.0)

Construye curvas paramétricas como las cúbicas de Bezier
o las Helicoides. Usa inmediato para dibujo.

Dependencias:
definitions.h
***************************************************/

#ifndef RV_CURVA_H
#define RV_CURVA_H

#include "definitions.h"

// CURVA PARAMETRICA -------------------------
// Clase madre de segmentos curvos. Por defecto es un segmento recto de longitud 1
class Curva {
protected:
public:
    Curva(){}
    virtual Punto  q(float u) const {return Punto(u,0.0,0.0,1.0);}
    virtual Vector t(float u) const {return Vector(1.0,0.0,0.0);}
    virtual Vector b(float u) const {return Vector(0.0,0.0,1.0);}
    virtual Vector n(float u) const {return Vector(0.0,1.0,0.0);}
    void dibujarInmediato();
};


// HELICE: CURVA ----------------------------
// Genera una hélice con eje y, de radio r y paso p
class Helice: public Curva {
protected:
    float radio, paso;								//Una vuelta (paso) es una unidad en parámetro
public:
    Helice(float r=1.0, float p=1.0): radio(r),paso(p){}
    Punto  q(float u) const; 						//Muestreo de la curva en u
    Vector t(float u) const;						//Vector tangente unitario a la curva en u
    Vector b(float u) const;						//Vector binormal unitario en u
    Vector n(float u) const;						//Vector normal unitario en u

};


// CBEZIER: CURVA --------------------------
class CBezier: public Curva {
protected:
	glm::mat4 G;
	glm::mat4 C;
	static const glm::mat4 MB;
public:
    CBezier(Punto const control[4]);				//Constructor por puntos de control P0..P3
    Punto  q(float u) const;                        //Muestreo de la curva Q(u)
    Vector t(float u) const;                        //Tangente unitaria Q'(u)
    Vector b(float u) const;                        //Vector binormal Q'(u)^Q"(u) unitario
    Vector n(float u) const;                        //Vector normal en u unitario
};

#endif // RV_CURVA_H
