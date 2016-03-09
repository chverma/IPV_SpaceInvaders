/**	Definition common algebra types

*	Prefix: UGKALG_

*	@author Ramón Mollá
*	@version 2014-05
*/

#ifndef UGKALG_ALGEBRA
#define UGKALG_ALGEBRA

//UGKALG_MATRIX_SIZE = TOTAL_DIMENSIONS x TOTAL_DIMENSIONS
#define UGKALG_MATRIX_SIZE 16

//Selection of the Algebra API used
#define UGKALG_API_DEFAULT			0
//#define UGKALG_API_EIGEN			1
//#define UGKALG_API_GLM			2
//#define UGKALG_API_BLAS			3
//#define UGKALG_API_FIXED_POINT	4

#ifdef UGKALG_API_DEFAULT
	#define UGKALG_SIMPLE
	//#define UGKALG_DOUBLE
	//#define UGKALG_FIXED_POINT

	#ifdef UGKALG_SIMPLE
		#define UGKALG_NUMERIC_TYPE float
		#define UGKALG_ONE			1.0f
		#define UGKALG_ZERO			0.0f
	#elif defined (UGKALG_DOUBLE)
		#define UGKALG_NUMERIC_TYPE double
		#define UGKALG_ONE			1.0
		#define UGKALG_ZERO			0.0
	#elif defined (UGKALG_FIXED_POINT)
		#include <UGKFPA64b.h>
		#define UGKALG_NUMERIC_TYPE UGKFPA64b
		#define UGKALG_ONE			UGKFPA64b_ONE	
		#define UGKALG_ZERO			UGKFPA64b_ZERO
	#endif
#elif defined(UGKALG_API_EIGEN)
	#include <Eigen/Core>
	#include <Eigen/Geometry>
	#define UGKALG_NUMERIC_TYPE float
	#define UGKALG_ONE			1.0f
	#define UGKALG_ZERO			0.0f
#elif defined(UGKALG_API_GLM)
	#include <glm/glm.hpp>
	#include <glm/gtc/type_ptr.hpp>
	#define UGKALG_NUMERIC_TYPE float
	#define UGKALG_ONE			1.0f
	#define UGKALG_ZERO			0.0f
#endif

#include <UGK.h>

namespace UGK
{
	typedef enum {
		TRASLATION,
		ROTATION,
		SCALE,
		MAX_TRANSFORMATION
	} TRANSFORMATION_TYPE;

	typedef enum {
		XDIM,
		YDIM,
		ZDIM,
		WDIM,
		TOTAL_DIMENSIONS
	} SpaceCoords;

	//Do not alter the order of these labels
	typedef enum {
		UGKALG_NO_DIM,
		UGKALG_1D,
		UGKALG_2D,
		UGKALG_3D
	} UGKALG_SpaceDims;

	#ifdef UGKALG_API_DEFAULT
		typedef UGKALG_NUMERIC_TYPE f4[TOTAL_DIMENSIONS];
	#elif UGKALG_API_EIGEN
		// Don't align or application will assert at the beggining. More info:
		// http://eigen.tuxfamily.org/dox-devel/group__TopicStructHavingEigenMembers.html
		typedef f4	Eigen::Matrix<UGKALG_NUMERIC_TYPE, TOTAL_DIMENSIONS, 1, Eigen::DontAlign>
	#elif UGKALG_API_GLM
		typedef f4 glm::vec4
	#endif
	
	/// A VERY simple vector struct
	class  UGK_API Vector
	{
	public:
		f4 v;
	
		//Methods
		void Reset(void);
		void Set(UGKALG_NUMERIC_TYPE X, UGKALG_NUMERIC_TYPE Y, UGKALG_NUMERIC_TYPE Z);

		Vector(){};
		Vector(UGKALG_NUMERIC_TYPE X, UGKALG_NUMERIC_TYPE Y, UGKALG_NUMERIC_TYPE Z);

		void	operator += ( const Vector&);
		void	operator -= ( const Vector&);
		Vector	operator +  ( const Vector&) const;
		Vector	operator -  ( const Vector&) const;
		Vector	operator -	(void) const;
		Vector	operator *	(UGKALG_NUMERIC_TYPE V) const;

	};	//Class Vector

	/// A VERY simple vector struct
	class  UGK_API Matrix
	{
	public:
	#ifdef UGKALG_API_DEFAULT
		UGKALG_NUMERIC_TYPE M[UGKALG_MATRIX_SIZE];
	#elif UGKALG_API_EIGEN
		// Don't align or application will assert at the beggining. More info:
		// http://eigen.tuxfamily.org/dox-devel/group__TopicStructHavingEigenMembers.html
		Eigen::Matrix<UGKALG_NUMERIC_TYPE, UGKALG_MATRIX_SIZE, 1, Eigen::DontAlign> M;
	#elif UGKALG_API_GLM
		glm::mat4 M;
	#endif
		//Methods
		void Reset(void);
		
		Matrix(){};
		void SetIdentity	();
		void RotScaleReset	();
		
		void	operator += ( const Matrix&);
		void	operator -= ( const Matrix&);
		Matrix	operator +  ( const Matrix&) const;
		Matrix	operator -  ( const Matrix&) const;
		Matrix	operator -	(void) const;
		Matrix	operator *	(UGKALG_NUMERIC_TYPE V) const;

	};	//Class Matrix

// Vertex struct to make code easier to read in places
typedef Vector Vertex;

Vector UGK_API Bezier(Vector *p, int n,UGKALG_NUMERIC_TYPE mu);	///<BEZIER CURVE interpolator

} //namespace UGK

#define NeutralModelView() SetIdentity()

#endif