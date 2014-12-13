#include "eng3dobject.h"
#include "eng3dmatrix.h"
#include "eng3dray.h"
#include "eng3dvector.h"
#include <math.h>

C3DObject::C3DObject()
{
  ObjectToWorldMatrixSnapshot=IdentityMatrix;
  ObjectToWorldMatrix=IdentityMatrix;
  ObjectToWorldRotationsMatrix=IdentityMatrix;
  ObjectToWorldRotationsMatrixSnapshot=IdentityMatrix;
  WorldObjectVertexes=NULL;
  PendingObjectToWorldMatrixToBeApplied=1;
  FaceNormals=NULL;
  VertexNormals=NULL;
  SmoothingGroupByVertex = NULL;
}

C3DObject::~C3DObject()
{
  if(VertexNormals)
    delete VertexNormals;
  if(WorldObjectVertexes)
    delete WorldObjectVertexes;
  if(FaceNormals)
    delete FaceNormals;
}

void C3DObject::ComputeNormals()
{
  unsigned int i,j;

  //face normals
  for(i=0;i<nObjectTriangles;i++)
  {
    TVertex NormalsTempab,NormalsTempac;
    NormalsTempab.x=ObjectVertexes[ObjectTriangles[i].b].x-ObjectVertexes[ObjectTriangles[i].a].x;
    NormalsTempab.y=ObjectVertexes[ObjectTriangles[i].b].y-ObjectVertexes[ObjectTriangles[i].a].y;
    NormalsTempab.z=ObjectVertexes[ObjectTriangles[i].b].z-ObjectVertexes[ObjectTriangles[i].a].z;
    NormalsTempac.x=ObjectVertexes[ObjectTriangles[i].c].x-ObjectVertexes[ObjectTriangles[i].a].x;
    NormalsTempac.y=ObjectVertexes[ObjectTriangles[i].c].y-ObjectVertexes[ObjectTriangles[i].a].y;
    NormalsTempac.z=ObjectVertexes[ObjectTriangles[i].c].z-ObjectVertexes[ObjectTriangles[i].a].z;
    
    float mag;
    v_cross_product(NormalsTempab,NormalsTempac,FaceNormals[i]);
    v_normalize(FaceNormals[i],mag,FaceNormals[i]);
  }

  //vertex normals
  memset(VertexNormals,0,sizeof(TVertex)*nObjectVertexes);
  for(i=0;i<nObjectTriangles;i++)
  {
    for(j=0;j<nObjectVertexes;j++)
    {
      if(ObjectInfo3ds->SmoothingGroups)
      {
        if(ObjectInfo3ds->SmoothingGroups[i]==SmoothingGroupByVertex[j] &&
           (ObjectTriangles[i].a==j||ObjectTriangles[i].b==j||ObjectTriangles[i].c==j))
        {
          VertexNormals[j].x+=FaceNormals[i].x;
          VertexNormals[j].y+=FaceNormals[i].y;
          VertexNormals[j].z+=FaceNormals[i].z;
        }
      }
      else
      {
        if(ObjectTriangles[i].a==j||ObjectTriangles[i].b==j||ObjectTriangles[i].c==j)
        {
          VertexNormals[j].x+=FaceNormals[i].x;
          VertexNormals[j].y+=FaceNormals[i].y;
          VertexNormals[j].z+=FaceNormals[i].z;
        }
      }
    }
  }

  for(i=0;i<nObjectVertexes;i++)
  {
    float mag;
    v_normalize(VertexNormals[i],mag,VertexNormals[i]);
  }
}

void C3DObject::GetWorldFaceNormal(unsigned int FaceIndex,TVertex * Normal)
{
  UpdateWorldObjectVertexes();
  C3DMatrixesTool::ApplyMatrix(&ObjectToWorldMatrix,
                               ObjectVertexes[ObjectTriangles[FaceIndex].a].x+FaceNormals[FaceIndex].x,
                               ObjectVertexes[ObjectTriangles[FaceIndex].a].y+FaceNormals[FaceIndex].y,
                               ObjectVertexes[ObjectTriangles[FaceIndex].a].z+FaceNormals[FaceIndex].z,
                               &Normal->x,
                               &Normal->y,
                               &Normal->z);
  Normal->x-=WorldObjectVertexes[ObjectTriangles[FaceIndex].a].x;
  Normal->y-=WorldObjectVertexes[ObjectTriangles[FaceIndex].a].y;
  Normal->z-=WorldObjectVertexes[ObjectTriangles[FaceIndex].a].z;
}

