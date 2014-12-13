#include <windows.h>
#include "eng3dbox.h"
#include "eng3dvector.h"
#include "eng3dmatrix.h"

void C3DBox::UpdateMembers()
{
  float mag;

  Position.x = ((v5->x + v3->x)/2);
  Position.y = ((v5->y + v3->y)/2);
  Position.z = ((v5->z + v3->z)/2);

  OrthoBasis[0].x = v2->x - v3->x;
  OrthoBasis[0].y = v2->y - v3->y;
  OrthoBasis[0].z = v2->z - v3->z;

  v_normalize(OrthoBasis[0],mag,OrthoBasis[0]);

  OrthoBasis[1].x = v7->x - v3->x;
  OrthoBasis[1].y = v7->y - v3->y;
  OrthoBasis[1].z = v7->z - v3->z;
  
  v_normalize(OrthoBasis[1],mag,OrthoBasis[1]);

  OrthoBasis[2].x = v0->x - v3->x;
  OrthoBasis[2].y = v0->y - v3->y;
  OrthoBasis[2].z = v0->z - v3->z;

  v_normalize(OrthoBasis[2],mag,OrthoBasis[2]);

  Extents.x = ((v5->x - v3->x))/2;
  Extents.y = ((v5->y - v3->y))/2;
  Extents.z = ((v5->z - v3->z))/2;

  TVertex TempVec;
  TempVec.x=v_dot_product(OrthoBasis[0],Extents);
  TempVec.y=v_dot_product(OrthoBasis[1],Extents);
  TempVec.z=v_dot_product(OrthoBasis[2],Extents);
  Extents.x = fabs(TempVec.x);
  Extents.y = fabs(TempVec.y);
  Extents.z = fabs(TempVec.z);
}

C3DBox::C3DBox(TVertex * v0p,TVertex * v1p,TVertex * v2p,TVertex * v3p,
         TVertex * v4p,TVertex * v5p,TVertex * v6p,TVertex * v7p)
{
  cv0 = iv0 = *v0p; v0 = &cv0;
  cv1 = iv1 = *v1p; v1 = &cv1;
  cv2 = iv2 = *v2p; v2 = &cv2;
  cv3 = iv3 = *v3p; v3 = &cv3;
  cv4 = iv4 = *v4p; v4 = &cv4;
  cv5 = iv5 = *v5p; v5 = &cv5;
  cv6 = iv6 = *v6p; v6 = &cv6;
  cv7 = iv7 = *v7p; v7 = &cv7;
  SplittedBox1=NULL;
  SplittedBox2=NULL;
  Next=NULL;
  ClientData=NULL;
  DeleteClientDataCallback=NULL;
  SplitPointsSetByClient=0;

  UpdateMembers();
}

void C3DBox::ApplyBoxToWorldMatrix(MATRIX * m)
{
  C3DMatrixesTool::ApplyMatrix(m,iv0.x,iv0.y,iv0.z,&cv0.x,&cv0.y,&cv0.z);
  C3DMatrixesTool::ApplyMatrix(m,iv1.x,iv1.y,iv1.z,&cv1.x,&cv1.y,&cv1.z);
  C3DMatrixesTool::ApplyMatrix(m,iv2.x,iv2.y,iv2.z,&cv2.x,&cv2.y,&cv2.z);
  C3DMatrixesTool::ApplyMatrix(m,iv3.x,iv3.y,iv3.z,&cv3.x,&cv3.y,&cv3.z);
  C3DMatrixesTool::ApplyMatrix(m,iv4.x,iv4.y,iv4.z,&cv4.x,&cv4.y,&cv4.z);
  C3DMatrixesTool::ApplyMatrix(m,iv5.x,iv5.y,iv5.z,&cv5.x,&cv5.y,&cv5.z);
  C3DMatrixesTool::ApplyMatrix(m,iv6.x,iv6.y,iv6.z,&cv6.x,&cv6.y,&cv6.z);
  C3DMatrixesTool::ApplyMatrix(m,iv7.x,iv7.y,iv7.z,&cv7.x,&cv7.y,&cv7.z);
  
  if(SplittedBox1 && SplittedBox2)
  {
    SplittedBox1->ApplyBoxToWorldMatrix(m);
    SplittedBox2->ApplyBoxToWorldMatrix(m);
  }

  UpdateMembers();
}

C3DBox::~C3DBox()
{
  if(DeleteClientDataCallback)
    DeleteClientDataCallback(ClientData);
  
  if(SplittedBox1)
    delete SplittedBox1;

  if(SplittedBox2)
    delete SplittedBox2;
}
    
