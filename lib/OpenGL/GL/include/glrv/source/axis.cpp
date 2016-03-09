#include <glrv/include/axis.h>
void Ejes::construir()
{
    //Construye la Display List compilada de una flecha vertical
    id = glGenLists(2);					//Obtiene un id's para las DLs
    glNewList(id+1,GL_COMPILE);			//Abre la DL
        //Brazo de la flecha
        glBegin(GL_LINES);
            glVertex3f(0,0,0);
            glVertex3f(0,1,0);
        glEnd();
        //Punta de la flecha
        glPushMatrix();
        glTranslatef(0,1,0);
        glRotatef(-90,1,0,0);
        glTranslatef(0.0,0.0,-1/10.0);
        GLUquadric* punta= gluNewQuadric();
        gluCylinder(punta,1/50.0,0,1/10.0,10,1);
        glPopMatrix();
    glEndList();						//Cierra la DL

    //Ahora construye los ejes
    glNewList(id,GL_COMPILE);
        //Eje X en rojo
        glColor3f(1,0,0);
        glPushMatrix();
            glRotatef(-90,0,0,1);
            glCallList(id+1);
        glPopMatrix();
        //Eje Y en verde
        glColor3f(0,1,0);
        glPushMatrix();
            glCallList(id+1);
        glPopMatrix();
        //Eje Z en azul
        glColor3f(0,0,1);
        glPushMatrix();
            glRotatef(90,1,0,0);
            glCallList(id+1);
        glPopMatrix();
        //Esferita en el origen
        if(esfera){
            glColor3f(0.5,0.5,0.5);
            GLUquadric* bolita= gluNewQuadric();
            gluSphere(bolita,0.05,8,8);
        };
    glEndList();
}