void C3DObject::GetWorldVertexNormal(unsigned int VertexIndex,TVertex * Normal)
{
  UpdateWorldObjectVertexes();
  C3DMatrixesTool::ApplyMatrix(&ObjectToWorldMatrix,
                               ObjectVertexes[VertexIndex].x+VertexNormals[VertexIndex].x,
                               ObjectVertexes[VertexIndex].y+VertexNormals[VertexIndex].y,
                               ObjectVertexes[VertexIndex].z+VertexNormals[VertexIndex].z,
                               &Normal->x,
                               &Normal->y,
                               &Normal->z);
  Normal->x-=WorldObjectVertexes[VertexIndex].x;
  Normal->y-=WorldObjectVertexes[VertexIndex].y;
  Normal->z-=WorldObjectVertexes[VertexIndex].z;
}

ENG3DERR C3DObject::DeleteDuplicates()
{
  unsigned int i=0,j=0,k=0;
  TVertex * ObjectVertexesNoDup=new TVertex[nObjectVertexes];
  TVertex * Temp;

  //delete vertex duplications
  for(i=0;i<nObjectVertexes;i++)
  {
    for(j=0;j<k;j++)
    {
      if(ObjectVertexes[i].x-ObjectVertexesNoDup[j].x==0&&
         ObjectVertexes[i].y-ObjectVertexesNoDup[j].y==0&&
         ObjectVertexes[i].z-ObjectVertexesNoDup[j].z==0)
      {
        break;
      }
    }

    if(j==k)
    {
      //add the vertex because its not repeated
      ObjectVertexesNoDup[k].x=ObjectVertexes[i].x;
      ObjectVertexesNoDup[k].y=ObjectVertexes[i].y;
      ObjectVertexesNoDup[k].z=ObjectVertexes[i].z;

      //search triangles using this vertex and change it to point the no dup one
      unsigned int t=0;
      for(t=0;t<nObjectTriangles;t++)
      {
        if(ObjectTriangles[t].a==i)ObjectTriangles[t].a=k;
        if(ObjectTriangles[t].b==i)ObjectTriangles[t].b=k;
        if(ObjectTriangles[t].c==i)ObjectTriangles[t].c=k;
      }
      k++;
    }
    else
    {
      //search triangles using this vertex and change it to point the no dup one
      unsigned int t=0;
      for(t=0;t<nObjectTriangles;t++)
      {
        if(ObjectTriangles[t].a==i)ObjectTriangles[t].a=j;
        if(ObjectTriangles[t].b==i)ObjectTriangles[t].b=j;
        if(ObjectTriangles[t].c==i)ObjectTriangles[t].c=j;
      }
    }
  }

  nObjectVertexes=k;
  if(ObjectInfo3ds)ObjectInfo3ds->nObjectVertexes=k;
  Temp=ObjectVertexes;
  ObjectVertexes=ObjectVertexesNoDup; //set the array with no dups
  delete Temp; //delete old array

  return ENG3DERR_NO_ERROR;
}

typedef struct
{
  ENG3D_LIST_ENTRY Link;
  unsigned int SmoothingGroup;
}tVertexSmoothingGroups;