void C3DBox::SetSplitPoints(TVertex * sp0,
                            TVertex * sp1,
                            TVertex * sp2,
                            TVertex * sp3,
                            unsigned int paramSplitDir)
{
  SplitPointsSetByClient=1;
  vMid0=*sp0;
  vMid1=*sp1;
  vMid2=*sp2;
  vMid3=*sp3;
  SplitDir=paramSplitDir;
}

ENG3DERR C3DBox::Split(TAskMoreSplitCallback MoreSplitCallback,
                       void * MoreSplitCallbackClientParam,
                       unsigned int DoSplitNTimes,
                       C3DBox *** List,
                       unsigned int debugdept)
{
  float LenH;
  float LenV;
  float LenD;
  TVertex UnitH;
  TVertex UnitV;
  TVertex UnitD;

  C3DBox ** TmpListPtr;

  if(!DoSplitNTimes)
    return ENG3DERR_NO_ERROR;

  if(!List)
  {
    TmpListPtr = &(this->Next);
    List = &TmpListPtr;
  }
  
  if(SplitPointsSetByClient)
  {
    if(SplitDir==1)
    {
      SplittedBox1 = new C3DBox(v0,&vMid0,&vMid1,v3,v4,&vMid3,&vMid2,v7);
      SplittedBox2 = new C3DBox(&vMid0,v1,v2,&vMid1,&vMid3,v5,v6,&vMid2);
    }
    else if(SplitDir==2)
    {
      SplittedBox1 = new C3DBox(v0,v1,v2,v3,&vMid0,&vMid1,&vMid2,&vMid3);
      SplittedBox2 = new C3DBox(&vMid0,&vMid1,&vMid2,&vMid3,v4,v5,v6,v7);    
    }
    else
    {
      SplittedBox1 = new C3DBox(v0,v1,&vMid3,&vMid0,v4,v5,&vMid2,&vMid1);
      SplittedBox2 = new C3DBox(&vMid0,&vMid3,v2,v3,&vMid1,&vMid2,v6,v7);    
    }
  }
  else //!SplitPointsSetByClient
  {
    vMid0.x = v1->x - v0->x;
    vMid0.y = v1->y - v0->y;
    vMid0.z = v1->z - v0->z;

    vMid1.x = v6->x - v2->x;
    vMid1.y = v6->y - v2->y;
    vMid1.z = v6->z - v2->z;

    vMid2.x = v3->x - v0->x;
    vMid2.y = v3->y - v0->y;
    vMid2.z = v3->z - v0->z;

    v_normalize(vMid0,LenH,UnitH);
    v_normalize(vMid1,LenV,UnitV);
    v_normalize(vMid2,LenD,UnitD);

    if(LenH >= LenV && LenH >= LenD) //cut horizontally
    {
      LenH=LenH/2;

      vMid0.x = v0->x + LenH*UnitH.x;
      vMid0.y = v0->y + LenH*UnitH.y;
      vMid0.z = v0->z + LenH*UnitH.z;

      vMid1.x = v3->x + LenH*UnitH.x;
      vMid1.y = v3->y + LenH*UnitH.y;
      vMid1.z = v3->z + LenH*UnitH.z;

      vMid2.x = v7->x + LenH*UnitH.x;
      vMid2.y = v7->y + LenH*UnitH.y;
      vMid2.z = v7->z + LenH*UnitH.z;

      vMid3.x = v4->x + LenH*UnitH.x;
      vMid3.y = v4->y + LenH*UnitH.y;
      vMid3.z = v4->z + LenH*UnitH.z;

      SplittedBox1 = new C3DBox(v0,&vMid0,&vMid1,v3,v4,&vMid3,&vMid2,v7);
      SplittedBox2 = new C3DBox(&vMid0,v1,v2,&vMid1,&vMid3,v5,v6,&vMid2);
    }
    else if(LenV >= LenH && LenV >= LenD) //cut vertically
    {
      LenV=LenV/2;

      vMid0.x = v0->x + LenV*UnitV.x;
      vMid0.y = v0->y + LenV*UnitV.y;
      vMid0.z = v0->z + LenV*UnitV.z;

      vMid1.x = v1->x + LenV*UnitV.x;
      vMid1.y = v1->y + LenV*UnitV.y;
      vMid1.z = v1->z + LenV*UnitV.z;

      vMid2.x = v2->x + LenV*UnitV.x;
      vMid2.y = v2->y + LenV*UnitV.y;
      vMid2.z = v2->z + LenV*UnitV.z;

      vMid3.x = v3->x + LenV*UnitV.x;
      vMid3.y = v3->y + LenV*UnitV.y;
      vMid3.z = v3->z + LenV*UnitV.z;  
      
      SplittedBox1 = new C3DBox(v0,v1,v2,v3,&vMid0,&vMid1,&vMid2,&vMid3);
      SplittedBox2 = new C3DBox(&vMid0,&vMid1,&vMid2,&vMid3,v4,v5,v6,v7);
    }
    else //cut dephtly
    {
      LenD=LenD/2;

      vMid0.x = v0->x + LenD*UnitD.x;
      vMid0.y = v0->y + LenD*UnitD.y;
      vMid0.z = v0->z + LenD*UnitD.z;

      vMid1.x = v4->x + LenD*UnitD.x;
      vMid1.y = v4->y + LenD*UnitD.y;
      vMid1.z = v4->z + LenD*UnitD.z;

      vMid2.x = v5->x + LenD*UnitD.x;
      vMid2.y = v5->y + LenD*UnitD.y;
      vMid2.z = v5->z + LenD*UnitD.z;

      vMid3.x = v1->x + LenD*UnitD.x;
      vMid3.y = v1->y + LenD*UnitD.y;
      vMid3.z = v1->z + LenD*UnitD.z;  
      
      SplittedBox1 = new C3DBox(v0,v1,&vMid3,&vMid0,v4,v5,&vMid2,&vMid1);
      SplittedBox2 = new C3DBox(&vMid0,&vMid3,v2,v3,&vMid1,&vMid2,v6,v7);  
    }
  }

  **List=SplittedBox1;
  *List=&(SplittedBox1->Next);
  **List=SplittedBox2;
  *List=&(SplittedBox2->Next);
  SplittedBox2->Next=NULL;

  if(MoreSplitCallback)
  {
    if(debugdept==0x186d0)
      __asm nop;

    if(MoreSplitCallback(SplittedBox1,MoreSplitCallbackClientParam))
      SplittedBox1->Split(MoreSplitCallback,MoreSplitCallbackClientParam,1,List,debugdept+1);

    if(MoreSplitCallback(SplittedBox2,MoreSplitCallbackClientParam))
      SplittedBox2->Split(MoreSplitCallback,MoreSplitCallbackClientParam,1,List,debugdept+1);
  }
  else
  {
    SplittedBox1->Split(NULL,NULL,DoSplitNTimes-1,List,debugdept+1);
    SplittedBox2->Split(NULL,NULL,DoSplitNTimes-1,List,debugdept+1);
  }
  return ENG3DERR_NO_ERROR;
}

