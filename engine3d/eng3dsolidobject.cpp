#include "eng3dsolidobject.h"
#include "eng3ddefs.h"
#include "eng3dpolygon.h"
#include "eng3dvertex.h"
#include "eng3dtri.h"
#include "eng3dvector.h"

typedef struct
{
  ENG3D_LIST_ENTRY Link;
  TTriangle * t;
}tTrianglesList;

typedef struct
{
  TVertex * v; //vertex into the box
  ENG3D_LIST_ENTRY t; //triangles with a vertex being v (its a list of type tTrianglesList)

}tBoundingBoxClientData;

typedef struct
{
  ENG3D_LIST_ENTRY Link;
  C3DBox * Box;
}tLeafNodeBoxList;

void BoundingBoxesDeleteClientDataCallback(void * ClientData)
{
  tBoundingBoxClientData * cd = (tBoundingBoxClientData *)ClientData;

  while(!Eng3dIsListEmpty(&cd->t))
  {
    tTrianglesList * t;
    ENG3D_LIST_ENTRY * Next;
    Next=cd->t.Flink;
    Eng3dExtractEntry(Next);
    t = ENG3D_BASE_FROM_FIELD(tTrianglesList,Link,Next);
    delete t;
  }
}

unsigned int PreBoundingBoxNotifyCollisionCallback(C3DBox * Box1,C3DBox * Box2,void * ClientParam)
{
  TColor color={1,0,0};
  TColor color2={0,0,1};  
  
  //here we have been notified a splitted box from Box1 and a splitted box from Box2 have collided. Now
  //we must test collisions of contanied triangles, and keep info about that collisions for first and second object.
  
  ENG3D_LIST_ENTRY * Node1;
  ENG3D_LIST_ENTRY * Node2;
  tTrianglesList *t1;
  tTrianglesList *t2;
  tBoundingBoxClientData * cd1 = (tBoundingBoxClientData *)Box1->ClientData;
  tBoundingBoxClientData * cd2 = (tBoundingBoxClientData *)Box2->ClientData;
  C3DSolidObject ** CollidingObjects = (C3DSolidObject **)ClientParam;
  
  CollidingObjects[0]->UpdateWorldObjectVertexes();
  CollidingObjects[1]->UpdateWorldObjectVertexes();

  Node1=cd1->t.Flink;
  
  while(Node1!=&cd1->t)
  {
    Node2=cd2->t.Flink;
    t1 = ENG3D_BASE_FROM_FIELD(tTrianglesList,Link,Node1);


    while(Node2!=&cd2->t)
    {
      int coplanar=0;
      TVertex isectO;
      TVertex isectD;
      t2 = ENG3D_BASE_FROM_FIELD(tTrianglesList,Link,Node2);      

      if(tri_tri_intersect_with_isectline(CollidingObjects[0]->WorldObjectVertexes[t1->t->a].v,
                                          CollidingObjects[0]->WorldObjectVertexes[t1->t->b].v,
                                          CollidingObjects[0]->WorldObjectVertexes[t1->t->c].v,
                                          CollidingObjects[1]->WorldObjectVertexes[t2->t->a].v,
                                          CollidingObjects[1]->WorldObjectVertexes[t2->t->b].v,
                                          CollidingObjects[1]->WorldObjectVertexes[t2->t->c].v,
                                          &coplanar,
                                          isectO.v,
                                          isectD.v))
      {
        //keep info about the collision
        C3DSolidObject ::tCollisionInfo * ci;
        ci = new C3DSolidObject ::tCollisionInfo;
        ci->TriangleObject1 = t1->t;
        ci->TriangleObject2 = t2->t;
        ci->IntersectionLinePO = isectO;
        ci->IntersectionLinePD = isectD;
        ci->Coplanar = coplanar;
        Eng3dPostInsertEntry(&CollidingObjects[0]->CollisionInfoList,&ci->Link);
      }

      Node2 = Node2->Flink;
    }

    Node1 = Node1->Flink;
  }

  return 0;
}

unsigned int C3DSolidObject::Collide(C3DSolidObject * Target)
{
  EmptyCollisionInfoList();

  if(BBoxNeedToBeUpdated)
  {
    BBox->ApplyBoxToWorldMatrix(&ObjectToWorldMatrix);
    BBoxNeedToBeUpdated=0;
  }

  if(Target->BBoxNeedToBeUpdated)
  {
    Target->BBox->ApplyBoxToWorldMatrix(&Target->ObjectToWorldMatrix);
    Target->BBoxNeedToBeUpdated=0;  
  }
  
  C3DSolidObject * CollidingObjects[] = {this,Target};

  if(BBox->Collide(Target->BBox,PreBoundingBoxNotifyCollisionCallback,CollidingObjects))
  {
    if(!Eng3dIsListEmpty(&CollisionInfoList))
      return 1;
  }

  return 0;
}

