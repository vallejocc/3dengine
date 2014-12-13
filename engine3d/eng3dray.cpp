#include "eng3dray.h"
#include "eng3dvector.h"

C3DRay::C3DRay(TVertex * pP0, TVertex * pP1, unsigned int fixray)
{
  if(fixray)
  {
    fixP0.x = pP0->x; fixP0.y = pP0->y; fixP0.z = pP0->z;
    fixP1.x = pP1->x; fixP1.y = pP1->y; fixP1.z = pP1->z;
    P0 = &fixP0;
    P1 = &fixP1;
  }
  else
  {
    P0 = pP0;
    P1 = pP1;
  }
}

C3DRay::~C3DRay()
{

}

int C3DRay::IntersectRayTriangle( TVertex * TV0, 
                                  TVertex * TV1, 
                                  TVertex * TV2, 
                                  TVertex * IP ,
                                  TVertex * RP0, 
                                  TVertex * RP1)
{
    #define SMALL_NUM  0.00000001

    TVertex    u, v, n;             // triangle vectors
    TVertex    dir, w0, w;          // ray vectors
    float     r, a, b;              // params to calc ray-plane intersect

    // get triangle edge vectors and plane normal
    u.x = TV1->x - TV0->x; u.y = TV1->y - TV0->y; u.z = TV1->z - TV0->z;
    v.x = TV2->x - TV0->x; v.y = TV2->y - TV0->y; v.z = TV2->z - TV0->z;

    v_cross_product(u,v,n);  // cross product
    
    if (n.x == 0 && 
        n.y == 0 &&
        n.z == 0)                  // triangle is degenerate
        return -1;                 // do not deal with this case

    dir.x = RP1->x - RP0->x; dir.y = RP1->y - RP0->y; dir.z = RP1->z - RP0->z;  // ray direction vector
    w0.x = RP0->x - TV0->x; w0.y = RP0->y - TV0->y; w0.z = RP0->z - TV0->z;
    
    a = -v_dot_product(n,w0);
    b = v_dot_product(n,dir);

    if (fabs(b) < SMALL_NUM) {     // ray is parallel to triangle plane
        if (a == 0)                // ray lies in triangle plane
            return 2;
        else return 0;             // ray disjoint from plane
    }

    // get intersect point of ray with triangle plane
    r = a / b;
    if (r < 0.0)                   // ray goes away from triangle
        return 0;                  // => no intersect
    // for a segment, also test if (r > 1.0) => no intersect

    IP->x = RP0->x + r * dir.x;           // intersect point of ray and plane
    IP->y = RP0->y + r * dir.y;
    IP->z = RP0->z + r * dir.z;

    // is I inside T?
    float    uu, uv, vv, wu, wv, D;
    uu = v_dot_product(u,u);
    uv = v_dot_product(u,v);
    vv = v_dot_product(v,v);
    
    w.x = IP->x - TV0->x; w.y = IP->y - TV0->y; w.z = IP->z - TV0->z;
    
    wu = v_dot_product(w,u);
    wv = v_dot_product(w,v);
    D = uv * uv - uu * vv;

    // get and test parametric coords
    float s, t;
    s = (uv * wv - vv * wu) / D;
    if (s < 0.0 || s > 1.0)        // I is outside T
        return 0;
    t = (uv * wu - uu * wv) / D;
    if (t < 0.0 || (s + t) > 1.0)  // I is outside T
        return 0;

    return 1;                      // I is in T
}

int C3DRay::IntersectRayTriangle( TVertex * TV0, 
                                  TVertex * TV1, 
                                  TVertex * TV2, 
                                  TVertex * IP )
{
  return IntersectRayTriangle(TV0,TV1,TV2,IP,P0,P1);
}