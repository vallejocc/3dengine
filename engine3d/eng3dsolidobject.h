#ifndef __ENG3DSOLIDOBJECT_H__
#define __ENG3DSOLIDOBJECT_H__

#include "eng3dobject.h"
#include "eng3dbox.h"

class C3DSolidObject : public C3DObject
{

public:

  C3DSolidObject();
  ~C3DSolidObject();
  
  //this function will detect a collision between this object and Target object.
  unsigned int Collide(C3DSolidObject * Target);

  //these functions need to be public, but not intended to be used by the client of the class
  void UpdateAABBTree();
  unsigned int BoundingBoxAskMoreSplitCallback(C3DBox * Box); //This class will stop of splitting the box when
                                                              //the children box had only a vertex inside.
                                                              //The vertex into the box is pointed by the ClientData
                                                              //member of the children box.
  
  //Bounding box
  C3DBox * BBox;
  //ObjectToWorld matrix must be applied to the bounding box before using it to detect collisions
  unsigned int BBoxNeedToBeUpdated;

  //Object's extreme vertexes (object coords)
  TVertex * MaxX;
  TVertex * MaxY;
  TVertex * MaxZ;
  TVertex * MinX;
  TVertex * MinY;
  TVertex * MinZ;

  //Top bounding box vertexes
  TVertex v0;
  TVertex v1;
  TVertex v2;
  TVertex v3;
  TVertex v4;
  TVertex v5;
  TVertex v6;
  TVertex v7;

  //Last collision information
  typedef struct
  {
    ENG3D_LIST_ENTRY Link;
    TTriangle * TriangleObject1;
    TTriangle * TriangleObject2;
    TVertex IntersectionLinePO;
    TVertex IntersectionLinePD;
    int Coplanar;
  }tCollisionInfo;
  

  ENG3D_LIST_ENTRY CollisionInfoList;

  void EmptyCollisionInfoList();

  private:

  void C3DSolidObject::EmptyLeafNodeBoundingBoxesList();
  ENG3D_LIST_ENTRY LeafNodeBoundingBoxesList;

  protected:

  void VertexesChangesCallbackC3DSolidObject(TVERTEXES_CHANGE_ID ChangeId,
                                             TVertex * ChangedVertex,
                                             void * ChangeParams);

  virtual void VertexesChangesCallback(TVERTEXES_CHANGE_ID ChangeId,
                                     TVertex * ChangedVertex,
                                     void * ChangeParams);
};

#define SOLID_OBJECT(a) ((C3DSolidObject*)a)

#endif