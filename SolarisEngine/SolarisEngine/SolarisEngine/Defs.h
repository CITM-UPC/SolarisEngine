#ifndef __DEFS_H__
#define __DEFS_H__

#include <stdio.h>
#include <vector>
#include <glm/glm.hpp>
#include <chrono>



using hrclock = std::chrono::high_resolution_clock;
using namespace std::chrono_literals;
using u8vec4 = glm::u8vec4;
using ivec2 = glm::ivec2;
using vec3 = glm::dvec3;


//  NULL just in case ----------------------

#ifdef NULL
#undef NULL
#endif
#define NULL  0

// Deletes a buffer
#define RELEASE( x ) \
	{						\
	if( x != NULL )		\
		{					  \
	  delete x;			\
	  x = NULL;			  \
		}					  \
	}

// Deletes an array of buffers
#define RELEASE_ARRAY( x ) \
	{							  \
	if( x != NULL )			  \
		{							\
	  delete[] x;				\
	  x = NULL;					\
		}							\
							  \
	}

#define IN_RANGE( value, min, max ) ( ((value) >= (min) && (value) <= (max)) ? 1 : 0 )
#define MIN( a, b ) ( ((a) < (b)) ? (a) : (b) )
#define MAX( a, b ) ( ((a) > (b)) ? (a) : (b) )
#define TO_BOOL( a )  ( (a != 0) ? true : false )

typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;

template <class VALUE_TYPE> void SWAP(VALUE_TYPE& a, VALUE_TYPE& b)
{
	VALUE_TYPE tmp = a;
	a = b;
	b = tmp;
}

// Standard string size
#define SHORT_STR	 32
#define MID_STR	    255
#define HUGE_STR   8192

// Joins a path and file
inline const char* const PATH(const char* folder, const char* file)
{
	static char path[MID_STR];
	sprintf_s(path, MID_STR, "%s/%s", folder, file);
	return path;
}

// Performance macros
#define PERF_START(timer) timer.Start()
#define PERF_PEEK(timer) LOG("%s took %f ms", __FUNCTION__, timer.ReadMs())

inline double lerp(double A, double B, double t) {
	return A + (B - A) * t;
}

// Maths calculations
#define PI 3.14159265359f
#define DEG_2_RAD(a) a * PI / 180.0f
#define RAD_2_DEG(a) a * 180.0f / PI





#define ICON_MIN 0xE000
#define ICON_MAX 0xF8FF


#define ASSETS_DIR "./Assets"
#define LIBRARY_DIR "./Library"
#define MESHES_DIR "Library/Meshes"
#define MATERIALS_DIR "Library/Materials"
#define TEXTURES_DIR "Library/Textures"
#define MODELS_DIR "Library/Models"
#define FONTS_DIR "Packages/Fonts"
#define ICONS_DIR "Packages/Icons"



#endif	// __DEFS_H__