#define myfabs(x) ((x < 0) ? -x : x)

unsigned int TestBoxBoxDisjoint(float B[3][3], float T[3], float a[3], float b[3])
{
  register float t, s;
  register int r;
  float Bf[3][3];
  const float reps = 1e-6;
  
  // Bf = fabs(B)
  Bf[0][0] = myfabs(B[0][0]);  Bf[0][0] += reps;
  Bf[0][1] = myfabs(B[0][1]);  Bf[0][1] += reps;
  Bf[0][2] = myfabs(B[0][2]);  Bf[0][2] += reps;
  Bf[1][0] = myfabs(B[1][0]);  Bf[1][0] += reps;
  Bf[1][1] = myfabs(B[1][1]);  Bf[1][1] += reps;
  Bf[1][2] = myfabs(B[1][2]);  Bf[1][2] += reps;
  Bf[2][0] = myfabs(B[2][0]);  Bf[2][0] += reps;
  Bf[2][1] = myfabs(B[2][1]);  Bf[2][1] += reps;
  Bf[2][2] = myfabs(B[2][2]);  Bf[2][2] += reps;
  
  // if any of these tests are one-sided, then the polyhedra are disjoint
  r = 1;

  // A1 x A2 = A0
  t = myfabs(T[0]);
  
  r &= (t <= 
	  (a[0] + b[0] * Bf[0][0] + b[1] * Bf[0][1] + b[2] * Bf[0][2]));
  if (!r) return 1;
  
  // B1 x B2 = B0
  s = T[0]*B[0][0] + T[1]*B[1][0] + T[2]*B[2][0];
  t = myfabs(s);

  r &= ( t <=
	  (b[0] + a[0] * Bf[0][0] + a[1] * Bf[1][0] + a[2] * Bf[2][0]));
  if (!r) return 2;
    
  // A2 x A0 = A1
  t = myfabs(T[1]);
  
  r &= ( t <= 
	  (a[1] + b[0] * Bf[1][0] + b[1] * Bf[1][1] + b[2] * Bf[1][2]));
  if (!r) return 3;

  // A0 x A1 = A2
  t = myfabs(T[2]);

  r &= ( t <= 
	  (a[2] + b[0] * Bf[2][0] + b[1] * Bf[2][1] + b[2] * Bf[2][2]));
  if (!r) return 4;

  // B2 x B0 = B1
  s = T[0]*B[0][1] + T[1]*B[1][1] + T[2]*B[2][1];
  t = myfabs(s);

  r &= ( t <=
	  (b[1] + a[0] * Bf[0][1] + a[1] * Bf[1][1] + a[2] * Bf[2][1]));
  if (!r) return 5;

  // B0 x B1 = B2
  s = T[0]*B[0][2] + T[1]*B[1][2] + T[2]*B[2][2];
  t = myfabs(s);

  r &= ( t <=
	  (b[2] + a[0] * Bf[0][2] + a[1] * Bf[1][2] + a[2] * Bf[2][2]));
  if (!r) return 6;

  // A0 x B0
  s = T[2] * B[1][0] - T[1] * B[2][0];
  t = myfabs(s);
  
  r &= ( t <= 
	(a[1] * Bf[2][0] + a[2] * Bf[1][0] +
	 b[1] * Bf[0][2] + b[2] * Bf[0][1]));
  if (!r) return 7;
  
  // A0 x B1
  s = T[2] * B[1][1] - T[1] * B[2][1];
  t = myfabs(s);

  r &= ( t <=
	(a[1] * Bf[2][1] + a[2] * Bf[1][1] +
	 b[0] * Bf[0][2] + b[2] * Bf[0][0]));
  if (!r) return 8;

  // A0 x B2
  s = T[2] * B[1][2] - T[1] * B[2][2];
  t = myfabs(s);

  r &= ( t <=
	  (a[1] * Bf[2][2] + a[2] * Bf[1][2] +
	   b[0] * Bf[0][1] + b[1] * Bf[0][0]));
  if (!r) return 9;

  // A1 x B0
  s = T[0] * B[2][0] - T[2] * B[0][0];
  t = myfabs(s);

  r &= ( t <=
	  (a[0] * Bf[2][0] + a[2] * Bf[0][0] +
	   b[1] * Bf[1][2] + b[2] * Bf[1][1]));
  if (!r) return 10;

  // A1 x B1
  s = T[0] * B[2][1] - T[2] * B[0][1];
  t = myfabs(s);

  r &= ( t <=
	  (a[0] * Bf[2][1] + a[2] * Bf[0][1] +
	   b[0] * Bf[1][2] + b[2] * Bf[1][0]));
  if (!r) return 11;

  // A1 x B2
  s = T[0] * B[2][2] - T[2] * B[0][2];
  t = myfabs(s);

  r &= (t <=
	  (a[0] * Bf[2][2] + a[2] * Bf[0][2] +
	   b[0] * Bf[1][1] + b[1] * Bf[1][0]));
  if (!r) return 12;

  // A2 x B0
  s = T[1] * B[0][0] - T[0] * B[1][0];
  t = myfabs(s);

  r &= (t <=
	  (a[0] * Bf[1][0] + a[1] * Bf[0][0] +
	   b[1] * Bf[2][2] + b[2] * Bf[2][1]));
  if (!r) return 13;

  // A2 x B1
  s = T[1] * B[0][1] - T[0] * B[1][1];
  t = myfabs(s);

  r &= ( t <=
	  (a[0] * Bf[1][1] + a[1] * Bf[0][1] +
	   b[0] * Bf[2][2] + b[2] * Bf[2][0]));
  if (!r) return 14;

  // A2 x B2
  s = T[1] * B[0][2] - T[0] * B[1][2];
  t = myfabs(s);

  r &= ( t <=
	  (a[0] * Bf[1][2] + a[1] * Bf[0][2] +
	   b[0] * Bf[2][1] + b[1] * Bf[2][0]));
  if (!r) return 15;

  return 0;  // should equal 0
}