unsigned int C3DSolidObject::BoundingBoxAskMoreSplitCallback(C3DBox * Box)
{
  unsigned int i=0;
  unsigned int nVertexesIntoBox=0;
  TVertexIndex VertexIndex=0;
  TVertex * FoundVertexes[2];

  for(i=0;i<nObjectVertexes && nVertexesIntoBox<=1;i++)
  {
    if(nVertexesIntoBox && 
       ObjectVertexes[i].x==FoundVertexes[0]->x &&
       ObjectVertexes[i].y==FoundVertexes[0]->y &&
       ObjectVertexes[i].z==FoundVertexes[0]->z)
    {
      //same values for different verteses!!!! With 3ds objects it occurs
      i++;
      continue;
    }

    if(Box->IsVertexIntoBox(&ObjectVertexes[i]))
    {
      if(!nVertexesIntoBox) //we take the first one. We are skiping repeated triangles for this vertex
        VertexIndex=i;

      FoundVertexes[nVertexesIntoBox] = &ObjectVertexes[i];
      nVertexesIntoBox++;
    }
  }

  if(!nVertexesIntoBox)
  {
    //put a empty client data

    tBoundingBoxClientData * cd = new tBoundingBoxClientData;

    cd->v = NULL;
    Eng3dInitList(&cd->t);

    Box->ClientData = cd;
    Box->DeleteClientDataCallback = BoundingBoxesDeleteClientDataCallback;

    tLeafNodeBoxList * LeafNodeBox = new tLeafNodeBoxList;
    LeafNodeBox->Box = Box;
    Eng3dPostInsertEntry(&LeafNodeBoundingBoxesList,&LeafNodeBox->Link);

    return 0;
  }

  if(nVertexesIntoBox>1)
  {
    return 1;
  }

  //only a vertex (or multiple vertexes with the same coords). Keep it and its triangles
  tBoundingBoxClientData * cd = new tBoundingBoxClientData;

  cd->v = &ObjectVertexes[VertexIndex];
  Eng3dInitList(&cd->t);

  //change: we dont need to keep triangles here. After splitting boxes we will check all triangles
  //against all leaf node boxes checking intersections.
  /*
  tTrianglesList * node;

  for(i=0;i<nObjectTriangles;i++)
  {
    if(
       (ObjectVertexes[ObjectTriangles[i].a].x == ObjectVertexes[VertexIndex].x &&
       ObjectVertexes[ObjectTriangles[i].a].y == ObjectVertexes[VertexIndex].y &&
       ObjectVertexes[ObjectTriangles[i].a].z == ObjectVertexes[VertexIndex].z)    
    ||
       (ObjectVertexes[ObjectTriangles[i].b].x == ObjectVertexes[VertexIndex].x &&
       ObjectVertexes[ObjectTriangles[i].b].y == ObjectVertexes[VertexIndex].y &&
       ObjectVertexes[ObjectTriangles[i].b].z == ObjectVertexes[VertexIndex].z)
    ||
       (ObjectVertexes[ObjectTriangles[i].c].x == ObjectVertexes[VertexIndex].x &&
       ObjectVertexes[ObjectTriangles[i].c].y == ObjectVertexes[VertexIndex].y &&
       ObjectVertexes[ObjectTriangles[i].c].z == ObjectVertexes[VertexIndex].z)
      )
    {
      //the vertex is part of this triangle
      node = new tTrianglesList;
      node->t = &ObjectTriangles[i];
      Eng3dPostInsertEntry(&cd->t,&node->Link);
    }
  }
  */

  Box->ClientData = cd;
  Box->DeleteClientDataCallback = BoundingBoxesDeleteClientDataCallback;

  //note here we added to the list the triangles with the found vertex into
  //the box. But we could find triangles crossing this splitted box without
  //a vertex into it. When we had finished the splitting process we will
  //go across all leaf nodes of the tree of splitted boxes and we will check
  //what triangles are crossing the box too, and we will add them to the list
  //of triangles. For this purpose we will create other list of leaf nodes.

  tLeafNodeBoxList * LeafNodeBox = new tLeafNodeBoxList;
  LeafNodeBox->Box = Box;
  Eng3dPostInsertEntry(&LeafNodeBoundingBoxesList,&LeafNodeBox->Link);

  return 0;
}

