#ifndef __ENG3DVECTORS_H__
#define __ENG3DVECTORS_H__

#include <math.h>

#define v_dot_product(A, B) \
  ( A.x*B.x + A.y*B.y + A.z*B.z )

#define v_magnitude(A) \
  ( sqrt( v_dot_product(A,A) ) )

#define v_normalize( A, mag, result ) \
  mag = v_magnitude(A); \
  if (mag != 0.0) { \
     result.x = A.x / mag; \
     result.y = A.y / mag; \
     result.z = A.z / mag; \
  } else { \
     result.x = 0.0; \
     result.y = 0.0; \
     result.z = 0.0; \
  }

#define v_cross_product(A, B, result) \
    result.x = A.y*B.z - A.z*B.y; \
    result.y = A.z*B.x - A.x*B.z; \
    result.z = A.x*B.y - A.y*B.x 

#endif