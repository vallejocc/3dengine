#include "eng3dvertex.h"
#include "eng3dvector.h"

int IsVertexIntoSegment(TVertex * S1,TVertex * S2, TVertex * P)
{
  TVertex Dir1;
  TVertex Dir2;
  float mag;

  if((P->x - S1->x == 0) && (P->y - S1->y == 0) && (P->z - S1->z == 0))
    return 2;

  if((P->x - S2->x == 0) && (P->y - S2->y == 0) && (P->z - S2->z == 0))
    return 3;

  Dir1.x = S2->x - S1->x; Dir1.y = S2->y - S1->y; Dir1.z = S2->z - S1->z;
  Dir2.x = P->x - S1->x; Dir2.y = P->y - S1->y; Dir2.z = P->z - S1->z;
  
  if(Dir2.x > Dir1.x || Dir2.y > Dir1.y || Dir2.z > Dir1.z)
  {
    //If P is contained, it cant be farer that S2 from S1
    return 0;
  }
  
  v_normalize(Dir1,mag,Dir1);
  v_normalize(Dir2,mag,Dir2);

  if(Dir1.x - Dir2.x || Dir1.y - Dir2.y || Dir1.z - Dir2.z)
  {
    //diferent, no contained
    return 0;
  }

  return 1;
}