unsigned int TestBoxBoxOverlap
(
//A
TVertex& a, //extents
TVertex& Pa, //position
TVertex* A, //orthonormal basis
//B
TVertex& b, //extents
TVertex& Pb, //position
TVertex* B //orthonormal basis
)
{
  /*
    Separating axis theorem:

    Both boxes are disjoint if there exists a separating axis which is:
      
      perpendicular to a face from either box,

      or perpedicular to an edge from each box.

    A box has 6 faces and 12 edges, but only 3 edge directions.

    Possible Separating axis:

      2*3 perpendicular to faces (a face in front other one has the same normal's direction),

      3*3 perpendicular to an edge from each box (a box has only 3 edges in different directions).

    We must test these separating axis:

    (note v10,v11,..v17 are first box's vexteses. v20,v21,...v27 are second box's first box).
  
    (v11-v10),(v14-v10),(v13-v10),
    (v11-v10)cross(v21-v20),(v11-v10)cross(v24-v20),(v11-v10)cross(v23-v20),
    (v14-v10)cross(v21-v20),(v14-v10)cross(v24-v20),(v14-v10)cross(v23-v20),
    (v13-v10)cross(v21-v20),(v13-v10)cross(v24-v20),(v13-v10)cross(v23-v20)

  */

	//translation, in parent frame
	TVertex v;
  v.x = Pb.x - Pa.x;
  v.y = Pb.y - Pa.y;
  v.z = Pb.z - Pa.z;

	//translation, in A's frame
  TVertex T = {v_dot_product(v,A[0]), v_dot_product(v,A[1]), v_dot_product(v,A[2])};
	
	//B's basis with respect to A's local frame
	float R[3][3];
	float ra, rb, t;
	long i, k;
	
	//calculate rotation matrix
	for( i=0 ; i<3 ; i++ )
		for( k=0 ; k<3 ; k++ )
			R[i][k] = v_dot_product(A[i],B[k]); 


	/*ALGORITHM: Use the separating axis test for all 15 potential 
	separating axes. If a separating axis could not be found, the two 
	boxes overlap. */
	
	//A's basis vectors
	for( i=0 ; i<3 ; i++ )
	{
    ra = a.v[i];
		
		rb = b.v[0]*fabs(R[i][0]) + b.v[1]*fabs(R[i][1]) + b.v[2]*fabs(R[i][2]);
		
    t = fabs( T.v[i] );
		
		if( t > ra + rb ) 
		  return 0;
	}
	
	//B's basis vectors
	for( k=0 ; k<3 ; k++ )
	{
		ra = a.v[0]*fabs(R[0][k]) + a.v[1]*fabs(R[1][k]) + a.v[2]*fabs(R[2][k]);
		
		rb = b.v[k];
		
		t =	fabs( T.v[0]*R[0][k] + T.v[1]*R[1][k] +	T.v[2]*R[2][k] );
		
		if( t > ra + rb )
		  return 0;
	}
	
	//9 cross products
	
	//L = A0 x B0
	ra = a.v[1]*fabs(R[2][0]) + a.v[2]*fabs(R[1][0]);
	
	rb = b.v[1]*fabs(R[0][2]) + b.v[2]*fabs(R[0][1]);
	
	t = fabs( T.v[2]*R[1][0] - T.v[1]*R[2][0] );
	
	if( t > ra + rb )
	  return 0;
	
	//L = A0 x B1
	ra = a.v[1]*fabs(R[2][1]) + a.v[2]*fabs(R[1][1]);
	
	rb = b.v[0]*fabs(R[0][2]) + b.v[2]*fabs(R[0][0]);
	
	t = fabs( T.v[2]*R[1][1] - T.v[1]*R[2][1] );
	
	if( t > ra + rb )
	  return 0;
	
	//L = A0 x B2
	ra = a.v[1]*fabs(R[2][2]) + a.v[2]*fabs(R[1][2]);
	
	rb = b.v[0]*fabs(R[0][1]) + b.v[1]*fabs(R[0][0]);
	
	t = fabs( T.v[2]*R[1][2] - T.v[1]*R[2][2] );
	
	if( t > ra + rb )
	  return 0;
	
	//L = A1 x B0
	ra = a.v[0]*fabs(R[2][0]) + a.v[2]*fabs(R[0][0]);
	
	rb = b.v[1]*fabs(R[1][2]) + b.v[2]*fabs(R[1][1]);
	
	t = fabs( T.v[0]*R[2][0] - T.v[2]*R[0][0] );
	
	if( t > ra + rb )
	  return 0;
	
	//L = A1 x B1
	ra = a.v[0]*fabs(R[2][1]) + a.v[2]*fabs(R[0][1]);
	
	rb = b.v[0]*fabs(R[1][2]) + b.v[2]*fabs(R[1][0]);
	
	t = fabs( T.v[0]*R[2][1] - T.v[2]*R[0][1] );
	
	if( t > ra + rb )
	  return 0;
	
	//L = A1 x B2
	ra = a.v[0]*fabs(R[2][2]) + a.v[2]*fabs(R[0][2]);
	
	rb = b.v[0]*fabs(R[1][1]) + b.v[1]*fabs(R[1][0]);
	
	t = fabs( T.v[0]*R[2][2] - T.v[2]*R[0][2] );
	
	if( t > ra + rb )
	  return 0;
	
	//L = A2 x B0
	ra = a.v[0]*fabs(R[1][0]) + a.v[1]*fabs(R[0][0]);
	
	rb = b.v[1]*fabs(R[2][2]) + b.v[2]*fabs(R[2][1]);
	
	t = fabs( T.v[1]*R[0][0] - T.v[0]*R[1][0] );
	
	if( t > ra + rb )
	  return 0;
	
	//L = A2 x B1
	ra = a.v[0]*fabs(R[1][1]) + a.v[1]*fabs(R[0][1]);
	
	rb = b.v[0] *fabs(R[2][2]) + b.v[2]*fabs(R[2][0]);
	
	t =	fabs( T.v[1]*R[0][1] - T.v[0]*R[1][1] );
	
	if( t > ra + rb )
	  return 0;
	
	//L = A2 x B2
	ra = a.v[0]*fabs(R[1][2]) + a.v[1]*fabs(R[0][2]);
	
	rb = b.v[0]*fabs(R[2][1]) + b.v[1]*fabs(R[2][0]);
	
	t = fabs( T.v[1]*R[0][2] - T.v[0]*R[1][2] );
	
	if( t > ra + rb )
	  return 0;
	
	/*no separating axis found,	the two boxes overlap */
	return 1;
}



