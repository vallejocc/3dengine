#include "eng3dcamera.h"
#include "eng3dmatrix.h"
#include "eng3dvector.h"
#include <math.h>

void C3DCamera::UpdateVectors()
{
  float mag;

  TempUnitDest.x = CurCameraDest->x - CurCameraOrig->x;
  TempUnitDest.y = CurCameraDest->y - CurCameraOrig->y;
  TempUnitDest.z = CurCameraDest->z - CurCameraOrig->z;
  
  v_normalize(TempUnitDest,mag,TempUnitDest);

  /*TempUnitDest.x = -TempUnitDest.x;
  TempUnitDest.y = -TempUnitDest.y;
  TempUnitDest.z = -TempUnitDest.z;*/

  TempUnitUp.x = CurCameraUp->x - CurCameraOrig->x;
  TempUnitUp.y = CurCameraUp->y - CurCameraOrig->y;
  TempUnitUp.z = CurCameraUp->z - CurCameraOrig->z;

  v_normalize(TempUnitUp,mag,TempUnitUp);

  v_cross_product(TempUnitDest,TempUnitUp,CameraUVector);

  v_normalize(CameraUVector,mag,CameraUVector);

  MATRIX m = 
  {
    {
      {CameraUVector.x,TempUnitUp.x,TempUnitDest.x,0},
      {CameraUVector.y,TempUnitUp.y,TempUnitDest.y,0},
      {CameraUVector.z,TempUnitUp.z,TempUnitDest.z,0},
      {0-(CameraUVector.x*CurCameraOrig->x+CameraUVector.y*CurCameraOrig->y+CameraUVector.z*CurCameraOrig->z),0-(TempUnitUp.x*CurCameraOrig->x+TempUnitUp.y*CurCameraOrig->y+TempUnitUp.z*CurCameraOrig->z),0-(TempUnitDest.x*CurCameraOrig->x+TempUnitDest.y*CurCameraOrig->y+TempUnitDest.z*CurCameraOrig->z),1}
    }
  };

  MATRIX m2;

  //rotate 180º over Y, opengl cam start seeing to 0,0,-1
  #define PI 3.14159265358979323846f
  C3DMatrixesTool::RotationYMatrix(&m2,PI);
  C3DMatrixesTool::MultMatrixes(&m,&m2,&VertexToCameraMatrix);
}

C3DCamera::C3DCamera(TVertex * Orig, TVertex * Dest, TVertex * Up)
{
  bFixed = 0;
  CameraOrig = Orig;
  CameraDest = Dest;
  CameraUp = Up;
  CurCameraOrig=CameraOrig;
  CurCameraDest=CameraDest;
  CurCameraUp=CameraUp;
  UpdateVectors();
}

void C3DCamera::VertexFromCamera(TVertex * VertexCoord,TVertex * VertexCameraCoord)
{
  C3DMatrixesTool::ApplyMatrix(&VertexToCameraMatrix,
                             VertexCoord->x,VertexCoord->y,VertexCoord->z,
                             &VertexCameraCoord->x,&VertexCameraCoord->y,&VertexCameraCoord->z);
}

void C3DCamera::ChangeCamera(TVertex * Orig, TVertex * Dest, TVertex * Up)
{
  bFixed = 0;
  CameraOrig = Orig;
  CameraDest = Dest; 
  CameraUp = Up;

  CurCameraOrig = CameraOrig; 
  CurCameraDest = CameraDest;
  CurCameraUp = CameraUp;

  UpdateVectors();
}

void C3DCamera::FixCamera(TVertex * Orig, TVertex * Dest, TVertex * Up)
{
  bFixed = 1;

  FixedCameraOrig.x = Orig->x;
  FixedCameraOrig.y = Orig->y;
  FixedCameraOrig.z = Orig->z;

  FixedCameraDest.x = Dest->x;
  FixedCameraDest.y = Dest->y;
  FixedCameraDest.z = Dest->z;
  
  FixedCameraUp.x = Up->x;
  FixedCameraUp.y = Up->y;
  FixedCameraUp.z = Up->z;

  CurCameraOrig = &FixedCameraOrig; 
  CurCameraDest = &FixedCameraDest;
  CurCameraUp = &FixedCameraUp;

  UpdateVectors();
}

void C3DCamera::RotateCameraOverOwnOriginX(float Angle)
{
  MATRIX m;

  C3DMatrixesTool::RotateOverVectorMatrix(&m,
                                          Angle,
                                          CurCameraOrig->x,CurCameraOrig->y,CurCameraOrig->z,
                                          -CameraUVector.x+CurCameraOrig->x,-CameraUVector.y+CurCameraOrig->y,-CameraUVector.z+CurCameraOrig->z);
  C3DMatrixesTool::ApplyMatrix(&m,
                               CurCameraDest->x,CurCameraDest->y,CurCameraDest->z,
                               &CurCameraDest->x,&CurCameraDest->y,&CurCameraDest->z);
  
  C3DMatrixesTool::ApplyMatrix(&m,
                               CurCameraUp->x,CurCameraUp->y,CurCameraUp->z,
                               &CurCameraUp->x,&CurCameraUp->y,&CurCameraUp->z);
  UpdateVectors();
}

