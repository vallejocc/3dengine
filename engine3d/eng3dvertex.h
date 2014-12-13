#ifndef __ENG3DVERTEX_H__
#define __ENG3DVERTEX_H__

/*
  TVertex:
    It will be used for representing a 3d point or vertex.
*/

typedef union _TVertex
{
  struct
  {
    float x, y, z;
  };
  
  float v[3];

} TVertex;

/*
  TVertexIndex:
    A object will have a set of vertexes. Usually that vertexes would compose triangles (the entire object will be composed 
    by triangles). We must keep a set of all object's triangles too. But we must keep a array with all object's vertexes, 
    without repetition, for doing some operations that requires to be applied one time to each vertex (traslation or 
    rotation for example).
*/
typedef unsigned int TVertexIndex;  

/*
  Test if a point is part of a segment.
  Return:
    0: its not part
    1: its part
    2: P = S1
    3: P = S2
*/
int IsVertexIntoSegment(TVertex * S1,TVertex * S2, TVertex * P);

#endif