unsigned int TestBoxBoxOverlapDebugX(C3DBox * Box1,C3DBox * Box2)
{
  unsigned int i=0;
  
  TVertex tempv0ba;
  TVertex tempv1ba;
  TVertex tempv2ba;
  TVertex tempv3ba;
  TVertex tempv4ba;
  TVertex tempv5ba;
  TVertex tempv6ba;
  TVertex tempv7ba;

  TVertex v3awa;
  v3awa.x=v_dot_product(Box1->OrthoBasis[0],(*(Box1->v3)));
  v3awa.y=v_dot_product(Box1->OrthoBasis[1],(*(Box1->v3)));
  v3awa.z=v_dot_product(Box1->OrthoBasis[2],(*(Box1->v3)));

  TVertex v0ba; for(i=0;i<3;i++) tempv0ba.v[i]=Box2->v0->v[i]-Box1->v3->v[i]; for(i=0;i<3;i++) v0ba.v[i]= v_dot_product(Box1->OrthoBasis[i],tempv0ba);
  TVertex v1ba; for(i=0;i<3;i++) tempv1ba.v[i]=Box2->v1->v[i]-Box1->v3->v[i]; for(i=0;i<3;i++) v1ba.v[i]= v_dot_product(Box1->OrthoBasis[i],tempv1ba);
  TVertex v2ba; for(i=0;i<3;i++) tempv2ba.v[i]=Box2->v2->v[i]-Box1->v3->v[i]; for(i=0;i<3;i++) v2ba.v[i]= v_dot_product(Box1->OrthoBasis[i],tempv2ba);
  TVertex v3ba; for(i=0;i<3;i++) tempv3ba.v[i]=Box2->v3->v[i]-Box1->v3->v[i]; for(i=0;i<3;i++) v3ba.v[i]= v_dot_product(Box1->OrthoBasis[i],tempv3ba);
  TVertex v4ba; for(i=0;i<3;i++) tempv4ba.v[i]=Box2->v4->v[i]-Box1->v3->v[i]; for(i=0;i<3;i++) v4ba.v[i]= v_dot_product(Box1->OrthoBasis[i],tempv4ba);
  TVertex v5ba; for(i=0;i<3;i++) tempv5ba.v[i]=Box2->v5->v[i]-Box1->v3->v[i]; for(i=0;i<3;i++) v5ba.v[i]= v_dot_product(Box1->OrthoBasis[i],tempv5ba);
  TVertex v6ba; for(i=0;i<3;i++) tempv6ba.v[i]=Box2->v6->v[i]-Box1->v3->v[i]; for(i=0;i<3;i++) v6ba.v[i]= v_dot_product(Box1->OrthoBasis[i],tempv6ba);
  TVertex v7ba; for(i=0;i<3;i++) tempv7ba.v[i]=Box2->v7->v[i]-Box1->v3->v[i]; for(i=0;i<3;i++) v7ba.v[i]= v_dot_product(Box1->OrthoBasis[i],tempv7ba);

    if(v0ba.x > (Box1->Extents.x*2))
      __asm nop; 
    if(v0ba.x<0)
      __asm nop;
    if(v0ba.y > (Box1->Extents.y*2))
      __asm nop;
    if(v0ba.y<0)
      __asm nop;
    if(v0ba.z > (Box1->Extents.z*2))
      __asm nop; 
    if(v0ba.z<0)
      __asm nop;
    if(v1ba.x > (Box1->Extents.x*2))
      __asm nop; 
    if(v1ba.x<0)
      __asm nop;
    if(v1ba.y > (Box1->Extents.y*2))
      __asm nop; 
    if(v1ba.y<0 )
      __asm nop; 
    if(v1ba.z > (Box1->Extents.z*2) )
      __asm nop; 
    if(v1ba.z<0)
      __asm nop; 
    if(v2ba.x > (Box1->Extents.x*2) )
      __asm nop; 
    if(v2ba.x<0 )
      __asm nop; 
    if(v2ba.y > (Box1->Extents.y*2) )
      __asm nop; 
    if(v2ba.y<0 )
      __asm nop; 
    if(v2ba.z > (Box1->Extents.z*2) )
      __asm nop; 
    if(v2ba.z<0) 
      __asm nop; 
    if(v3ba.x > (Box1->Extents.x*2) )
      __asm nop; 
    if(v3ba.x<0 )
      __asm nop; 
    if(v3ba.y > (Box1->Extents.y*2) )
      __asm nop; 
    if(v3ba.y<0 )
      __asm nop; 
    if(v3ba.z > (Box1->Extents.z*2) )
      __asm nop; 
    if(v3ba.z<0) 
      __asm nop; 
    if(v4ba.x > (Box1->Extents.x*2) )
      __asm nop; 
    if(v4ba.x<0 )
      __asm nop; 
    if(v4ba.y > (Box1->Extents.y*2) )
      __asm nop; 
    if(v4ba.y<0 )
      __asm nop; 
    if(v4ba.z > (Box1->Extents.z*2) )
      __asm nop;
    if(v4ba.z<0) 
      __asm nop; 
    if(v5ba.x > (Box1->Extents.x*2) )
      __asm nop; 
    if(v5ba.x<0 )
      __asm nop; 
    if(v5ba.y > (Box1->Extents.y*2) )
      __asm nop; 
    if(v5ba.y<0 )
      __asm nop; 
    if(v5ba.z > (Box1->Extents.z*2) )
      __asm nop; 
    if(v5ba.z<0) 
      __asm nop; 
    if(v6ba.x > (Box1->Extents.x*2) )
      __asm nop; 
    if(v6ba.x<0 )
      __asm nop; 
    if(v6ba.y > (Box1->Extents.y*2) )
      __asm nop; 
    if(v6ba.y<0 )
      __asm nop; 
    if(v6ba.z > (Box1->Extents.z*2) )
      __asm nop; 
    if(v6ba.z<0) 
      __asm nop; 
    if(v7ba.x > (Box1->Extents.x*2) )
      __asm nop; 
    if(v7ba.x<0 )
      __asm nop; 
    if(v7ba.y > (Box1->Extents.y*2) )
      __asm nop; 
    if(v7ba.y<0 )
      __asm nop; 
    if(v7ba.z > (Box1->Extents.z*2) )
      __asm nop; 
    if(v7ba.z<0)
      __asm nop; 


  if(
    (v0ba.x > (Box1->Extents.x*2) || 
    v0ba.x<0 || 
    v0ba.y > (Box1->Extents.y*2) || 
    v0ba.y<0 || 
    v0ba.z > (Box1->Extents.z*2) || 
    v0ba.z<0) &&
    (v1ba.x > (Box1->Extents.x*2) || 
    v1ba.x<0 || 
    v1ba.y > (Box1->Extents.y*2) || 
    v1ba.y<0 || 
    v1ba.z > (Box1->Extents.z*2) || 
    v1ba.z<0) &&
    (v2ba.x > (Box1->Extents.x*2) || 
    v2ba.x<0 || 
    v2ba.y > (Box1->Extents.y*2) || 
    v2ba.y<0 || 
    v2ba.z > (Box1->Extents.z*2) || 
    v2ba.z<0) &&
    (v3ba.x > (Box1->Extents.x*2) || 
    v3ba.x<0 || 
    v3ba.y > (Box1->Extents.y*2) || 
    v3ba.y<0 || 
    v3ba.z > (Box1->Extents.z*2) || 
    v3ba.z<0) &&
    (v4ba.x > (Box1->Extents.x*2) || 
    v4ba.x<0 || 
    v4ba.y > (Box1->Extents.y*2) || 
    v4ba.y<0 || 
    v4ba.z > (Box1->Extents.z*2) ||
    v4ba.z<0) &&
    (v5ba.x > (Box1->Extents.x*2) || 
    v5ba.x<0 || 
    v5ba.y > (Box1->Extents.y*2) || 
    v5ba.y<0 || 
    v5ba.z > (Box1->Extents.z*2) || 
    v5ba.z<0) &&
    (v6ba.x > (Box1->Extents.x*2) || 
    v6ba.x<0 || 
    v6ba.y > (Box1->Extents.y*2) || 
    v6ba.y<0 || 
    v6ba.z > (Box1->Extents.z*2) || 
    v6ba.z<0) &&
    (v7ba.x > (Box1->Extents.x*2) || 
    v7ba.x<0 || 
    v7ba.y > (Box1->Extents.y*2) || 
    v7ba.y<0 || 
    v7ba.z > (Box1->Extents.z*2) || 
    v7ba.z<0)
    )
  {
    return 0;
  }

  return 1;
}