ENG3DERR C3DObject::DuplicateVerticesWithDifferentSmoothingGroup()
{
  unsigned int i=0,j=0,k=0;

  if(!ObjectInfo3ds->SmoothingGroups)
    return ENG3DERR_NO_ERROR;



  ENG3D_LIST_ENTRY * VerticesSmoothingGroups=new ENG3D_LIST_ENTRY[nObjectVertexes];

  for(i=0;i<nObjectVertexes;i++)
  {
    Eng3dInitList(&VerticesSmoothingGroups[i]);

    for(j=0;j<nObjectTriangles;j++)
    {
      if(ObjectTriangles[j].a==i||
         ObjectTriangles[j].b==i||
         ObjectTriangles[j].c==i)
      {
        ENG3D_LIST_ENTRY * Temp;
        for(Temp=VerticesSmoothingGroups[i].Flink;Temp!=&VerticesSmoothingGroups[i];Temp=Temp->Flink)
        {
          tVertexSmoothingGroups * TempVertexSmoothingGroups = ENG3D_BASE_FROM_FIELD(tVertexSmoothingGroups,Link,Temp);
          if(TempVertexSmoothingGroups->SmoothingGroup==ObjectInfo3ds->SmoothingGroups[j])
          {
            break;
          }
        }
        
        if(Temp==&VerticesSmoothingGroups[i])
        {
          tVertexSmoothingGroups * sg = new tVertexSmoothingGroups;
          sg->SmoothingGroup=ObjectInfo3ds->SmoothingGroups[j];
          Eng3dPreInsertEntry(&VerticesSmoothingGroups[i],&sg->Link);
        }
      }
    }
  }
  
  unsigned int TotalVertexesToDup=0;

  for(i=0;i<nObjectVertexes;i++)
  {
    ENG3D_LIST_ENTRY * Temp;
    for(Temp=VerticesSmoothingGroups[i].Flink;Temp!=&VerticesSmoothingGroups[i];Temp=Temp->Flink)
    {
      tVertexSmoothingGroups * TempVertexSmoothingGroups = ENG3D_BASE_FROM_FIELD(tVertexSmoothingGroups,Link,Temp);
      TotalVertexesToDup++;
    }  
  }

  TVertex * NewVertexes;
  
  if(TotalVertexesToDup)
  {
    NewVertexes = new TVertex[nObjectVertexes+TotalVertexesToDup];
    TTriangle * NewTriangles = new TTriangle[nObjectTriangles];
    memcpy(NewTriangles,ObjectTriangles,sizeof(TTriangle)*nObjectTriangles);

    for(i=0,j=0;i<nObjectVertexes;i++)
    {
      ENG3D_LIST_ENTRY * Temp;
      for(Temp=VerticesSmoothingGroups[i].Flink;Temp!=&VerticesSmoothingGroups[i];Temp=Temp->Flink)
      {
        tVertexSmoothingGroups * TempVertexSmoothingGroups = ENG3D_BASE_FROM_FIELD(tVertexSmoothingGroups,Link,Temp);
        
        NewVertexes[j].x=ObjectVertexes[i].x;
        NewVertexes[j].y=ObjectVertexes[i].y;
        NewVertexes[j].z=ObjectVertexes[i].z;

        for(k=0;k<nObjectTriangles;k++)
        {
          if(TempVertexSmoothingGroups->SmoothingGroup==ObjectInfo3ds->SmoothingGroups[k])
          {
            if(ObjectTriangles[k].a==i)NewTriangles[k].a=j;
            if(ObjectTriangles[k].b==i)NewTriangles[k].b=j;
            if(ObjectTriangles[k].c==i)NewTriangles[k].c=j;
          }
        }

        j++;
      }  
    }

    TTriangle * Temp = ObjectTriangles;
    ObjectTriangles = NewTriangles;
    delete Temp;
  }

  SmoothingGroupByVertex = new unsigned int[nObjectVertexes+TotalVertexesToDup];

  for(i=0,j=0;i<nObjectVertexes;i++)
  {
    ENG3D_LIST_ENTRY * Temp;

    for(Temp=VerticesSmoothingGroups[i].Flink;Temp!=&VerticesSmoothingGroups[i];Temp=VerticesSmoothingGroups[i].Flink)
    {
      tVertexSmoothingGroups * TempVertexSmoothingGroups = ENG3D_BASE_FROM_FIELD(tVertexSmoothingGroups,Link,Temp);
      Eng3dExtractEntry(&TempVertexSmoothingGroups->Link);
      SmoothingGroupByVertex[j]=TempVertexSmoothingGroups->SmoothingGroup;j++;
      delete TempVertexSmoothingGroups;
    }  
  }

  if(TotalVertexesToDup)
  {
    TVertex * Temp = ObjectVertexes;
    ObjectVertexes = NewVertexes;
    nObjectVertexes += TotalVertexesToDup;
    delete Temp;
  }

  delete VerticesSmoothingGroups;

  return ENG3DERR_NO_ERROR;
}

