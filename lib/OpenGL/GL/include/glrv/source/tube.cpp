#include <iostream>
#include <GL/glew.h>
#include <glrv/include/tube.h>
#include <glm/gtc/type_ptr.hpp>
#include <glrv/include/definitions.h>


float Tubo::radio(float u)const                         //Devuelve el radio en u
{
    return fabs(grosor*(sini*(1-u) + sfin*u));
}
void  Tubo::construir()                                 //Construye DL y VBOs
{
    //El tubo se construye como una malla abierta donde los últimos vértices tienen las mismas
    //coordenadas de los primeros como si fuera una cremallera que cierra el tubo.
    //Lo hago así para que se pueda aplicar la textura (s,t) sobre la superficie desplegada (u,v)
    Vector muestra, p;							//Puntos sobre la espina y el tubo
    Vector normal, binormal, normal_p;			//Vectores normal y binormal de la espina y normal al tubo en p
    float u;									//Parámetro de la curva
    int nvert= (n_facetas+1)*n_rodajas;			//Número total de vértices del tubo
    int nindi= 6*n_facetas*(n_rodajas-1);		//Número total de índices para formar triángulos
    int n=0;									//Indice de inserción
    Punto* vert= new Punto[nvert];				//Array de coordenadas (w no se usa)
    Vector* norm= new Vector[nvert];			//Array de normales
	Vector* tang= new Vector[nvert];			//Array de tangentes
    GLuint* indi= new GLuint[nindi];			//Array de índices a vértices
    Punto* ctex= new Punto[nvert];				//Array de coordenadas de textura (en realidad solo se usan x e y)

    //Vértices
    for(unsigned int i=0; i<n_rodajas; i++){	//Sección a sección
        u= i*longitud/(n_rodajas-1);			//1. Calculamos el valor del parámetro
        muestra= Vector(espina->q(u));			//2. Obtenemos la muestra en la espina y sus vectores
        normal= espina->n(u);
        binormal=espina->b(u);
        float r= radio(u);
		Vector tangente= espina->t(u);
        for(unsigned int j=0; j<=n_facetas; j++){//3. Calculamos los vértices (coord,normal,ctex)
            //Vertice= q(u)+n(u)*grosor*cos(delta)+b(u)*grosor*sin(delta)
            normal_p= normal*cos(j*dPI/n_facetas)+binormal*sin(j*dPI/n_facetas);
            p= muestra + normal_p*r;			//r: radio variable del tubo
            vert[n]= Punto(p,1.0);
            norm[n]= normal_p;
			tang[n]= tangente;
            ctex[n]= Punto(2*u, (float)j/n_facetas,0.f,0.f); //Cuidado con las ctex de los triángulos altos
            n++;
        }
    };

    //Indices
    //Para cada rodaja i, los vertices: j=0,1,..,nf
    //Primer vertice de la rodaja i: i*(nf+1)
    //Triangulos entre i e i+1:
    // 0i,0(i+1),1(i+1); 0i,1(i+1),1i :faceta 0
    // 1i,1(i+1),2(i+1); 1i,2(i+1),2i :faceta 1
    // ....

    n=0;
    for(unsigned int i=0; i<n_rodajas-1; i++){			//Para cada sección menos la última
        for(unsigned int j=0; j<n_facetas; j++){		//Para cada vértice menos el último
            //Faceta j entre i e i+1
            indi[n]=(n_facetas+1)*i+j; n++;				//Primer triángulo (CCW)
            indi[n]=(n_facetas+1)*(i+1)+j; n++;
            indi[n]=(n_facetas+1)*(i+1)+(j+1); n++;
            indi[n]=(n_facetas+1)*i+j; n++;				//Segundo triangulo (CCW)
            indi[n]=(n_facetas+1)*(i+1)+(j+1); n++;
            indi[n]=(n_facetas+1)*i+(j+1); n++;
        };
    };

    //Construir los VBOs
    glGenBuffers(2,vbo);
    glBindBuffer(GL_ARRAY_BUFFER,vbo[0]);

	GLsizeiptr  szvec= sizeof(Vector)*nvert;
	GLsizeiptr  szpto= sizeof(Punto)*nvert;

    glBufferData(GL_ARRAY_BUFFER,2*szvec+2*szpto,NULL,GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER,0,szvec,norm);
    glBufferSubData(GL_ARRAY_BUFFER,szvec,szpto,ctex);
    glBufferSubData(GL_ARRAY_BUFFER,szvec+szpto,szvec,tang);
	glBufferSubData(GL_ARRAY_BUFFER,2*szvec+szpto,szpto,vert);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vbo[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(GLuint)*nindi,indi,GL_STATIC_DRAW);

    //Construye la Display List
    id= glGenLists(1);
    glNewList(id, GL_COMPILE);
    glBegin(GL_TRIANGLES);
    for(int i=0; i<nindi; i++){
		glVertexAttrib3fv(glrv::TGTE,glm::value_ptr(tang[indi[i]]));
        glTexCoord2f(ctex[indi[i]].x,ctex[indi[i]].y);
		glNormal3fv(glm::value_ptr(norm[indi[i]]));
		glVertex4fv(glm::value_ptr(vert[indi[i]]));
    };
    glEnd();
    glEndList();

    delete []norm;
    delete []indi;
    delete []vert;
    delete []ctex;
	delete []tang;
}
void  Tubo::dibujarRetenido() const                     //Dibuja los VBOs
{
	GLsizeiptr  szvec= sizeof(Vector)*(n_facetas+1)*n_rodajas;
	GLsizeiptr  szpto= sizeof(Punto)*(n_facetas+1)*n_rodajas;

    //Ver Cubo::dibujarRetenido. Es lo mismo añadiendo coordenadas de textura
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_INDEX_ARRAY);
	glEnableVertexAttribArray(glrv::TGTE); 

    glBindBuffer(GL_ARRAY_BUFFER,vbo[0]);
    glNormalPointer(GL_FLOAT,0,0);
    glTexCoordPointer(2,GL_FLOAT,4*sizeof(GLfloat),(GLvoid *) szvec);
 	//glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0, (GLvoid *)(szvec+szpto)); 
 	glVertexAttribPointer(glrv::TGTE,3,GL_FLOAT,GL_FALSE,0, (GLvoid *)(szvec+szpto)); 
    glVertexPointer(4,GL_FLOAT,0,(GLvoid *) (2*szvec+szpto));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vbo[1]);
    glIndexPointer(GL_UNSIGNED_INT,0,0);
    glDrawElements(GL_TRIANGLES,6*n_facetas*(n_rodajas-1),GL_UNSIGNED_INT,0);

	glDisableVertexAttribArray(glrv::TGTE); 
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_INDEX_ARRAY);
}
void  Tubo::dibujarInmediato() const                    //Dibuja la DL
{
    glCallList(id);
}