unsigned int PreBoundingBoxAskMoreSplitCallback(C3DBox * Box,void * ClientParam)
{
  C3DSolidObject * Obj = (C3DSolidObject *)ClientParam;
  return Obj->BoundingBoxAskMoreSplitCallback(Box);
}

void C3DSolidObject::UpdateAABBTree()
{
  v0.x = MinX->x; v0.y = MinY->y; v0.z = MaxZ->z;
  v1.x = MaxX->x; v1.y = MinY->y; v1.z = MaxZ->z;
  v2.x = MaxX->x; v2.y = MinY->y; v2.z = MinZ->z;
  v3.x = MinX->x; v3.y = MinY->y; v3.z = MinZ->z;
  v4.x = MinX->x; v4.y = MaxY->y; v4.z = MaxZ->z;
  v5.x = MaxX->x; v5.y = MaxY->y; v5.z = MaxZ->z;
  v6.x = MaxX->x; v6.y = MaxY->y; v6.z = MinZ->z;
  v7.x = MinX->x; v7.y = MaxY->y; v7.z = MinZ->z;

  if(BBox)delete BBox;
  BBox = new C3DBox(&v0,&v1,&v2,&v3,&v4,&v5,&v6,&v7);
  if(PreBoundingBoxAskMoreSplitCallback(BBox,this))
    BBox->Split(PreBoundingBoxAskMoreSplitCallback,this);
    
  //the splitting process have finished, but we must check crossing triangles across leaf node boxes.
  
  unsigned int i=0;
  ENG3D_LIST_ENTRY * Temp;

  for(i=0;i<nObjectTriangles;i++)
  {
    for(Temp=LeafNodeBoundingBoxesList.Flink;Temp!=&LeafNodeBoundingBoxesList;Temp=Temp->Flink)
    {
      tLeafNodeBoxList * lnb = ENG3D_BASE_FROM_FIELD(tLeafNodeBoxList,Link,Temp);
      
      TVertex BoxCenter = {(lnb->Box->v0->x+lnb->Box->v6->x)/2,
                           (lnb->Box->v0->y+lnb->Box->v6->y)/2,
                           (lnb->Box->v0->z+lnb->Box->v6->z)/2};
      
      float trivert[3][3] = {
        {ObjectVertexes[ObjectTriangles[i].a].x,ObjectVertexes[ObjectTriangles[i].a].y,ObjectVertexes[ObjectTriangles[i].a].z},
        {ObjectVertexes[ObjectTriangles[i].b].x,ObjectVertexes[ObjectTriangles[i].b].y,ObjectVertexes[ObjectTriangles[i].b].z},
        {ObjectVertexes[ObjectTriangles[i].c].x,ObjectVertexes[ObjectTriangles[i].c].y,ObjectVertexes[ObjectTriangles[i].c].z},
        };

      if(triAABBBoxOverlap(BoxCenter.v,lnb->Box->Extents.v,trivert))
      {
        int bFound=0;
        ENG3D_LIST_ENTRY * Temp;
  
        for(Temp=((tBoundingBoxClientData*)lnb->Box->ClientData)->t.Flink;
            Temp!=&((tBoundingBoxClientData*)lnb->Box->ClientData)->t;
            Temp=Temp->Flink)
        {
          tTrianglesList * t;
          t = ENG3D_BASE_FROM_FIELD(tTrianglesList,Link,Temp);
          if(t->t==&ObjectTriangles[i])
          {
            bFound=1;
            break;
          }
        }

        if(!bFound)
        {
          //the triangle is crossing the box and its not in the list
          tTrianglesList * t=new tTrianglesList;
          t->t=&ObjectTriangles[i];
          Eng3dPostInsertEntry(&((tBoundingBoxClientData*)lnb->Box->ClientData)->t,&t->Link);
        }
      }
    }
  }
}

void C3DSolidObject::EmptyCollisionInfoList()
{
  while(!Eng3dIsListEmpty(&CollisionInfoList))
  {
    ENG3D_LIST_ENTRY * Next;
    Next=CollisionInfoList.Flink;
    Eng3dExtractEntry(Next);
    tCollisionInfo * ci = ENG3D_BASE_FROM_FIELD(tCollisionInfo,Link,Next);
    delete ci;
  }  
}

