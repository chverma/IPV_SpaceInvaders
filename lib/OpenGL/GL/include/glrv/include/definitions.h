/***********************************************************
Roberto Vivó (c) 2012.

Definiciones y tipos

***********************************************************/
#ifndef RV_DEFINITIONS_H
#define RV_DEFINITIONS_H v1.0

#define GLM_SWIZZLE
#include <glm/glm.hpp>

#define PI 3.1415926535897932384626433832795f
#define dPI 6.283185307179586476925286766559f
#define hPI 1.5707963267948966192313216916398f
#define EPSILON 0.01f

namespace glrv
{
	extern unsigned int TGTE;
};

typedef glm::vec4 Punto;
typedef glm::vec4 Color;
typedef glm::vec3 Vector;


#endif // RV_DEFINITIONS_H
