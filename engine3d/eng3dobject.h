#ifndef __ENG3DOBJECT_H__
#define __ENG3DOBJECT_H__

#include <windows.h>
#include "eng3dvertex.h"
#include "eng3dpolygon.h"
#include "eng3ddefs.h"
#include "eng3dmatrix.h"
#include "eng3dcolor.h"
#include "eng3d3dsreader.h"

class C3DObject
{

public:

  C3DObject();
  ~C3DObject();
  
  ENG3DERR Initialize(tObjectInfo3ds * param3dsObjectInfo);

  void RotateOverWorldX(float Angle,unsigned int preApply=0);
  void RotateOverWorldY(float Angle,unsigned int preApply=0);
  void RotateOverWorldZ(float Angle,unsigned int preApply=0);
  void RotateOverVector(float Angle,TVertex * Orig,TVertex * Dest,unsigned int preApply=0);

  void Traslate(float x,float y,float z,unsigned int preApply=0);

  void GetObjectOrigin(TVertex * v);
  void GetObjectOrthoBasis(TVertex * o,TVertex * u,TVertex * v,TVertex * w);

  void GetWorldVertexNormal(unsigned int VertexIndex,TVertex * Normal);
  void GetWorldFaceNormal(unsigned int FaceIndex,TVertex * Normal);
  void ComputeNormals();

  int IsPointIntoObject(TVertex * P, unsigned int InWorld=1);

  void Snapshot();
  void RevertToSnapshot();

  tObjectInfo3ds * ObjectInfo3ds;

  TTriangle * ObjectTriangles;
  unsigned int nObjectTriangles;
  TVertex * ObjectVertexes;
  unsigned int nObjectVertexes;
  TVertex * FaceNormals;
  TVertex * VertexNormals;
  unsigned int * SmoothingGroupByVertex;

  void UpdateWorldObjectVertexes(); //this function will update the world coord. of vertexes
  TVertex * WorldObjectVertexes; //world coord. of vertexes

  MATRIX ObjectToWorldRotationsMatrix;
  MATRIX ObjectToWorldRotationsMatrixSnapshot;
  MATRIX ObjectToWorldMatrix;
  MATRIX ObjectToWorldMatrixSnapshot;

protected:

  typedef enum
  {
    VertexChangesObjectInitializedEvent,
    MatrixAppliedEvent,
    SnapshotEvent,
    RevertToSnapshotEvent
  } TVERTEXES_CHANGE_ID;

  void VertexesChangesCallbackC3DObject(TVERTEXES_CHANGE_ID ChangeId,
                                        TVertex * ChangedVertex,
                                        void * ChangeParams);

  virtual void VertexesChangesCallback(TVERTEXES_CHANGE_ID ChangeId,
                               TVertex * ChangedVertex,
                               void * ChangeParams);
  
  C3DMatrixesTool MatrixTool;
  
  unsigned int PendingObjectToWorldMatrixToBeApplied;

private:
  
  ENG3DERR DeleteDuplicates();
  ENG3DERR DuplicateVerticesWithDifferentSmoothingGroup();
};

#endif