void C3DSolidObject::EmptyLeafNodeBoundingBoxesList()
{
  while(!Eng3dIsListEmpty(&LeafNodeBoundingBoxesList))
  {
    ENG3D_LIST_ENTRY * Next;
    Next=LeafNodeBoundingBoxesList.Flink;
    Eng3dExtractEntry(Next);
    tLeafNodeBoxList * lnb = ENG3D_BASE_FROM_FIELD(tLeafNodeBoxList,Link,Next);
    delete lnb;
  }  
}

C3DSolidObject::C3DSolidObject()
{
  Eng3dInitList(&LeafNodeBoundingBoxesList);
  Eng3dInitList(&CollisionInfoList);
  MaxX=NULL;
  MaxY=NULL;
  MaxZ=NULL;
  MinX=NULL;
  MinY=NULL;
  MinZ=NULL;
  BBox=NULL;
  BBoxNeedToBeUpdated=1;
}

C3DSolidObject::~C3DSolidObject()
{
  EmptyLeafNodeBoundingBoxesList();
  EmptyCollisionInfoList();
  if(BBox)delete BBox;
}

void C3DSolidObject::VertexesChangesCallbackC3DSolidObject(TVERTEXES_CHANGE_ID ChangeId,TVertex * ChangedVertex,void * ChangeParams)
{
  switch(ChangeId)
  {
    case VertexChangesObjectInitializedEvent:
      {
        unsigned int i=0;
        MaxX = MaxY = MaxZ = MinX = MinY = MinZ = &ObjectVertexes[0];
        for(i=0;i<nObjectVertexes;i++)
        {
          if(ObjectVertexes[i].x > MaxX->x)
            MaxX = &ObjectVertexes[i];
          if(ObjectVertexes[i].y > MaxY->y)
            MaxY = &ObjectVertexes[i];
          if(ObjectVertexes[i].z > MaxZ->z)
            MaxZ = &ObjectVertexes[i];
          if(ObjectVertexes[i].x < MinX->x)
            MinX = &ObjectVertexes[i];
          if(ObjectVertexes[i].y < MinY->y)
            MinY = &ObjectVertexes[i];
          if(ObjectVertexes[i].z < MinZ->z)
            MinZ = &ObjectVertexes[i];
        } 
      }
      UpdateAABBTree();
      break;
    
      //Its not necesary to update bounding boxes here.
      //We can update bounding box when we try to detect a collision.
      //However we will keep a flag indicating if the object changed. If the object
      //was not changed we will not need to update the box when we was trying to detect collisions
      case MatrixAppliedEvent:
      case RevertToSnapshotEvent:
        BBoxNeedToBeUpdated=1;
      break;
  }

  VertexesChangesCallbackC3DObject(ChangeId,ChangedVertex,ChangeParams); //call parent class's changes callback
}

void C3DSolidObject::VertexesChangesCallback(TVERTEXES_CHANGE_ID ChangeId,TVertex * ChangedVertex,void * ChangeParams)
{
  VertexesChangesCallbackC3DSolidObject(ChangeId,ChangedVertex,ChangeParams);
}


