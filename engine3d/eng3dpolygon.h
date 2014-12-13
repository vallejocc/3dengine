#ifndef __ENG3DPOLYGON_H__
#define __ENG3DPOLYGON_H__

#include "eng3dvertex.h"

/*
  TTriangle:
    The entire object will be composed by triangles. These triangles will be composed by three vertexes (a,b,c). Instead of 
    representing that vertexes with TVertex types we will represent them with indexes to a array containing all object's 
    vertexes (that array will contain TVertex elements).
*/
typedef struct _TTriangle
{
  TVertexIndex a,b,c;
} TTriangle;

#endif