#include <iostream>
#include <fstream>
#include <algorithm>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glrv/include/definitions.h>
#include <glrv/include/shapefile.h>

using namespace std;

void Forma::cargaFormaDefecto()         //Construye un triángulo por defecto
{
    int numtriangulos= 1;
    static const Punto coord[]={Punto(-1,0,0,1),Punto(1,0,0,1),Punto(0,1,0,1)};
    static const Vector norm[]={Vector(0,0,1),Vector(0,0,1),Vector(0,0,1)};
    static const Color col[]={Color(1,0,0,1),Color(0,1,0,1),Color(0,0,1,1)};
    static const Punto tex[]={Punto(0,0,0,0),Punto(1,0,0,0),Punto(0.5f,1,0,0)};
    static const int indices[]={0,1,2};
    //Construir la DISPLAY LIST
    id=glGenLists(1);
    glNewList(id,GL_COMPILE);
        glBegin(GL_TRIANGLES);
        for(int i=0; i<3*numtriangulos; i++){
			glNormal3fv(glm::value_ptr(norm[indices[i]]));
			glColor4fv(glm::value_ptr(col[indices[i]]));
            glTexCoord2f(tex[indices[i]].x,tex[indices[i]].y);
			glVertex4fv(glm::value_ptr(coord[indices[i]]));
        };
        glEnd();
    glEndList();

    //Construir los VBOs
    glGenBuffers(2,vbo);  //Necesitamos 2 vertex-buffers, uno para vértices y otro para indices
    glBindBuffer(GL_ARRAY_BUFFER,vbo[0]);  //Seleccionamos el de vértices
    glBufferData(GL_ARRAY_BUFFER,sizeof(col)+sizeof(coord)+sizeof(norm)+sizeof(tex),NULL,GL_STATIC_DRAW);  //Reservamos espacio para la info asociada al vértice
    glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(col),col);  //Rellenamos el primer trozo con info de colores
    glBufferSubData(GL_ARRAY_BUFFER,sizeof(col),sizeof(norm),norm);  //El segundo trozo es para normales
    glBufferSubData(GL_ARRAY_BUFFER,sizeof(col)+sizeof(norm),sizeof(tex),tex);  //El tercero para coordenadas de textura
    glBufferSubData(GL_ARRAY_BUFFER,sizeof(col)+sizeof(norm)+sizeof(tex),sizeof(coord),coord);  //El cuarto y ultimo para las coordenadas

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vbo[1]);  //El siguiente buffer es para los índices
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW); //Cargamos directamente (reserva+relleno)
}
void Forma::construir(char *fichero3ds) //Construye una forma desde fichero si no es nulo y existe. Si no construye un cubo.
{
    Model_3DS modelo;

    ifstream file;
    file.open(fichero3ds,ios::in);
    if( !file ){
        cout << "Forma::Error al abrir el fichero" << endl;
        cargaFormaDefecto();
        return;
    }
    else file.close();

    modelo.Load(fichero3ds);					//Esto debería cargar el modelo (posiblemente con varios objetos)

    float xmin,xmax,ymin,ymax,zmin,zmax;
    modelo.Extent( xmin, xmax, ymin, ymax, zmin, zmax);
    float lado= max(xmax-xmin,ymax-ymin);
    lado= max(lado,zmax-zmin);

    //Construir la DISPLAY LIST
    id=glGenLists(1);
    glNewList(id,GL_COMPILE);
        glPushMatrix();
        glScalef(2/lado,2/lado,2/lado);
        glTranslatef(-(xmax+xmin)/2,-(ymax+ymin)/2,-(zmax+zmin)/2);
        for(int k=0; k<modelo.numObjects; k++){
            glBegin(GL_TRIANGLES);
            for(int i=0; i<modelo.Objects[k].numFaces; i++){	//realmente es el numero de indices
                int j= modelo.Objects[k].Faces[i];				//indice del vertice
                glNormal3f(	modelo.Objects[k].Normals[j*3],
                            modelo.Objects[k].Normals[j*3+1],
                            modelo.Objects[k].Normals[j*3+2]);
				glColor4fv(glm::value_ptr(material));
                //float a=modelo.Objects[k].TexCoords[j*3], b=modelo.Objects[k].TexCoords[j*3+1];
                glTexCoord2f(	modelo.Objects[k].TexCoords[j*3],
                                modelo.Objects[k].TexCoords[j*3+1]);
                glVertex3f(	modelo.Objects[k].Vertexes[j*3],
                            modelo.Objects[k].Vertexes[j*3+1],
                            modelo.Objects[k].Vertexes[j*3+2]);
            };
            glEnd();
        };
        glPopMatrix();
    glEndList();

    /*
    //Construir los VBOs
    glGenBuffers(2,vbo);  //Necesitamos 2 vertex-buffers, uno para vértices y otro para indices
    glBindBuffer(GL_ARRAY_BUFFER,vbo[0]);  //Seleccionamos el de vértices
    glBufferData(GL_ARRAY_BUFFER,sizeof(modelo.Objects[0].Normals)+sizeof(modelo.Objects[0].TexCoords)+sizeof(modelo.Objects[0].Vertexes),NULL,GL_STATIC_DRAW);  //Reservamos espacio para la info asociada al vértice
    glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(modelo.Objects[0].Normals),modelo.Objects[0].Normals);  //El segundo trozo es para normales
    glBufferSubData(GL_ARRAY_BUFFER,sizeof(modelo.Objects[0].Normals),sizeof(modelo.Objects[0].TexCoords),modelo.Objects[0].TexCoords);  //El tercero para coordenadas de textura
    glBufferSubData(GL_ARRAY_BUFFER,sizeof(modelo.Objects[0].Normals)+sizeof(modelo.Objects[0].TexCoords),sizeof(modelo.Objects[0].Vertexes),modelo.Objects[0].Vertexes);  //El cuarto y ultimo para las coordenadas

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vbo[1]);  //El siguiente buffer es para los índices
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(modelo.Objects[0].Faces),modelo.Objects[0].Faces,GL_STATIC_DRAW); //Cargamos directamente (reserva+relleno)
    */

}
void Forma::dibujarInmediato() const
{
    glCallList(id);
}
void Forma::dibujarRetenido() const
{
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_INDEX_ARRAY);

    /*
    glBindBuffer(GL_ARRAY_BUFFER,vbo[0]);
    glNormalPointer(GL_FLOAT,3*sizeof(GLfloat),0);
    glTexCoordPointer(2,GL_FLOAT,4*sizeof(GLfloat),(GLvoid *) (sizeof(Vector)*(n_facetas+1)*n_rodajas));
    glVertexPointer(4,GL_FLOAT,0,(GLvoid *) (sizeof(Vector)*(n_facetas+1)*n_rodajas*2));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vbo[1]);
    glIndexPointer(GL_UNSIGNED_INT,0,0);
    glDrawElements(GL_TRIANGLES,6*n_facetas*(n_rodajas-1),GL_UNSIGNED_INT,0);
    */

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_INDEX_ARRAY);
}
