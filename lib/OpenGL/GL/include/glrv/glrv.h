/***********************************************************
Roberto Vivó (c) 2012.

Clases y funciones de utilidad para gráficos.

***********************************************************/
#ifndef RVIVO_H
#define RVIVO_H v1.0

#ifdef USE_GLSDK
#include <glload/gl_all.h>
#include <glload/gll.hpp>
#include <glimg/glimg.h>
#else
#include <GL/glew.h>
#endif
#include <glm/glm.hpp>

#include "include/definitions.h"
#include "include/axis.h"
#include "include/colorcube.h"
#include "include/curve.h"
#include "include/tube.h"
#include "include/images.h"
#include "include/shaders.h"
#include "include/shapefile.h"

#endif // RVIVO_H
