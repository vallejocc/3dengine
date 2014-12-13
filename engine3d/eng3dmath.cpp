#include "eng3dmath.h"

float C3DMath::ProjectPointToLine(TVertex P, TVertex L1, TVertex L2, TVertex * Pproj)
{
  /*
    theory:

      Given a 3d segment L2-L1 and a point P, the minimal distance d from the point P to the line L2-L1:

      d = |(L2-L1)x(L1-P)| / |L2-L1|

      Having this parameters:

                P  
               /| \
              / |d \ |L1-P|
             /  |   \
          L2 ---Pp--- L1
              |L2-L1|
      
        |L1-Pp|*|L1-Pp| + d*d = |L1-P|*|L1-P|

        |L1-Pp| = sqrt(|L1-P|*|L1-P| - d*d)

        Having |L1-Pp| we need to know if Pp is nearer L2 than L1 or not. For that we will get two possible Pp points 
        (based on |L1-Pp| distance from L1). But we know too that Pp-P is perpendicular to L2-L1. We can use that 
        for knowing the projected point.
  */


  return 0;
}