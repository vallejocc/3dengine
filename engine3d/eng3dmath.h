#ifndef __ENG3DMATH_H__
#define __ENG3DMATH_H__

#include "eng3dvertex.h"

class C3DMath
{
  static float ProjectPointToLine(TVertex P, TVertex L1, TVertex L2, TVertex * Pproj);

};

#endif