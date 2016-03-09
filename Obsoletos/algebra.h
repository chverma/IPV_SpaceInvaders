/**	Definition common algebra types

*	Prefix: ALG_

*	@author Ramón Mollá
*	@version 2011-10
*/

#ifndef ALG_ALGEBRA
#define ALG_ALGEBRA

//For 3D transformation matrices
#define ALG_MATRIX_DIM 4

//ALG_MATRIX_SIZE = ALG_MATRIX_DIM x ALG_MATRIX_DIM
#define ALG_MATRIX_SIZE 16

typedef enum {
	ALG_TRASLATION,
	ALG_ROTATION,
	ALG_SCALE,
	ALG_MAX_TRANSFORMATION
} ALG_TRANSFORMATION;

typedef enum {
	X3D,
	Y3D,
	Z3D,
	ALG_DIMENSION
} ALG_SpaceCoords;

//Do not alter the order of these labels
typedef enum {
	ALG_NO_DIM,
	ALG_1D,
	ALG_2D,
	ALG_3D
} ALG_SpaceDims;

/// A VERY simple vector struct
class  Vector
{
public:

	float v[ALG_DIMENSION];

	//Methods
	inline void Reset(void) {v[X3D] = v[Y3D] = v[Z3D] = 0.0f;};

	Vector(){};
	Vector(float X, float Y, float Z);

	void	operator += ( const Vector);
	void	operator -= ( const Vector);
	Vector	operator +  ( const Vector) const;
	Vector	operator -  ( const Vector) const;
	Vector	operator -	(void) const;
	Vector	operator *	(float V) const;
};	//Class

// Vertex struct to make code easier to read in places

typedef Vector Vertex;
typedef float Matrix[ALG_MATRIX_SIZE];

Vector ALG_Bezier(Vector *p,int n,double mu);	///<BEZIER CURVE FOR CAMERA MOVEMENT TO 3D

void ALG_NeutralModelView	(Matrix);
void ALG_RotScaleReset		(Matrix);

#endif