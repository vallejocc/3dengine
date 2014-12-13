#include "eng3dmatrix.h"
#include <math.h>

void C3DMatrixesTool::ApplyMatrix(MATRIX *m, float x, float y, float z, float *xout, float *yout, float *zout)
{
 *xout = x*m->v[0][0] + y*m->v[1][0] + z*m->v[2][0] + 1.0*m->v[3][0];
 *yout = x*m->v[0][1] + y*m->v[1][1] + z*m->v[2][1] + 1.0*m->v[3][1];
 *zout = x*m->v[0][2] + y*m->v[1][2] + z*m->v[2][2] + 1.0*m->v[3][2];
}

void C3DMatrixesTool::MultMatrixes(MATRIX *m1, MATRIX *m2, MATRIX *out)
{
 int i, j, k;

 for (i=0; i<4; i++) {
	for (j=0; j<4; j++) {
		out->v[i][j] = 0.0;
		for (k=0; k<4; k++)
			out->v[i][j] = out->v[i][j] + m1->v[i][k]*m2->v[k][j];
		}
	}
}

void C3DMatrixesTool::ApplyMatrix(float x, float y, float z, float *xout, float *yout, float *zout)
{
 *xout = x*Matrix.v[0][0] + y*Matrix.v[1][0] + z*Matrix.v[2][0] + 1.0*Matrix.v[3][0];
 *yout = x*Matrix.v[0][1] + y*Matrix.v[1][1] + z*Matrix.v[2][1] + 1.0*Matrix.v[3][1];
 *zout = x*Matrix.v[0][2] + y*Matrix.v[1][2] + z*Matrix.v[2][2] + 1.0*Matrix.v[3][2];
}

void C3DMatrixesTool::MultMatrixes(MATRIX *m2, MATRIX *out)
{
 int i, j, k;

 for (i=0; i<4; i++) {
	for (j=0; j<4; j++) {
		out->v[i][j] = 0.0;
		for (k=0; k<4; k++)
			out->v[i][j] = out->v[i][j] + Matrix.v[i][k]*m2->v[k][j];
		}
	}
}

void C3DMatrixesTool::TraslationMatrix(MATRIX *m, float Tx, float Ty, float Tz)
{
*m = IdentityMatrix;
 m->v[3][0] = Tx;
 m->v[3][1] = Ty;
 m->v[3][2] = Tz;
}

void C3DMatrixesTool::EscaleMatrix(MATRIX *m, float Ex, float Ey, float Ez)
{
 *m = IdentityMatrix;
 m->v[0][0] = Ex;
 m->v[1][1] = Ey;
 m->v[2][2] = Ez;
}

void C3DMatrixesTool::RotationXMatrix(MATRIX *m, float angle)
{
 float  tmpsin, tmpcos;

 tmpsin=sin(angle);
 tmpcos=cos(angle);
 *m = IdentityMatrix;
 m->v[1][1] =  tmpcos;
 m->v[1][2] =  tmpsin;
 m->v[2][1] = -tmpsin;
 m->v[2][2] =  tmpcos;
}

void C3DMatrixesTool::RotationYMatrix(MATRIX *m, float angle)
{
 float  tmpsin, tmpcos;

 tmpsin=sin(angle);
 tmpcos=cos(angle);
 *m = IdentityMatrix;
 m->v[0][0] =  tmpcos;
 m->v[0][2] = -tmpsin;
 m->v[2][0] =  tmpsin;
 m->v[2][2] =  tmpcos;
}

void C3DMatrixesTool::RotationZMatrix(MATRIX *m, float angle)
{
 float  tmpsin, tmpcos;

 tmpsin=sin(angle);
 tmpcos=cos(angle);
 *m = IdentityMatrix;
 m->v[0][0] =  tmpcos;
 m->v[0][1] =  tmpsin;
 m->v[1][0] = -tmpsin;
 m->v[1][1] =  tmpcos;
}

void C3DMatrixesTool::TraslationMatrix(float Tx, float Ty, float Tz)
{
  Matrix = IdentityMatrix;
  Matrix.v[3][0] = Tx;
  Matrix.v[3][1] = Ty;
  Matrix.v[3][2] = Tz;
}

void C3DMatrixesTool::EscaleMatrix(float Ex, float Ey, float Ez)
{
  Matrix = IdentityMatrix;
  Matrix.v[0][0] = Ex;
  Matrix.v[1][1] = Ey;
  Matrix.v[2][2] = Ez;
}

void C3DMatrixesTool::RotationXMatrix(float angle)
{
  float  tmpsin, tmpcos;

  tmpsin=sin(angle);
  tmpcos=cos(angle);
  Matrix = IdentityMatrix;
  Matrix.v[1][1] =  tmpcos;
  Matrix.v[1][2] =  tmpsin;
  Matrix.v[2][1] = -tmpsin;
  Matrix.v[2][2] =  tmpcos;
}

void C3DMatrixesTool::RotationYMatrix(float angle)
{
  float  tmpsin, tmpcos;

  tmpsin=sin(angle);
  tmpcos=cos(angle);
  Matrix = IdentityMatrix;
  Matrix.v[0][0] =  tmpcos;
  Matrix.v[0][2] = -tmpsin;
  Matrix.v[2][0] =  tmpsin;
  Matrix.v[2][2] =  tmpcos;
}

