#include "eng3dphysicobject.h"

#define MAX_SPLITTED_BOXES_DIMENSIONS 0.4f
#define MAX_MASS_BBOX_DIVISIONS 50

typedef struct
{
  ENG3D_LIST_ENTRY Link;
  C3DBox * Box;
}tSmallMassBoxListNode;

unsigned int C3DPhysicObject::AddBoxesInsideToLists(C3DBox * Box)
{
  //this box is a leaf node. Check if the box is inside the object. Note we have constructed
  //the bounding box with MinX,MaxX,MinY,MaxY,MinZ,MaxZ variables. These variables was calculated
  //with the initial position of the object. So we must check if the box is inside or outside the
  //initial object, before applying the world matrix.
    
  TVertex BoxCenter; 
  BoxCenter.x = Box->cv6.x - Box->cv0.x; 
  BoxCenter.y = Box->cv6.y - Box->cv0.y; 
  BoxCenter.z = Box->cv6.z - Box->cv0.z;

  if(IsPointIntoObject(&BoxCenter,0))
  {
    /*
    //if the box is inside the object add it to the sorted lists
    ENG3D_LIST_ENTRY * Temp;
    ENG3D_LIST_ENTRY * TempNext;
    tSmallMassBoxListNode * TempSmallMassBox;
    tSmallMassBoxListNode * TempNextSmallMassBox;
    
    //Insert to x list
    if(!Eng3dIsListEmpty(&XMass))
    {
      for(Temp=XMass.Flink; Temp!=&XMass; Temp=Temp->Flink)
      {
        TempNext=Temp->Flink;

        if(TempNext==&XMass)
        {
          tSmallMassBoxListNode * NewSmallMassBox = new tSmallMassBoxListNode;
          NewSmallMassBox->Box = Box;
          Eng3dPostInsertEntry(Temp,&NewSmallMassBox->Link);
          break;
        }
        else
        {
          TempSmallMassBox = ENG3D_BASE_FROM_FIELD(tSmallMassBoxListNode,Link,Temp);
          TempNextSmallMassBox = ENG3D_BASE_FROM_FIELD(tSmallMassBoxListNode,Link,TempNext);
          
          if(TempSmallMassBox->Box->cv0.x<=Box->cv0.x && 
             TempNextSmallMassBox->Box->cv0.x>=Box->cv0.x)
          {
            tSmallMassBoxListNode * NewSmallMassBox = new tSmallMassBoxListNode;
            NewSmallMassBox->Box = Box;
            Eng3dPostInsertEntry(Temp,&NewSmallMassBox->Link);
            break;
          }
        }
      }
    }
    else
    {
      tSmallMassBoxListNode * NewSmallMassBox = new tSmallMassBoxListNode;
      NewSmallMassBox->Box = Box;
      Eng3dPostInsertEntry(&XMass,&NewSmallMassBox->Link);      
    }

    //insert to y list
    if(!Eng3dIsListEmpty(&YMass))
    {
      for(Temp=YMass.Flink; Temp!=&YMass; Temp=Temp->Flink)
      {
        TempNext=Temp->Flink;

        if(TempNext==&YMass)
        {
          tSmallMassBoxListNode * NewSmallMassBox = new tSmallMassBoxListNode;
          NewSmallMassBox->Box = Box;
          Eng3dPostInsertEntry(Temp,&NewSmallMassBox->Link);
          break;
        }
        else
        {
          TempSmallMassBox = ENG3D_BASE_FROM_FIELD(tSmallMassBoxListNode,Link,Temp);
          TempNextSmallMassBox = ENG3D_BASE_FROM_FIELD(tSmallMassBoxListNode,Link,TempNext);
          
          if(TempSmallMassBox->Box->cv0.y<=Box->cv0.y && 
             TempNextSmallMassBox->Box->cv0.y>=Box->cv0.y)
          {
            tSmallMassBoxListNode * NewSmallMassBox = new tSmallMassBoxListNode;
            NewSmallMassBox->Box = Box;
            Eng3dPostInsertEntry(Temp,&NewSmallMassBox->Link);
            break;
          }
        }
      }
    }
    else
    {
      tSmallMassBoxListNode * NewSmallMassBox = new tSmallMassBoxListNode;
      NewSmallMassBox->Box = Box;
      Eng3dPostInsertEntry(&YMass,&NewSmallMassBox->Link);      
    }

    //insert to z list
    if(!Eng3dIsListEmpty(&ZMass))
    {
      for(Temp=ZMass.Flink; Temp!=&ZMass; Temp=Temp->Flink)
      {
        TempNext=Temp->Flink;

        if(TempNext==&ZMass)
        {
          tSmallMassBoxListNode * NewSmallMassBox = new tSmallMassBoxListNode;
          NewSmallMassBox->Box = Box;
          Eng3dPostInsertEntry(Temp,&NewSmallMassBox->Link);
          break;
        }
        else
        {
          TempSmallMassBox = ENG3D_BASE_FROM_FIELD(tSmallMassBoxListNode,Link,Temp);
          TempNextSmallMassBox = ENG3D_BASE_FROM_FIELD(tSmallMassBoxListNode,Link,TempNext);
          
          if(TempSmallMassBox->Box->cv3.z<=Box->cv3.z && 
             TempNextSmallMassBox->Box->cv3.z>=Box->cv3.z)
          {
            tSmallMassBoxListNode * NewSmallMassBox = new tSmallMassBoxListNode;
            NewSmallMassBox->Box = Box;
            Eng3dPostInsertEntry(Temp,&NewSmallMassBox->Link);
            break;
          }
        }
      }
    }
    else
    {
      tSmallMassBoxListNode * NewSmallMassBox = new tSmallMassBoxListNode;
      NewSmallMassBox->Box = Box;
      Eng3dPostInsertEntry(&ZMass,&NewSmallMassBox->Link);      
    }
    
    */
    return 1;
  }
  
  return 0;
}