ENG3DERR C3DObject::Initialize(tObjectInfo3ds * paramObjectInfo3ds)
{
  ObjectInfo3ds=paramObjectInfo3ds;
  ObjectTriangles = ObjectInfo3ds->ObjectTriangles;
  nObjectTriangles = ObjectInfo3ds->nObjectTriangles;
  ObjectVertexes = ObjectInfo3ds->ObjectVertexes;
  nObjectVertexes = ObjectInfo3ds->nObjectVertexes;

  char * av = new char [1000000];

  DeleteDuplicates();
  DuplicateVerticesWithDifferentSmoothingGroup();

  VertexNormals = new TVertex[nObjectVertexes];
  if(!VertexNormals)
    return ENG3DERR_MEMORY_ERROR;
  WorldObjectVertexes=new TVertex[nObjectVertexes];
  if(!WorldObjectVertexes)
    return ENG3DERR_MEMORY_ERROR;
  FaceNormals=new TVertex[nObjectTriangles];
  if(!FaceNormals)
    return ENG3DERR_MEMORY_ERROR;

  ComputeNormals();
  
  VertexesChangesCallback(VertexChangesObjectInitializedEvent,NULL,NULL);
  return ENG3DERR_NO_ERROR;
}

void C3DObject::UpdateWorldObjectVertexes()
{
  if(PendingObjectToWorldMatrixToBeApplied)
  {
    unsigned int i=0;
    for(i=0;i<nObjectVertexes;i++)
    {
      C3DMatrixesTool::ApplyMatrix(&ObjectToWorldMatrix,
                                   ObjectVertexes[i].x,ObjectVertexes[i].y,ObjectVertexes[i].z,
                                   &WorldObjectVertexes[i].x,&WorldObjectVertexes[i].y,&WorldObjectVertexes[i].z);
    } 
    PendingObjectToWorldMatrixToBeApplied=0;
  }
}

void C3DObject::RotateOverWorldX(float Angle,unsigned int preApply)
{
  MatrixTool.RotationXMatrix(Angle);
  MatrixTool.AuxMatrix=ObjectToWorldMatrix;
  if(preApply)
  {
    MatrixTool.MultMatrixes(&MatrixTool.AuxMatrix,&ObjectToWorldMatrix);
    MatrixTool.AuxMatrix=ObjectToWorldRotationsMatrix;
    MatrixTool.MultMatrixes(&MatrixTool.AuxMatrix,&ObjectToWorldRotationsMatrix);
  }
  else
  {
    MatrixTool.MultMatrixes(&MatrixTool.AuxMatrix,&MatrixTool.Matrix,&ObjectToWorldMatrix);
    MatrixTool.AuxMatrix=ObjectToWorldRotationsMatrix;
    MatrixTool.MultMatrixes(&MatrixTool.AuxMatrix,&MatrixTool.Matrix,&ObjectToWorldRotationsMatrix);
  }
  PendingObjectToWorldMatrixToBeApplied=1;
  VertexesChangesCallback(MatrixAppliedEvent,NULL,NULL);
}

void C3DObject::RotateOverWorldY(float Angle,unsigned int preApply)
{
  MatrixTool.RotationYMatrix(Angle);
  MatrixTool.AuxMatrix=ObjectToWorldMatrix;
  if(preApply)
  {
    MatrixTool.MultMatrixes(&MatrixTool.AuxMatrix,&ObjectToWorldMatrix);
    MatrixTool.AuxMatrix=ObjectToWorldRotationsMatrix;
    MatrixTool.MultMatrixes(&MatrixTool.AuxMatrix,&ObjectToWorldRotationsMatrix);
  }
  else
  {
    MatrixTool.MultMatrixes(&MatrixTool.AuxMatrix,&MatrixTool.Matrix,&ObjectToWorldMatrix);
    MatrixTool.AuxMatrix=ObjectToWorldRotationsMatrix;
    MatrixTool.MultMatrixes(&MatrixTool.AuxMatrix,&MatrixTool.Matrix,&ObjectToWorldRotationsMatrix);
  }
  PendingObjectToWorldMatrixToBeApplied=1;
  VertexesChangesCallback(MatrixAppliedEvent,NULL,NULL);
}