void C3DCamera::RotateCameraOverOwnOriginY(float Angle)
{
  MATRIX m;

  C3DMatrixesTool::RotateOverVectorMatrix(&m,
                                          Angle,
                                          CurCameraOrig->x,CurCameraOrig->y,CurCameraOrig->z,
                                          CurCameraUp->x,CurCameraUp->y,CurCameraUp->z);
  C3DMatrixesTool::ApplyMatrix(&m,
                               CurCameraDest->x,CurCameraDest->y,CurCameraDest->z,
                               &CurCameraDest->x,&CurCameraDest->y,&CurCameraDest->z);
  UpdateVectors();
}

void C3DCamera::RotateCameraOverOwnOriginZ(float Angle)
{
  MATRIX m;

  C3DMatrixesTool::RotateOverVectorMatrix(&m,
                                          Angle,
                                          CurCameraOrig->x,CurCameraOrig->y,CurCameraOrig->z,
                                          CurCameraDest->x,CurCameraDest->y,CurCameraDest->z);
  C3DMatrixesTool::ApplyMatrix(&m,
                               CurCameraUp->x,CurCameraUp->y,CurCameraUp->z,
                               &CurCameraUp->x,&CurCameraUp->y,&CurCameraUp->z);
  UpdateVectors();
}

void C3DCamera::TraslateCameraUVector(float Dist)
{
  CurCameraOrig->x+=(CameraUVector.x*Dist);
  CurCameraOrig->y+=(CameraUVector.y*Dist);
  CurCameraOrig->z+=(CameraUVector.z*Dist);

  CurCameraDest->x+=(CameraUVector.x*Dist);
  CurCameraDest->y+=(CameraUVector.y*Dist);
  CurCameraDest->z+=(CameraUVector.z*Dist);

  CurCameraUp->x+=(CameraUVector.x*Dist);
  CurCameraUp->y+=(CameraUVector.y*Dist);
  CurCameraUp->z+=(CameraUVector.z*Dist);
  
  UpdateVectors();
}

void C3DCamera::TraslateCameraUp(float Dist)
{ 
  CurCameraOrig->x+=(TempUnitUp.x*Dist);
  CurCameraOrig->y+=(TempUnitUp.y*Dist);
  CurCameraOrig->z+=(TempUnitUp.z*Dist);

  CurCameraDest->x+=(TempUnitUp.x*Dist);
  CurCameraDest->y+=(TempUnitUp.y*Dist);
  CurCameraDest->z+=(TempUnitUp.z*Dist);

  CurCameraUp->x+=(TempUnitUp.x*Dist);
  CurCameraUp->y+=(TempUnitUp.y*Dist);
  CurCameraUp->z+=(TempUnitUp.z*Dist);
  
  UpdateVectors();
}

void C3DCamera::TraslateCameraEye(float Dist)
{
  CurCameraOrig->x+=(TempUnitDest.x*Dist);
  CurCameraOrig->y+=(TempUnitDest.y*Dist);
  CurCameraOrig->z+=(TempUnitDest.z*Dist);

  CurCameraDest->x+=(TempUnitDest.x*Dist);
  CurCameraDest->y+=(TempUnitDest.y*Dist);
  CurCameraDest->z+=(TempUnitDest.z*Dist);

  CurCameraUp->x+=(TempUnitDest.x*Dist);
  CurCameraUp->y+=(TempUnitDest.y*Dist);
  CurCameraUp->z+=(TempUnitDest.z*Dist);

  UpdateVectors();
}

void C3DCamera::GetCameraOrig(TVertex * o)
{
  o->x = CurCameraOrig->x;
  o->y = CurCameraOrig->y;
  o->z = CurCameraOrig->z;
}

void C3DCamera::GetCameraOrthoBasis(TVertex * u,TVertex * v,TVertex * w)
{
  float mag;

  w->x = CurCameraDest->x - CurCameraOrig->x;
  w->y = CurCameraDest->y - CurCameraOrig->y;
  w->z = CurCameraDest->z - CurCameraOrig->z;
  
  v_normalize((*w),mag,(*w));

  v->x = CurCameraUp->x - CurCameraOrig->x;
  v->y = CurCameraUp->y - CurCameraOrig->y;
  v->z = CurCameraUp->z - CurCameraOrig->z;

  v_normalize((*v),mag,(*v));

  v_cross_product((*v),(*w),(*u));

  v_normalize((*u),mag,(*u));
}
