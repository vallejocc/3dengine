/*
 * int tri_tri_intersect(float V0[3],float V1[3],float V2[3],
 *                       float U0[3],float U1[3],float U2[3])
 *
 * parameters: vertices of triangle 1: V0,V1,V2
 *             vertices of triangle 2: U0,U1,U2
 * result    : returns 1 if the triangles intersect, otherwise 0
 *
 * Here is a version withouts divisions (a little faster)
 * int NoDivTriTriIsect(float V0[3],float V1[3],float V2[3],
 *                      float U0[3],float U1[3],float U2[3]);
 * 
 * This version computes the line of intersection as well (if they are not coplanar):
 * int tri_tri_intersect_with_isectline(float V0[3],float V1[3],float V2[3], 
 *				        float U0[3],float U1[3],float U2[3],int *coplanar,
 *				        float isectpt1[3],float isectpt2[3]);
 * coplanar returns whether the tris are coplanar
 * isectpt1, isectpt2 are the endpoints of the line of intersection
 */

#ifndef __ENG3DTRI_H__
#define __ENG3DTRI_H__

int tri_tri_intersect(float V0[3],float V1[3],float V2[3],
                      float U0[3],float U1[3],float U2[3]);

int NoDivTriTriIsect(float V0[3],float V1[3],float V2[3],
                     float U0[3],float U1[3],float U2[3]);

int tri_tri_intersect_with_isectline(float V0[3],float V1[3],float V2[3],
                                     float U0[3],float U1[3],float U2[3],
                                     int *coplanar,
                                     float isectpt1[3],float isectpt2[3]);

 /*    
    Use separating axis theorem to test overlap between triangle and box
    need to test for overlap in these directions:
    1) the {x,y,z}-directions (actually, since we use the AABB of the triangle
       we do not even need to test these)
    2) normal of the triangle
    3) crossproduct(edge from tri, {x,y,z}-directin)
       this gives 3x3=9 more tests 
 */

int triAABBBoxOverlap(float boxcenter[3],float boxhalfsize[3],float triverts[3][3]);

#endif