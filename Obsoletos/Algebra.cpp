/**	Common algebra types

	Prefix: ALG_

	@author Ramon Molla
	@version 2011-07
*/

#include "Algebra.h"
#include "math.h"

float ALG_ModelView[ALG_MATRIX_SIZE];

float ALG_NullMatrix[ALG_MATRIX_SIZE] = 
{
	0.0f, 0.0f, 0.0f, 0.0f, 
	0.0f, 0.0f, 0.0f, 0.0f, 
	0.0f, 0.0f, 0.0f, 0.0f, 
	0.0f, 0.0f, 0.0f, 0.0f
};

/**
*	\fn void ALG_NeutralModelView(Matrix M)

*	Generates an identity matrix
*	\param[in,out] M External matrix where to set the generated identity matrix
*/
void ALG_NeutralModelView(Matrix M)
{
	unsigned int k, l;


	for( k=0; k<ALG_MATRIX_DIM; k++ ) 
		for( l=0; l<ALG_MATRIX_DIM; l++ )
		{
			if ( k==l )
				M[k*ALG_MATRIX_DIM+l] = 1.0;
			else
				M[k*ALG_MATRIX_DIM+l] = 0.0;
		}
		M[ALG_MATRIX_SIZE-1] = 0.0;	//No translation at all
}

/**
*	\fn void ALG_RotScaleReset	(Matrix M)

*	Undo all rotations (beware all scaling is lost as well). No translation is lost
*	\param[in,out] M External matrix where to set the generated identity matrix
*/
void ALG_RotScaleReset	(Matrix M)
{
	unsigned int k, l;

	for( k=0; k<ALG_MATRIX_DIM-1; k++ ) 
		for( l=0; l<ALG_MATRIX_DIM-1; l++ )
		{
			if ( k==l )
				M[k*ALG_MATRIX_DIM+l] = 1.0f;
			else
				M[k*ALG_MATRIX_DIM+l] = 0.0f;
		}
}

Vector::Vector(float X, float Y, float Z)
{
	v[X3D] = X;
	v[Y3D] = Y;
	v[Z3D] = Z;
}

void Vector::operator += ( const Vector A)
{
	v[X3D] += A.v[X3D];
	v[Y3D] += A.v[Y3D];
	v[Z3D] += A.v[Z3D];
}

void Vector::operator -= ( const Vector A)
{
	v[X3D] -= A.v[X3D];
	v[Y3D] -= A.v[Y3D];
	v[Z3D] -= A.v[Z3D];
}

Vector  Vector::operator + ( const Vector A) const
{
	Vector P;

	P.v[X3D] = v[X3D] + A.v[X3D];
	P.v[Y3D] = v[Y3D] + A.v[Y3D];
	P.v[Z3D] = v[Z3D] + A.v[Z3D];

	return P;
}

Vector  Vector::operator - ( const Vector A) const
{
	Vector P;

	P.v[X3D] = v[X3D] - A.v[X3D];
	P.v[Y3D] = v[Y3D] - A.v[Y3D];
	P.v[Z3D] = v[Z3D] - A.v[Z3D];

	return P;
}

Vector  Vector::operator - (void) const
{
	Vector P;

	P.v[X3D] = -v[X3D];
	P.v[Y3D] = -v[Y3D];
	P.v[Z3D] = -v[Z3D];

	return P;
}

Vector	Vector::operator *	(float V) const
{
	Vector P;

	P.v[X3D] = v[X3D]*V;
	P.v[Y3D] = v[Y3D]*V;
	P.v[Z3D] = v[Z3D]*V;

	return P;
}

/**
   * General Bezier curve
   * Interpolates the position for any given time.
   * @param p Vector of control points for determining the shape of the Bezier curve.
   * @param n	The amount of control points to be taken into account. The size of parameter p plus one
   *			Uses Vector control_points[n+1];
   *			Defined by:  control_points[0].Position.v[X3D]=... until control_points[n].Position.v[Z3D]=...
   * @param mu The time elapsed in normalized coordinates between [0,1]. Determines the exact position on the curve
   * @see Vector
   * @return A given 3D point that belongs to the curve for mu in the interval [0,1]. Where the curve is at a moment given by parameter mu
   * Calling example => return_point = ALG_Bezier(control_points, 5, t); (example for 6 control points)
   */

Vector ALG_Bezier(Vector *p,int n,double mu)
{
   int k,kn,nn,nkn;
   double blend,muk,munk;
   Vector b;

	b.v[X3D] = 0.0f;
	b.v[Y3D] = 0.0f;
	b.v[Z3D] = 0.0f;

   // Particular case for mu = 1
   if (mu == 1)
   {
	b.v[X3D] = p[n].v[X3D];
	b.v[Y3D] = p[n].v[Y3D];
	b.v[Z3D] = p[n].v[Z3D];
	return(b);
   }

   // General case
   muk = 1;
   munk = pow(1-mu,(double)n);

   for (k=0;k<=n;k++) {
      nn = n;
      kn = k;
      nkn = n - k;
      blend = muk * munk;
      muk *= mu;
      munk /= (1-mu);
      while (nn >= 1) {
         blend *= nn;
         nn--;
         if (kn > 1) {
            blend /= (double)kn;
            kn--;
         }
         if (nkn > 1) {
            blend /= (double)nkn;
            nkn--;
         }
      }
      b.v[X3D] += p[k].v[X3D] * (float)blend;
      b.v[Y3D] += p[k].v[Y3D] * (float)blend;
      b.v[Z3D] += p[k].v[Z3D] * (float)blend;
   }

   return(b);
}