void C3DObject::RotateOverWorldZ(float Angle,unsigned int preApply)
{
  MatrixTool.RotationZMatrix(Angle);
  MatrixTool.AuxMatrix=ObjectToWorldMatrix;
  if(preApply)
  {
    MatrixTool.MultMatrixes(&MatrixTool.AuxMatrix,&ObjectToWorldMatrix);
    MatrixTool.AuxMatrix=ObjectToWorldRotationsMatrix;
    MatrixTool.MultMatrixes(&MatrixTool.AuxMatrix,&ObjectToWorldRotationsMatrix);
  }
  else
  {
    MatrixTool.MultMatrixes(&MatrixTool.AuxMatrix,&MatrixTool.Matrix,&ObjectToWorldMatrix);
    MatrixTool.AuxMatrix=ObjectToWorldRotationsMatrix;
    MatrixTool.MultMatrixes(&MatrixTool.AuxMatrix,&MatrixTool.Matrix,&ObjectToWorldRotationsMatrix);
  }
  PendingObjectToWorldMatrixToBeApplied=1;
  VertexesChangesCallback(MatrixAppliedEvent,NULL,NULL);
}

//positive value for Angle means counterclockwise rotation
void C3DObject::RotateOverVector(float Angle,TVertex * Orig,TVertex * Dest,unsigned int preApply)
{
  MATRIX m;
  MatrixTool.RotateOverVectorMatrix(&m,Angle,Orig->x,Orig->y,Orig->z,Dest->x,Dest->y,Dest->z);
  MatrixTool.AuxMatrix=ObjectToWorldMatrix;
  if(preApply)
  {
    MatrixTool.MultMatrixes(&m,&MatrixTool.AuxMatrix,&ObjectToWorldMatrix);
    MatrixTool.AuxMatrix=ObjectToWorldRotationsMatrix;
    MatrixTool.MultMatrixes(&m,&MatrixTool.AuxMatrix,&ObjectToWorldRotationsMatrix);
  }
  else
  {
    MatrixTool.MultMatrixes(&MatrixTool.AuxMatrix,&m,&ObjectToWorldMatrix);
    MatrixTool.AuxMatrix=ObjectToWorldRotationsMatrix;
    MatrixTool.MultMatrixes(&MatrixTool.AuxMatrix,&m,&ObjectToWorldRotationsMatrix);
  }
  PendingObjectToWorldMatrixToBeApplied=1;
  VertexesChangesCallback(MatrixAppliedEvent,NULL,NULL);
}

void C3DObject::Traslate(float x,float y,float z,unsigned int preApply)
{
  MatrixTool.TraslationMatrix(x,y,z);
  MatrixTool.AuxMatrix=ObjectToWorldMatrix;
  if(preApply)
    MatrixTool.MultMatrixes(&MatrixTool.AuxMatrix,&ObjectToWorldMatrix);
  else
    MatrixTool.MultMatrixes(&MatrixTool.AuxMatrix,&MatrixTool.Matrix,&ObjectToWorldMatrix);
  PendingObjectToWorldMatrixToBeApplied=1;
  VertexesChangesCallback(MatrixAppliedEvent,NULL,NULL);
}

void C3DObject::GetObjectOrigin(TVertex * v)
{
  MatrixTool.ApplyMatrix(&ObjectToWorldMatrix,0,0,0,&v->x,&v->y,&v->z);
}

