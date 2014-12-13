#ifndef __ENG3DRAY_H__
#define __ENG3DRAY_H__

#include "eng3dvertex.h"

// Important for this class:
// A line is infinite, rays and segments start at P0 and a ray extends beyond P1, but a segment ends at P1

class C3DRay
{
public:

  C3DRay(TVertex * pP0, TVertex * pP1, unsigned int fixray = 1);
  ~C3DRay();

  // IntersectRayTriangle(): intersect a ray with a 3D triangle
  //    Input:  a ray RP1-RP2, and a triangle TV1,TV2,TV3
  //    Output: IP = intersection point (when it exists)
  //    Return: -1 = triangle is degenerate (a segment or point)
  //             0 = disjoint (no intersect)
  //             1 = intersect in unique point I1
  //             2 = are in the same plane

  static int IntersectRayTriangle( TVertex * TV0, 
                                   TVertex * TV1, 
                                   TVertex * TV2, 
                                   TVertex * IP,                                  
                                   TVertex * RP0, 
                                   TVertex * RP1 );
  
  int IntersectRayTriangle( TVertex * TV0, 
                            TVertex * TV1, 
                            TVertex * TV2, 
                            TVertex * IP);
  TVertex * P1;
  TVertex * P0;
  TVertex fixP1;
  TVertex fixP0;
};

#endif