void C3DMatrixesTool::RotationZMatrix(float angle)
{
  float  tmpsin, tmpcos;

  tmpsin=sin(angle);
  tmpcos=cos(angle);
  Matrix = IdentityMatrix;
  Matrix.v[0][0] =  tmpcos;
  Matrix.v[0][1] =  tmpsin;
  Matrix.v[1][0] = -tmpsin;
  Matrix.v[1][1] =  tmpcos;
}

void C3DMatrixesTool::RotateOverVectorMatrix(MATRIX *m,float Angle,float Origx,float Origy,float Origz,float Destx,float Desty,float Destz)
{
  float A,B,C,L,Lp;
  MATRIX AuxMatrix;
  MATRIX AuxMatrix2;
  MATRIX AuxMatrix3;
  
  A = Destx - Origx;
  B = Desty - Origy;
  C = Destz - Origz;
  L = sqrt((A*A)+(B*B)+(C*C));
  Lp = sqrt((B*B)+(C*C));

  if(!B && !C) //we cant to apply the method, but its a rotation over X, so we will rotate over X with translation
  {
    C3DMatrixesTool::TraslationMatrix(&AuxMatrix,-Origx,-Origy,-Origz);
    C3DMatrixesTool::RotationXMatrix(&AuxMatrix3,Angle);
    C3DMatrixesTool::MultMatrixes(&AuxMatrix,&AuxMatrix3,&AuxMatrix2);
    C3DMatrixesTool::TraslationMatrix(&AuxMatrix,Origx,Origy,Origz);
    C3DMatrixesTool::MultMatrixes(&AuxMatrix2,&AuxMatrix,m); 
  }
  else
  {
    MATRIX Rx = 
    {
      {
        { 1.0, 0.0,   0.0,    0.0},
        { 0.0, C/Lp,  B/Lp,   0.0},
        { 0.0, -B/Lp, C/Lp,   0.0},
        { 0.0, 0.0,   0.0,    1.0}
      }
    };

    MATRIX Rxi = 
    {
      {
        { 1.0, 0.0,   0.0,    0.0},
        { 0.0, C/Lp,  -B/Lp,  0.0},
        { 0.0, B/Lp,  C/Lp,   0.0},
        { 0.0, 0.0,   0.0,    1.0}
      }
    };

    MATRIX Ry = 
    {
      {
        { Lp/L, 0.0,   A/L,    0.0},
        { 0.0,  1.0,   0.0,    0.0},
        { -A/L, 0.0,   Lp/L,   0.0},
        { 0.0,  0.0,   0.0,    1.0}
      }
    };

    MATRIX Ryi = 
    {
      {
        { Lp/L, 0.0,  -A/L,    0.0},
        { 0.0,  1.0,   0.0,    0.0},
        { A/L, 0.0,   Lp/L,   0.0},
        { 0.0,  0.0,   0.0,    1.0}
      }
    };
    
    C3DMatrixesTool::TraslationMatrix(&AuxMatrix,-Origx,-Origy,-Origz);
    C3DMatrixesTool::MultMatrixes(&AuxMatrix,&Rx,&AuxMatrix3);
    C3DMatrixesTool::MultMatrixes(&AuxMatrix3,&Ry,&AuxMatrix);
    C3DMatrixesTool::RotationZMatrix(&AuxMatrix3,Angle);
    C3DMatrixesTool::MultMatrixes(&AuxMatrix,&AuxMatrix3,&AuxMatrix2);
    C3DMatrixesTool::MultMatrixes(&AuxMatrix2,&Ryi,&AuxMatrix);
    C3DMatrixesTool::MultMatrixes(&AuxMatrix,&Rxi,&AuxMatrix3);
    C3DMatrixesTool::TraslationMatrix(&AuxMatrix,Origx,Origy,Origz);
    C3DMatrixesTool::MultMatrixes(&AuxMatrix3,&AuxMatrix,m);
  }
}


void C3DMatrixesTool::ChangeOrthoBasisMatrix(MATRIX * m,
                                             TVertex * OrthoBasisO,
                                             TVertex * OrthoBasisU,
                                             TVertex * OrthoBasisV,
                                             TVertex * OrthoBasisW)
{
  MATRIX temp = 
  {
    {
      {OrthoBasisU->x,OrthoBasisV->x,OrthoBasisW->x,0},
      {OrthoBasisU->y,OrthoBasisV->y,OrthoBasisW->y,0},
      {OrthoBasisU->z,OrthoBasisV->z,OrthoBasisW->z,0},
      {0-(OrthoBasisU->x*OrthoBasisO->x+OrthoBasisU->y*OrthoBasisO->y+OrthoBasisU->z*OrthoBasisO->z),0-(OrthoBasisV->x*OrthoBasisO->x+OrthoBasisV->y*OrthoBasisO->y+OrthoBasisV->z*OrthoBasisO->z),0-(OrthoBasisW->x*OrthoBasisO->x+OrthoBasisW->y*OrthoBasisO->y+OrthoBasisW->z*OrthoBasisO->z),1},
    }
  };
  
  *m = temp;
}