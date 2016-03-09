#include <glrv/include/curve.h>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glut.h>

void Curva::dibujarInmediato()                      //Dibujo inmediato sin DL
{
   glBegin(GL_LINE_STRIP);
    for (int i=0; i<101; i++){
		glVertex4fv(glm::value_ptr(q(i*0.01f)));
    };
    glEnd();
    glBegin(GL_LINES);
    for (float u=0; u<=1; u+=0.01){
		Vector pos=q(u).xyz;			//cuidado con el swizzle. Devuelve referencias no objetos del tipo
		Vector incn=n(u)*0.1f;
		Vector incb=b(u)*0.1f;
        glVertex3fv(glm::value_ptr( pos ));
		glVertex3fv(glm::value_ptr( pos + incn ));
        glVertex3fv(glm::value_ptr( pos ));
		glVertex3fv(glm::value_ptr( pos + incb ));
    };
    glEnd();
}

Punto  Helice::q(float u) const                     //Muestreo de la curva en u
{
    Punto p(sin(u*dPI)*radio, u*paso, cos(u*dPI)*radio, 1.0);
    return p;
}
Vector Helice::t(float u) const                     //Vector tangente unitario a la curva en u
{
	Vector tg(cos(u*dPI)*radio*dPI, paso, -sin(u*dPI)*radio*dPI);
	return glm::normalize(tg);
}
Vector Helice::b(float u) const                     //Vector binormal unitario en u
{
	Vector a(-sin(u*dPI),0.0,-cos(u*dPI));
	Vector bn(glm::cross(t(u),a));
	return glm::normalize(bn);
}
Vector Helice::n(float u) const                     //Vector normal unitario en u
{
    Vector a(-sin(u*dPI),0.0,-cos(u*dPI));
	Vector normal(glm::cross(t(u),glm::cross(t(u),a)));
	return glm::normalize(normal);
}


const glm::mat4 CBezier::MB(glm::vec4(-1.f, 3.f,-3.f, 1.f),      //Matriz característica de Bezier
							glm::vec4( 3.f,-6.f, 3.f, 0.f),
							glm::vec4(-3.f, 3.f, 0.f, 0.f),
							glm::vec4( 1.f, 0.f, 0.f, 0.f));

CBezier::CBezier(Punto const control[4])                  //Construye con P0..P3
{
	G=glm::transpose(glm::mat4(control[0],control[1],control[2],control[3]));
	C=MB*G;
}
Punto  CBezier::q(float u) const                    //Q(u) por muestreo directo
{
    float uu=u*u, uuu=uu*u;
	Punto U(uuu,uu,u,1.0);
    return U*C;
}
Vector CBezier::t(float u) const                    //Vector tangente unitario en u
{
	Punto Uprima(3*u*u,2*u,1.0,0.0);
	return glm::normalize(Vector(Uprima*C));
}
Vector CBezier::b(float u) const                    //Vector binormal unitario en u
{
    static Vector bna;								//binormal anterior
	Punto Usegunda(6*u,2.0,0.0,0.0);
    Vector a(Usegunda*C);
	Vector bn= glm::cross(t(u),a);					//binormal actual
	if(u==0) bna=bn;								//inicializa la binormal anterior
    if(bn.length()==0) bn=bna;						//Justo en el punto de inflexión
	if(glm::dot(bn,bna)<0) bn= -bn;					//cambia la binormal de sentido
    bna=bn;											//actualiza la binormal actual
	return glm::normalize(bn);
}
Vector CBezier::n(float u) const                    //Vector normal en u
{
	Vector normal= glm::cross(t(u),b(u));
	return glm::normalize(normal);
}