unsigned int TestBoxBoxOverlapDebug(C3DBox * Box1,C3DBox * Box2)
{
  unsigned int a = TestBoxBoxOverlapDebugX(Box1,Box2);
  unsigned int b = TestBoxBoxOverlapDebugX(Box2,Box1);
  return a || b;
}


unsigned int C3DBox::Collide(C3DBox * Target,
                     TNotifyCollisionCallback NotifyCallback,
                     void * NotifyCallbackClientParam,
                     unsigned int CollideSplittedBoxes)
{
  //No need to update. The update is done when toworld matrix is applied (its when the vertexes change)
  //UpdateMembers();
  //Target->UpdateMembers();

  if(!CollideSplittedBoxes)
  {
    return TestBoxBoxOverlap(Extents,Position,OrthoBasis,Target->Extents,Target->Position,Target->OrthoBasis);
  }
  else
  {
    if(TestBoxBoxOverlap(Extents,Position,OrthoBasis,Target->Extents,Target->Position,Target->OrthoBasis))
    {
      //this box is colliding.. continue the tree. If this box is a leaf node of the tree, add the box to the
      //list of colliding boxes.

      unsigned int AnyCollide=0;

      if(SplittedBox1 && SplittedBox2)
      {
        if(Target->SplittedBox1 && Target->SplittedBox2)
        { 
          AnyCollide+=SplittedBox1->Collide(Target->SplittedBox1,NotifyCallback,NotifyCallbackClientParam,1);
          AnyCollide+=SplittedBox1->Collide(Target->SplittedBox2,NotifyCallback,NotifyCallbackClientParam,1);
          AnyCollide+=SplittedBox2->Collide(Target->SplittedBox1,NotifyCallback,NotifyCallbackClientParam,1);
          AnyCollide+=SplittedBox2->Collide(Target->SplittedBox2,NotifyCallback,NotifyCallbackClientParam,1);
        }
        else
        {
          //test against target again
          AnyCollide+=SplittedBox1->Collide(Target,NotifyCallback,NotifyCallbackClientParam,1);
          AnyCollide+=SplittedBox2->Collide(Target,NotifyCallback,NotifyCallbackClientParam,1);          
        }
      }
      else
      {
        if(Target->SplittedBox1 && Target->SplittedBox2)
        { 
          AnyCollide+=Collide(Target->SplittedBox1,NotifyCallback,NotifyCallbackClientParam,1);
          AnyCollide+=Collide(Target->SplittedBox2,NotifyCallback,NotifyCallbackClientParam,1);
        }
        else
        {
          //leaf nodes
          if(NotifyCallback)NotifyCallback(this,Target,NotifyCallbackClientParam);
          return 1;
        }
      }

      return AnyCollide?1:0;
    }
    else
    {
      return 0;
    }
  }
}


unsigned int C3DBox::IsVertexIntoBox(TVertex * v)
{
  MATRIX m = 
  {
    {
      {OrthoBasis[0].x,OrthoBasis[1].x,OrthoBasis[2].x,0},
      {OrthoBasis[0].y,OrthoBasis[1].y,OrthoBasis[2].y,0},
      {OrthoBasis[0].z,OrthoBasis[1].z,OrthoBasis[2].z,0},
      {0-(OrthoBasis[0].x*v3->x+OrthoBasis[0].y*v3->y+OrthoBasis[0].z*v3->z),0-(OrthoBasis[1].x*v3->x+OrthoBasis[1].y*v3->y+OrthoBasis[1].z*v3->z),0-(OrthoBasis[2].x*v3->x+OrthoBasis[2].y*v3->y+OrthoBasis[2].z*v3->z),1},
    }
  };
  
  TVertex vb;
  C3DMatrixesTool::ApplyMatrix(&m,
                               v->x,v->y,v->z,
                               &vb.x,&vb.y,&vb.z);

  if(vb.x < 0 || vb.x > Extents.x*2 || vb.y < 0 || vb.y > Extents.y*2 || vb.z < 0 || vb.z > Extents.z*2)
    return 0;

  return 1;
}