#ifndef __ENG3DBOX_H__
#define __ENG3DBOX_H__

#include "eng3dvertex.h"
#include "eng3ddefs.h"
#include "eng3dmatrix.h"

/* 
  Box vertexes:
  
                   v7---v6         vMid0 (v0-v1 or v0-v4)
                  /|   / |         vMid1 (v3-v2 or v1-v5)
                v4---v5  |         vMid2 (v7-v6 or v2-v6)
                | v3----v2         vMid3 (v4-v5 or v3-v7)
                | /   |/
                v0---v1
*/

class C3DBox;

typedef void (*TDeleteClientDataCallback)(void *);
typedef unsigned int (*TAskMoreSplitCallback)(C3DBox * Box,void * ClientParam);
typedef unsigned int (*TNotifyCollisionCallback)(C3DBox * Box1,C3DBox * Box2,void * ClientParam);

class C3DBox
{
  public:

  C3DBox(TVertex * v0p,TVertex * v1p,TVertex * v2p,TVertex * v3p,
         TVertex * v4p,TVertex * v5p,TVertex * v6p,TVertex * v7p);

  ~C3DBox();

  //Split lets the client to split this box, keeping them into SplittedBox1 and SplittedBox2. The client
  //could give a callback to the function and the function would ask the client if it must continue splitting
  //the resulting boxes. If a callback is not given the client can specify how many times the box must be
  //splitted.
  //List param must be ignored by the client.
  ENG3DERR Split(TAskMoreSplitCallback MoreSplitCallbackCallback=NULL,
                 void * MoreSplitCallbackClientParam=NULL,
                 unsigned int DoSplitNTimes=1,
                 C3DBox *** List=NULL,
                 unsigned int debugdept=0);

  //This function will test collisions of this box with target box given in Target.
  //If CollideSplittedBoxes parameters is 1 the collision test will be done recursively, the splitted boxes will
  //be tested too and a list of leaf boxes colliding is created in Next.
  unsigned int Collide(C3DBox * Target,
                       TNotifyCollisionCallback NotifyCallback=NULL,
                       void * NotifyCallbackClientParam=NULL,
                       unsigned int CollideSplittedBoxes=1);  


  //This function will test if a vertex is into this box.
  unsigned int IsVertexIntoBox(TVertex * v);

  //This function will apply the matrix m to initial vertexes(ivX), keeping the result in current vertexes (cvX).
  //If the box has children splitted boxes the function will recursively call ApplyBoxToWorldMatrix of the children boxes.
  void ApplyBoxToWorldMatrix(MATRIX * m);

  TVertex * v0;
  TVertex * v1;
  TVertex * v2;
  TVertex * v3;
  TVertex * v4;
  TVertex * v5;
  TVertex * v6;
  TVertex * v7;

  //initial vertexes of the bounding box
  TVertex iv0;
  TVertex iv1;
  TVertex iv2;
  TVertex iv3;
  TVertex iv4;
  TVertex iv5;
  TVertex iv6;
  TVertex iv7;
  
  //current vertexes of the bounding box (after rotations,etc..)
  TVertex cv0;
  TVertex cv1;
  TVertex cv2;
  TVertex cv3;
  TVertex cv4;
  TVertex cv5;
  TVertex cv6;
  TVertex cv7;

  //the client can specify the split point of the box with this function. If the client doesnt specify
  //these points, when Split method is called the box will be cut by the middle of the largest part of
  //the box.
  void SetSplitPoints(TVertex * sp0,
                      TVertex * sp1,
                      TVertex * sp2,
                      TVertex * sp3,
                      unsigned int paramSplitDir);

  C3DBox * SplittedBox1;
  C3DBox * SplittedBox2;
  C3DBox * Next; //Valid for a operation. List of boxes splitted,colliding,etc.. in the last call to Split,Collide...

  void * ClientData; //A client of this class could associate own data to this box. I.e, it could be interesting for
                     //a client implementing a collision detection to keep here a list of vertexes or triangles inside
                     //this box.
  TDeleteClientDataCallback DeleteClientDataCallback;

  void UpdateMembers(); //update some members such as Extents, OrthoBasis, ... Client will never need to call
                        //this function

  TVertex Extents;
  TVertex Position;
  TVertex OrthoBasis[3]; //Orthonormal basis  

private:

  TVertex vMid0;
  TVertex vMid1;
  TVertex vMid2;
  TVertex vMid3;
  unsigned int SplitPointsSetByClient;
  unsigned int SplitDir; //H=1, V=2, D=3

};

#endif