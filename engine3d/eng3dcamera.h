#ifndef __ENG3DCAMERA_H__
#define __ENG3DCAMERA_H__

#include "eng3dvertex.h"
#include "eng3dmatrix.h"

class C3DCamera
{

public:

  C3DCamera(TVertex * Orig, TVertex * Dest, TVertex * Up);
  void VertexFromCamera(TVertex * VertexCoord,TVertex * VertexCameraCoord);
  void ChangeCamera(TVertex * Orig, TVertex * Dest, TVertex * Up);
  void FixCamera(TVertex * Orig, TVertex * Dest, TVertex * Up);

  void RotateCameraOverOwnOriginX(float Angle);
  void RotateCameraOverOwnOriginY(float Angle);
  void RotateCameraOverOwnOriginZ(float Angle);
  void TraslateCameraUVector(float Dist);
  void TraslateCameraUp(float Dist);
  void TraslateCameraEye(float Dist);

  void GetCameraOrthoBasis(TVertex * u,TVertex * v,TVertex * w);
  void GetCameraOrig(TVertex * o);

  TVertex * CameraOrig;
  TVertex * CameraDest;
  TVertex * CameraUp;
  TVertex FixedCameraOrig;
  TVertex FixedCameraDest;
  TVertex FixedCameraUp;
  
private:
  
  void UpdateVectors();

  TVertex *CurCameraOrig, *CurCameraDest, *CurCameraUp;
  TVertex TempUnitUp,TempUnitDest;
  TVertex CameraUVector;

  MATRIX VertexToCameraMatrix;

  unsigned int bFixed;
};

#endif