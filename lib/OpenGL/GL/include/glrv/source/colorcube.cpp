#include <glrv/include/colorcube.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
const unsigned int Cubo::triangulos[36]=			//Lista de polígonos a vértices
{0,3,7, 7,3,4, 0,1,2, 0,2,3, 4,3,2, 4,2,5,
 6,7,4, 6,4,5, 1,5,2, 1,6,5, 7,6,1, 7,1,0};
void Cubo::construir()
{
        //Colores del espacio RGB
		colores[0]= Color(1,0,0,1); coord[0]=Punto( semilado,-semilado, semilado, 1.0);
        colores[1]= Color(1,0,1,1); coord[1]=Punto( semilado,-semilado,-semilado, 1.0);
        colores[2]= Color(1,1,1,1); coord[2]=Punto( semilado, semilado,-semilado, 1.0);
        colores[3]= Color(1,1,0,1); coord[3]=Punto( semilado, semilado, semilado, 1.0);
        colores[4]= Color(0,1,0,1); coord[4]=Punto(-semilado, semilado, semilado, 1.0);
        colores[5]= Color(0,1,1,1); coord[5]=Punto(-semilado, semilado,-semilado, 1.0);
        colores[6]= Color(0,0,1,1); coord[6]=Punto(-semilado,-semilado,-semilado, 1.0);
        colores[7]= Color(0,0,0,1); coord[7]=Punto(-semilado,-semilado, semilado, 1.0);
        //Construir la DISPLAY LIST
        id=glGenLists(1);
        glNewList(id,GL_COMPILE);
                glBegin(GL_TRIANGLES);
                        for(int i=0; i<36; i++){
							glColor4fv(glm::value_ptr(colores[triangulos[i]]));
							glVertex4fv(glm::value_ptr(coord[triangulos[i]]));
                        };
                glEnd();
        glEndList();

        //Construir los VBOs
        glGenBuffers(2,vbo);  //Necesitamos 2 vertex-buffers, uno para vértices y otro para indices
        glBindBuffer(GL_ARRAY_BUFFER,vbo[0]);  //Seleccionamos el de vértices
        glBufferData(GL_ARRAY_BUFFER,sizeof(colores)+sizeof(coord),NULL,GL_STATIC_DRAW);  //Reservamos espacio para la info asociada al vértice
        glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(colores),colores);  //Rellenamos el primer trozo con info de colores
        glBufferSubData(GL_ARRAY_BUFFER,sizeof(colores),sizeof(coord),coord);  //El segundo trozo es para coordenadas
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vbo[1]);  //El siguiente buffer es para los índices
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(triangulos),triangulos,GL_STATIC_DRAW); //Cargamos directamente (reserva+relleno)

}
void Cubo::dibujarInmediato() const
{
        //Simplemente se ejecuta la lista
        glCallList(id);
}
void Cubo::dibujarRetenido() const
{

        //Habilita los arrays de coordenadas, colores e índices
        glEnableClientState(GL_COLOR_ARRAY);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_INDEX_ARRAY);

        //Engancha con el VBO que contiene la info asociada a los vértices y apunta la direccion
        glBindBuffer(GL_ARRAY_BUFFER,vbo[0]);
        glColorPointer(4,GL_FLOAT,0,0);
        glVertexPointer(4,GL_FLOAT,0,(GLvoid *) sizeof(colores));

        //Engancha con el VBO de indices y apunta la direccion
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vbo[1]);
        glIndexPointer(GL_UNSIGNED_INT,0,0);

        //Dibuja lo que tiene apuntado como triángulos
        glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_INT,0);

        //Desengancha los punteros de los buffers
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_INDEX_ARRAY);

}
