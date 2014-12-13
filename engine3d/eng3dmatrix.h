#ifndef __ENG3DMATRIX_H__
#define __ENG3DMATRIX_H__

#include "eng3dvertex.h"

typedef struct _MATRIX
{
  float v[4][4];
}MATRIX;

static MATRIX IdentityMatrix = 
{
  {
    { 1.0, 0.0, 0.0, 0.0},
    { 0.0, 1.0, 0.0, 0.0},
    { 0.0, 0.0, 1.0, 0.0},
    { 0.0, 0.0, 0.0, 1.0}
  }
};

class C3DMatrixesTool
{

public:
  
  static void MultMatrixes(MATRIX *m1, MATRIX *m2, MATRIX *out);
  static void ApplyMatrix(MATRIX *m, float x, float y, float z, float *xout, float *yout, float *zout);
  static void TraslationMatrix(MATRIX *m, float Tx, float Ty, float Tz);
  static void EscaleMatrix(MATRIX *m, float Ex, float Ey, float Ez);
  static void RotationXMatrix(MATRIX *m, float angle);
  static void RotationYMatrix(MATRIX *m, float angle);
  static void RotationZMatrix(MATRIX *m, float angle);
  static void RotateOverVectorMatrix(MATRIX *m,float Angle,float Origx,float Origy,float Origz,float Destx,float Desty,float Destz);
  void MultMatrixes(MATRIX *m2, MATRIX *out);
  void ApplyMatrix(float x, float y, float z, float *xout, float *yout, float *zout);
  void TraslationMatrix(float Tx, float Ty, float Tz);
  void EscaleMatrix(float Ex, float Ey, float Ez);
  void RotationXMatrix(float angle);
  void RotationYMatrix(float angle);
  void RotationZMatrix(float angle);
  static void ChangeOrthoBasisMatrix(MATRIX * m,TVertex * OrthoBasisO,TVertex * OrthoBasisU,TVertex * OrthoBasisV,TVertex * OrthoBasisW);
  
  MATRIX Matrix;
  MATRIX AuxMatrix;
};

#endif