/***************************************************
Cilindro generalizado
Roberto Vivó, 2012 (v1.0)

Construye cilindros generalizados usando una curva
paramétrica como espina y una sección poligonal
perpendicular. Usa inmediato y retenido para dibujo.

Dependencias:
definitions.h
curve.h
glm.h
glew.h
glut.h
***************************************************/
#ifndef RV_TUBE_H
#define RV_TUBE_H

#include "curve.h"
#include <GL/glut.h>

// TUBO ------------------------------------
// Genera un tubo de seccion poligonal (escala variable) usando una curva como espina
class Tubo {
protected:
    unsigned int n_rodajas;							//Numero de secciones perpendiculares a la espina (incluyendo tapas)
    unsigned int n_facetas;							//Numero de lados de la sección poligonal
    float grosor, longitud;							//Grosor del tubo (radio) y longitud paramétrica
    float sini,sfin;								//Escala de grosor inicial y final
    Curva* espina;									//Espina
    GLuint id, vbo[2];								//Id's de la DL y los VBO's
    void construir();                               //Construye la DL y los VBOs
    float radio(float u)const;                      //Devuelve el radio en u
public:
    Tubo(Curva* q, float g=0.5, float lon=1.0,		//Construye un tubo siguiendo la espina q
         unsigned int nr=10, unsigned int nf=5,
         float escalaini=1.0, float escalafin=1.0)
        :espina(q),grosor(g),longitud(lon),n_rodajas(nr),
         n_facetas(nf),sini(escalaini),sfin(escalafin){construir();}
    void dibujarRetenido() const;					//Genera normales y coordenadas de textura
    void dibujarInmediato() const;					//Genera normales y coordenadas de textura
};

class Tubotorico: public Tubo {
public:
	Tubotorico(float radio=1.0, float radiotubo=0.1,int rodajas= 20, int segmentos= 6):
	  Tubo(new Helice(radio,0.0),radiotubo,1.0,rodajas,segmentos){}
};
class Toro
{
public:
	static const int N=20;					//Numero de segmentos y secciones
	static const int M=20;					//Numero de segmentos y secciones

private:
	typedef struct{
		float x,y,z;
	} vec3;
	float rmedio,rtubo;		//Radios medio e interior
	int id_att;					//Id del atributo tangente en el shader
	vec3 vert[N*M];
	vec3 norm[N*M];
	vec3 tang[N*M];
	vec3 text[N*M];
public:

	Toro(float rm=1.0,float rin=0.4,int sg=20, int sec=20):rmedio(rm),rtubo(rin){
		vec3 vmedio={0.0,0.0,1.0};  //Radio medio
		vec3 vinter={0.0,0.0,1.0};  //Radio del tubo
		//Carga de vertices, normales, tangentes y coordenadas de textura
		int cont=0;
		for(int s=0; s<M; s++){
			vmedio.x=rmedio*sin(2*PI*s/M);
			vmedio.z=rmedio*cos(2*PI*s/M);
			for(int i=0; i<N; i++){
				vinter.x=rtubo*sin(2*PI*s/M)*cos(2*PI*i/N);
				vinter.z=rtubo*cos(2*PI*s/M)*cos(2*PI*i/N);
				vinter.y=rtubo*sin(2*PI*i/N);
				vert[cont].x=vmedio.x+vinter.x;
				vert[cont].y=vmedio.y+vinter.y;
				vert[cont].z=vmedio.z+vinter.z;
				norm[cont].x=vinter.x;
				norm[cont].y=vinter.y;
				norm[cont].z=vinter.z;
				tang[cont].x=vmedio.z;
				tang[cont].y=0.0;
				tang[cont].z=-vmedio.x;
				text[cont].x=(float)s/M;
				text[cont].y=(float)i/N;
				cont++;
			}
		}
	};
	void dibujarInmediato(int att_tangent=0){
		//Formacion de Quads
		int c1,c2;
		for(int s=0; s<M-1; s++){	//Para cada seccion
			glBegin(GL_QUAD_STRIP);
			for(int i=0; i<N; i++){  //Para cada segmento entre secciones
				c1=s*N+i;
				c2=(s+1)*N+i;
				glTexCoord2f(text[c1].x,text[c1].y);
				glNormal3f(norm[c1].x,norm[c1].y,norm[c1].z);
				if(att_tangent>0) glVertexAttrib3f(att_tangent,tang[c1].x,tang[c1].y,tang[c1].z);
				glVertex3f(vert[c1].x,vert[c1].y,vert[c1].z);	//Primer vertice del segmento
				glTexCoord2f(text[c2].x,text[c2].y);
				glNormal3f(norm[c2].x,norm[c2].y,norm[c2].z);
				if(att_tangent>0)glVertexAttrib3f(att_tangent,tang[c2].x,tang[c2].y,tang[c2].z);
				glVertex3f(vert[c2].x,vert[c2].y,vert[c2].z);	//Segundo vertice del segmento
			};
			c1=s*N;		//Ultimo segmento que se repite
			c2=(s+1)*N; 
			glTexCoord2f(text[c1].x,1.0);
			glNormal3f(norm[c1].x,norm[c1].y,norm[c1].z);
			if(att_tangent>0)glVertexAttrib3f(att_tangent,tang[c1].x,tang[c1].y,tang[c1].z);
			glVertex3f(vert[c1].x,vert[c1].y,vert[c1].z);
			glTexCoord2f(text[c2].x,1.0);
			glNormal3f(norm[c2].x,norm[c2].y,norm[c2].z);
			if(att_tangent>0)glVertexAttrib3f(att_tangent,tang[c2].x,tang[c2].y,tang[c2].z);
			glVertex3f(vert[c2].x,vert[c2].y,vert[c2].z);
			glEnd();
		}
		glBegin(GL_QUAD_STRIP);	//Ultima seccion
		for(int i=0; i<N; i++){ //Para cada segmento
			c1=(M-1)*N+i;
			c2=i;
			glTexCoord2f(text[c1].x,text[c1].y);
			glNormal3f(norm[c1].x,norm[c1].y,norm[c1].z);
			if(att_tangent>0)glVertexAttrib3f(att_tangent,tang[c1].x,tang[c1].y,tang[c1].z);
			glVertex3f(vert[c1].x,vert[c1].y,vert[c1].z);
			glTexCoord2f(1.0,text[c2].y);
			glNormal3f(norm[c2].x,norm[c2].y,norm[c2].z);
			if(att_tangent>0)glVertexAttrib3f(att_tangent,tang[c2].x,tang[c2].y,tang[c2].z);
			glVertex3f(vert[c2].x,vert[c2].y,vert[c2].z);
		};
		c1=(M-1)*N;			//Ultimo segmento entre la ultima seccion y la primera
		c2=0;
		glTexCoord2f(text[c1].x,1.0);
		glNormal3f(norm[c1].x,norm[c1].y,norm[c1].z);
		if(att_tangent>0)glVertexAttrib3f(att_tangent,tang[c1].x,tang[c1].y,tang[c1].z);
		glVertex3f(vert[c1].x,vert[c1].y,vert[c1].z);
		glTexCoord2f(1.0,1.0);
		glNormal3f(norm[c2].x,norm[c2].y,norm[c2].z);
		if(att_tangent>0)glVertexAttrib3f(att_tangent,tang[c2].x,tang[c2].y,tang[c2].z);
		glVertex3f(vert[c2].x,vert[c2].y,vert[c2].z);
		glEnd();
	};
};


#endif // RV_TUBE_H