void C3DObject::GetObjectOrthoBasis(TVertex * o,TVertex * u,TVertex * v,TVertex * w)
{
  MatrixTool.ApplyMatrix(&ObjectToWorldMatrix,0,0,0,&o->x,&o->y,&o->z);
  MatrixTool.ApplyMatrix(&ObjectToWorldMatrix,1,0,0,&u->x,&u->y,&u->z);
  MatrixTool.ApplyMatrix(&ObjectToWorldMatrix,0,1,0,&v->x,&v->y,&v->z);
  MatrixTool.ApplyMatrix(&ObjectToWorldMatrix,0,0,1,&w->x,&w->y,&w->z);
  u->x-=o->x;u->y-=o->y;u->z-=o->z;
  v->x-=o->x;v->y-=o->y;v->z-=o->z;
  w->x-=o->x;w->y-=o->y;w->z-=o->z;
}

int C3DObject::IsPointIntoObject(TVertex * P, unsigned int InWorld)
{
  //method: we will simulate a ray from P in a random direction. 
  //If the ray intersect a odd number of triangles the point is
  //into the object. We have a problem here. In rare cases the 
  //ray could intersect the triangle by a edge, or by a vertex.
  //In that case we will count 2 triangle intersections (the edge
  //will be part of the current triangle and other one) but our point
  //is into the object.
  //For this case, we will analyse if the point of intersection is 
  //part of a edge or a vertex. In that case we will rotate a few the 
  //ray, avoiding edge and vertex intersections.

  TVertex * ObjectVertexesToUse;
  int ires=0;
  unsigned int IntersectingTriangles=0;
  unsigned int i=0;
  TVertex I;
  TVertex PR = {P->x+1,P->y,P->z};
  C3DRay R(P,&PR,0);

  if(InWorld)
  {
    UpdateWorldObjectVertexes();  
    ObjectVertexesToUse=WorldObjectVertexes;
  }
  else
  {
    ObjectVertexesToUse=ObjectVertexes;
  }

  for(i=0;i<nObjectTriangles;i++)
  {
    ires=R.IntersectRayTriangle(&ObjectVertexesToUse[ObjectTriangles[i].a],
                                &ObjectVertexesToUse[ObjectTriangles[i].b],
                                &ObjectVertexesToUse[ObjectTriangles[i].c],
                                &I);
    
    if(
         IsVertexIntoSegment(&ObjectVertexesToUse[ObjectTriangles[i].a],
                             &ObjectVertexesToUse[ObjectTriangles[i].b], 
                             &I) ||
         IsVertexIntoSegment(&ObjectVertexesToUse[ObjectTriangles[i].a],
                             &ObjectVertexesToUse[ObjectTriangles[i].c], 
                             &I) ||
         IsVertexIntoSegment(&ObjectVertexesToUse[ObjectTriangles[i].b],
                             &ObjectVertexesToUse[ObjectTriangles[i].c], 
                             &I)
      )
    {
      i=0;PR.y+=0.01f;IntersectingTriangles=0;
      continue;
    }

    if(ires==1)
    {
      IntersectingTriangles++;
    }
  }

  if(IntersectingTriangles%2)
  {
    return 1; //the ray interesect a unique triangle or a odd number of them (the ray leaves, enters, leaves.. the object)
              //so, its into the object
  }
  else
  {
    return 0;
  }
}

void C3DObject::Snapshot()
{
  ObjectToWorldRotationsMatrixSnapshot=ObjectToWorldRotationsMatrix;
  ObjectToWorldMatrixSnapshot=ObjectToWorldMatrix;
  VertexesChangesCallback(SnapshotEvent,NULL,NULL);
}

void C3DObject::RevertToSnapshot()
{
  ObjectToWorldRotationsMatrix=ObjectToWorldRotationsMatrixSnapshot;
  ObjectToWorldMatrix=ObjectToWorldMatrixSnapshot;
  PendingObjectToWorldMatrixToBeApplied=1;
  VertexesChangesCallback(RevertToSnapshotEvent,NULL,NULL);
}

void C3DObject::VertexesChangesCallbackC3DObject(TVERTEXES_CHANGE_ID ChangeId,TVertex * ChangedVertex,void * ChangeParams)
{

}

void C3DObject::VertexesChangesCallback(TVERTEXES_CHANGE_ID ChangeId,TVertex * ChangedVertex,void * ChangeParams)
{
  VertexesChangesCallbackC3DObject(ChangeId,ChangedVertex,ChangeParams);
}