void C3DPhysicObject::UpdateCenterOfMass()
{
  /*TVertex tv0,tv1,tv2,tv3,tv4,tv5,tv6,tv7;
  C3DBox * TempBBox;
  C3DBox * TempSmallBBox;

  //step 1: we create a bbox for the object.

  tv0.x = MinX->x; tv0.y = MinY->y; tv0.z = MaxZ->z;
  tv1.x = MaxX->x; tv1.y = MinY->y; tv1.z = MaxZ->z;
  tv2.x = MaxX->x; tv2.y = MinY->y; tv2.z = MinZ->z;
  tv3.x = MinX->x; tv3.y = MinY->y; tv3.z = MinZ->z;
  tv4.x = MinX->x; tv4.y = MaxY->y; tv4.z = MaxZ->z;
  tv5.x = MaxX->x; tv5.y = MaxY->y; tv5.z = MaxZ->z;
  tv6.x = MaxX->x; tv6.y = MaxY->y; tv6.z = MinZ->z;
  tv7.x = MinX->x; tv7.y = MaxY->y; tv7.z = MinZ->z;

  TempBBox = new C3DBox(&tv0,&tv1,&tv2,&tv3,&tv4,&tv5,&tv6,&tv7);
  
  //step 2: split the box. We want to split it into very small boxes. We will try to know if the
  //box is inside the object (aprox) by taking the center of the box and asking the object if that
  //point is inside of the object or outside of it.
  
  unsigned int i,j,k;
  unsigned int nx,ny,nz;
  float lx,ly,lz;

  nx=MAX_MASS_BBOX_DIVISIONS;//(TempBBox->Extents.x*2)/MAX_SPLITTED_BOXES_DIMENSIONS;
  ny=MAX_MASS_BBOX_DIVISIONS;//(TempBBox->Extents.y*2)/MAX_SPLITTED_BOXES_DIMENSIONS;
  nz=MAX_MASS_BBOX_DIVISIONS;//(TempBBox->Extents.z*2)/MAX_SPLITTED_BOXES_DIMENSIONS;
  lx=(TempBBox->Extents.x*2)/nx;
  ly=(TempBBox->Extents.y*2)/ny;
  lz=(TempBBox->Extents.z*2)/nz;

  for(i=0;i<nx;i++)
  {
    for(j=0;j<ny;j++)
    {
      for(k=0;k<nz;k++)
      {
        tv0.x=TempBBox->cv0.x+i*lx; tv0.y=TempBBox->cv0.y+j*ly; tv0.z=TempBBox->cv0.z-k*lz;
        tv1.x=tv0.x+lx; tv1.y=tv0.y; tv0.z=tv0.z;
        tv2.x=tv0.x+lx; tv2.y=tv0.y; tv2.z=tv0.z-lz;
        tv3.x=tv0.x; tv3.y=tv0.y; tv3.z=tv0.z-lz;
        tv4.x=tv0.x; tv4.y=tv0.y+ly;tv4.z=tv0.z;
        tv5.x=tv0.x+lx; tv5.y=tv0.y+ly; tv5.z=tv0.z;
        tv6.x=tv0.x+lx; tv6.y=tv0.y+ly; tv6.z=tv0.z-lz;
        tv7.x=tv0.x; tv7.y=tv0.y+ly; tv7.z=tv0.z-lz;
        TempSmallBBox = new C3DBox(&tv0,&tv1,&tv2,&tv3,&tv4,&tv5,&tv6,&tv7);
        if(!AddBoxesInsideToLists(TempSmallBBox))
          delete TempSmallBBox;
      }
    }
  }*/
}

C3DPhysicObject::C3DPhysicObject()
{
  Eng3dInitList(&XMass);
  Eng3dInitList(&YMass);
  Eng3dInitList(&ZMass);
}

C3DPhysicObject::~C3DPhysicObject()
{

}

void C3DPhysicObject::VertexesChangesCallbackC3DPhysicObject(TVERTEXES_CHANGE_ID ChangeId,
                                                             TVertex * ChangedVertex,
                                                             void * ChangeParams)
{
  VertexesChangesCallbackC3DSolidObject(ChangeId,ChangedVertex,ChangeParams);

  switch(ChangeId)
  {
    case VertexChangesObjectInitializedEvent:
      UpdateCenterOfMass();
    break;

    case MatrixAppliedEvent:
    case RevertToSnapshotEvent:
      //here we must update the MassCenter in world
    break;

  };
}

void C3DPhysicObject::VertexesChangesCallback(TVERTEXES_CHANGE_ID ChangeId,
                                              TVertex * ChangedVertex,
                                              void * ChangeParams)
{
  VertexesChangesCallbackC3DPhysicObject(ChangeId,ChangedVertex,ChangeParams);
}