/*
  Here is a alternative code for the function BoundingBoxAskMoreSplitCallback when multiple vertexes are found
  into the box. It split the box in the middle point of 2 vertexes found. But its better to let the C3DBox class
  to split the box by the middle of the larger dimension of the box.


    //we will set the split points for the box, because if we dont it the box will be splitted
    //by the half of the largest dimension of the box.
    
    //note here we are assuming the box is aligned with the xyz axis (we can assume it because
    //this is a solid object and here the bounding box is being calculated: this occurs when
    //this solid object is instantiated).

    float LenH;
    float LenV;
    float LenD;
    TVertex UnitH;
    TVertex UnitV;
    TVertex UnitD;
    TVertex vMid0;
    TVertex vMid1;
    TVertex vMid2;
    TVertex vMid3;
    unsigned int SplitDir=-1;

    vMid0.x = Box->v1->x - Box->v0->x;
    vMid0.y = Box->v1->y - Box->v0->y;
    vMid0.z = Box->v1->z - Box->v0->z;

    vMid1.x = Box->v6->x - Box->v2->x;
    vMid1.y = Box->v6->y - Box->v2->y;
    vMid1.z = Box->v6->z - Box->v2->z;

    vMid2.x = Box->v3->x - Box->v0->x;
    vMid2.y = Box->v3->y - Box->v0->y;
    vMid2.z = Box->v3->z - Box->v0->z;

    v_normalize(vMid0,LenH,UnitH);
    v_normalize(vMid1,LenV,UnitV);
    v_normalize(vMid2,LenD,UnitD);

    //note we must have in mind this: if our algorithm says we must cut by a direction we must
    //check first if the found vertexes have different x,y or z (the coord. of that direction).
    //I mean: if we cunt horizontally but the vertexes have the same x, the will have a box
    //without size. In these cases we will cut in other direction.
    while(1)
    {
      if(SplitDir==1 || (SplitDir==-1 && LenH >= LenV && LenH >= LenD)) //cut horizontally
      {
        if(fabs(FoundVertexes[0]->x - FoundVertexes[1]->x)<5e-6)
        {
          //use other direction
          SplitDir=2;
          continue;
        }

        LenH = (fabs(FoundVertexes[0]->x - Box->v0->x) + fabs(FoundVertexes[1]->x - Box->v0->x))/2;

        vMid0.x = Box->v0->x + LenH*UnitH.x;
        vMid0.y = Box->v0->y + LenH*UnitH.y;
        vMid0.z = Box->v0->z + LenH*UnitH.z;

        vMid1.x = Box->v3->x + LenH*UnitH.x;
        vMid1.y = Box->v3->y + LenH*UnitH.y;
        vMid1.z = Box->v3->z + LenH*UnitH.z;

        vMid2.x = Box->v7->x + LenH*UnitH.x;
        vMid2.y = Box->v7->y + LenH*UnitH.y;
        vMid2.z = Box->v7->z + LenH*UnitH.z;

        vMid3.x = Box->v4->x + LenH*UnitH.x;
        vMid3.y = Box->v4->y + LenH*UnitH.y;
        vMid3.z = Box->v4->z + LenH*UnitH.z;

        Box->SetSplitPoints(&vMid0,&vMid1,&vMid2,&vMid3,1);
        break;
      }
      else if(SplitDir==2 || (SplitDir==-1 && LenV >= LenH && LenV >= LenD)) //cut vertically
      {
        if(fabs(FoundVertexes[0]->y - FoundVertexes[1]->y)<5e-6)
        {
          //use other direction
          SplitDir=3;
          continue;
        }

        LenV = (fabs(FoundVertexes[0]->y - Box->v2->y) + fabs(FoundVertexes[1]->y - Box->v2->y))/2;

        vMid0.x = Box->v0->x + LenV*UnitV.x;
        vMid0.y = Box->v0->y + LenV*UnitV.y;
        vMid0.z = Box->v0->z + LenV*UnitV.z;

        vMid1.x = Box->v1->x + LenV*UnitV.x;
        vMid1.y = Box->v1->y + LenV*UnitV.y;
        vMid1.z = Box->v1->z + LenV*UnitV.z;

        vMid2.x = Box->v2->x + LenV*UnitV.x;
        vMid2.y = Box->v2->y + LenV*UnitV.y;
        vMid2.z = Box->v2->z + LenV*UnitV.z;

        vMid3.x = Box->v3->x + LenV*UnitV.x;
        vMid3.y = Box->v3->y + LenV*UnitV.y;
        vMid3.z = Box->v3->z + LenV*UnitV.z;  

        Box->SetSplitPoints(&vMid0,&vMid1,&vMid2,&vMid3,2);
        break;
      }
      else //cut dephtly
      {
        if(fabs(FoundVertexes[0]->z - FoundVertexes[1]->z)<5e-6)
        {
          //use other direction
          SplitDir=1;
          continue;
        }

        LenD = (fabs(FoundVertexes[0]->z - Box->v0->z) + fabs(FoundVertexes[1]->z - Box->v0->z))/2;

        vMid0.x = Box->v0->x + LenD*UnitD.x;
        vMid0.y = Box->v0->y + LenD*UnitD.y;
        vMid0.z = Box->v0->z + LenD*UnitD.z;

        vMid1.x = Box->v4->x + LenD*UnitD.x;
        vMid1.y = Box->v4->y + LenD*UnitD.y;
        vMid1.z = Box->v4->z + LenD*UnitD.z;

        vMid2.x = Box->v5->x + LenD*UnitD.x;
        vMid2.y = Box->v5->y + LenD*UnitD.y;
        vMid2.z = Box->v5->z + LenD*UnitD.z;

        vMid3.x = Box->v1->x + LenD*UnitD.x;
        vMid3.y = Box->v1->y + LenD*UnitD.y;
        vMid3.z = Box->v1->z + LenD*UnitD.z;   

        Box->SetSplitPoints(&vMid0,&vMid1,&vMid2,&vMid3,3);
        break;
      }
    }

    return 1;
*/