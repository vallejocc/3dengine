#include <windows.h>
#include "eng3ddefs.h"
#include "eng3dautodrawworld.h"
#include "eng3dopengl.h"
#include "eng3dcamera.h"
#include "eng3dvector.h"
#include "eng3dsolidobject.h"
#include "eng3dbox.h"
#include "eng3dmatrix.h"
#include "eng3dphysicobject.h"
#include "eng3d3dsreader.h"
#include <math.h>
//#include "glut.h"
#include "eng3dlightfocus.h"
#include <gl/gl.h>

#define DISABLE_COLLISIONS

#define PI 3.14159265358979323846f

C3DAutodrawWorld AutoDrawWorld;
TVertex OrigCam = {0,0,0};
TVertex DestCam = {0,0,1};
TVertex UpCam = {0,1,0};
C3DCamera * Cam;
CLightFocus * lf,*lf2,*lf3,*lf4,*lf5,*lf6,*lf7,*lf8;

unsigned int SelectedObject=0;

TColor test_vertexes_colors[] =
{
1.0,0.0,0.0,0.8,
1.0,0.0,0.0,0.8, 
1.0,0.0,0.0,0.8, 
1.0,0.0,0.0,0.8, 
1.0,0.0,0.0,0.8, 
1.0,0.0,0.0,0.8, 
1.0,1.0,0.0,0.8, 
1.0,0.0,0.0,0.8  
};

TColor test_vertexes_colors2[] =
{
1.0,1.0,1.0,0.8,
1.0,1.0,1.0,0.8,
1.0,1.0,1.0,0.8,
1.0,1.0,1.0,0.8,
1.0,1.0,1.0,0.8,
1.0,1.0,1.0,0.8,
1.0,1.0,1.0,0.8,
1.0,1.0,1.0,0.8
};

TColor test_vertexes_colors3[] =
{
0.0,0.0,1.0,0.8,
0.0,0.0,1.0,0.8,
0.0,0.0,1.0,0.8,
0.0,0.0,1.0,0.8,
0.0,0.0,1.0,0.8,
0.0,0.0,1.0,0.8,
0.0,0.0,1.0,0.8,
0.0,0.0,1.0,0.8
};

TColor test_vertexes_colors4[] =
{
0.0,1.0,0.0,0.8,
0.0,1.0,0.0,0.8,
0.0,1.0,0.0,0.8,
0.0,1.0,0.0,0.8,
0.0,1.0,0.0,0.8,
0.0,1.0,0.0,0.8,
0.0,1.0,0.0,0.8,
0.0,1.0,0.0,0.8
};

TColor test_vertexes_colors5[] =
{
1.0,0.0,0.0,0.8,
1.0,0.0,0.0,0.8,
1.0,0.0,0.0,0.8,
1.0,0.0,0.0,0.8,
1.0,0.0,0.0,0.8,
1.0,0.0,0.0,0.8,
1.0,0.0,0.0,0.8,
1.0,0.0,0.0,0.8
};

TVertex test_vertexes[] = 
{
-10,-10, 10, //vertex v0
10,-10, 10, //vertex v1
10,-10,-10, //vertex v2
-10,-10,-10, //vertex v3
-10, 10, 10, //vertex v4
10, 10, 10, //vertex v5
10, 10,-10, //vertex v6 
-10, 10,-10  //vertex v7
};

TVertex test_vertexes2[] = 
{
-0.5,-0.5, 0.5, //vertex v0
0.5,-0.5, 0.5, //vertex v1
0.5,-0.5,-0.5, //vertex v2
-0.5,-0.5,-0.5, //vertex v3
-0.5, 0.5, 0.5, //vertex v4
0.5, 0.5, 0.5, //vertex v5
0.5, 0.5,-0.5, //vertex v6 
-0.5, 0.5,-0.5  //vertex v7
};

TVertex test_vertexes3[] = 
{
-0.5+10,-0.5, 0.5, //vertex v0
0.5+10,-0.5, 0.5, //vertex v1
0.5+10,-0.5,-0.5, //vertex v2
-0.5+10,-0.5,-0.5, //vertex v3
-0.5+10, 0.5, 0.5, //vertex v4
0.5+10, 0.5, 0.5, //vertex v5
0.5+10, 0.5,-0.5, //vertex v6 
-0.5+10, 0.5,-0.5  //vertex v7
};

TVertex test_vertexes4[] = 
{
-0.5,-0.5+10, 0.5, //vertex v0
0.5,-0.5+10, 0.5, //vertex v1
0.5,-0.5+10,-0.5, //vertex v2
-0.5,-0.5+10,-0.5, //vertex v3
-0.5, 0.5+10, 0.5, //vertex v4
0.5, 0.5+10, 0.5, //vertex v5
0.5, 0.5+10,-0.5, //vertex v6 
-0.5, 0.5+10,-0.5  //vertex v7
};

TVertex test_vertexes5[] = 
{
-0.5,-0.5, 0.5+10, //vertex v0
0.5,-0.5, 0.5+10, //vertex v1
0.5,-0.5,-0.5+10, //vertex v2
-0.5,-0.5,-0.5+10, //vertex v3
-0.5, 0.5, 0.5+10, //vertex v4
0.5, 0.5, 0.5+10, //vertex v5
0.5, 0.5,-0.5+10, //vertex v6 
-0.5, 0.5,-0.5+10  //vertex v7
};

TVertex test_vertexes6[] = 
{
-3,-3, 3-10, //vertex v0
8,-3, 3-10, //vertex v1
8,-3,-3-10, //vertex v2
-3,-3,-3-10, //vertex v3
-3, 3, 3-10, //vertex v4
8, 3, 3-10, //vertex v5
8, 3,-3-10, //vertex v6 
-3, 3,-3-10  //vertex v7
};

TVertex test_vertexes7[] = 
{
-3,-3, 3-20, //vertex v0
8,-3, 3-20, //vertex v1
8,-3,-3-20, //vertex v2
-3,-3,-3-20, //vertex v3
-3, 3, 3-20, //vertex v4
8, 3, 3-20, //vertex v5
8, 3,-3-20, //vertex v6 
-3, 3,-3-20  //vertex v7
};

TTriangle test_triangles[] = 
{
0, 1, 4,0, //polygon v0,v1,v4
1, 5, 4,0, //polygon v1,v5,v4
1, 2, 5,0, //polygon v1,v2,v5
2, 6, 5,0, //polygon v2,v6,v5
2, 3, 6,0, //polygon v2,v3,v6
3, 7, 6,0, //polygon v3,v7,v6
3, 0, 7,0, //polygon v3,v0,v7
0, 4, 7,0, //polygon v0,v4,v7
4, 5, 7,0, //polygon v4,v5,v7
5, 6, 7,0, //polygon v5,v6,v7
3, 2, 0,0, //polygon v3,v2,v0
2, 1, 0,0, //polygon v2,v1,v0
};

TVertex test_complex_object_vertexes[] = 
{
1,1,1,
0,2,0,
2,2,2,
2,2,0,
0,2,2
};

TTriangle test_complex_object_triangles[] =
{
  0,1,3,0,
  0,1,4,0,
  0,2,3,0,
  0,2,4,0,
  1,2,3,0,
  1,2,4,0,
};

TColor test_complex_object_colors[] =
{
  1,1,0,0.8,
  1,0,1,0.8,
  1,0,2,0.8,
  1,2,0,0.8,
  1,1,1,0.8
};

unsigned int ControlAmpli=1;

/*
unsigned int NotifyCollisionCallback(C3DBox * Box1,C3DBox * Box2,void * ClientParam)
{
  TColor c1 = {1.0,0.0,0.0};
  TColor c2 = {0.0,1.0,0.0};
  C3DAutodrawWorld * pAutodrawWorld = (C3DAutodrawWorld *)ClientParam;
  
  pAutodrawWorld->DebugAddBoxToScene(Box1->v0,Box1->v1,Box1->v2,Box1->v3,
                                     Box1->v4,Box1->v5,Box1->v6,Box1->v7,
                                    &c1);

  pAutodrawWorld->DebugAddBoxToScene(Box2->v0,Box2->v1,Box2->v2,Box2->v3,
                                     Box2->v4,Box2->v5,Box2->v6,Box2->v7,
                                    &c2);
  return 0;
}
*/

void Display(void)
{

  TVertex TempUnitEye;
  float mag;

  TempUnitEye.x = DestCam.x - OrigCam.x;
  TempUnitEye.y = DestCam.y - OrigCam.y;
  TempUnitEye.z = DestCam.z - OrigCam.z;

  v_normalize(TempUnitEye,mag,TempUnitEye);

TColor test_vertexes_colors_camera_dir[] =
    {
         1.0,0.0,0.0,
         1.0,0.0,0.0,
         1.0,0.0,0.0,
         1.0,0.0,0.0,
         1.0,0.0,0.0,
         1.0,0.0,0.0,
         1.0,0.0,0.0,
         1.0,0.0,0.0
    };

TColor test_vertexes_colors_camera_dir2[] =
    {
         1.0,1.0,0.0,
         1.0,1.0,0.0,
         1.0,1.0,0.0,
         1.0,1.0,0.0,
         1.0,1.0,0.0,
         1.0,1.0,0.0,
         1.0,1.0,0.0,
         1.0,1.0,0.0
    };

#define TARGETDIST 15
#define TARGETDIST2 4

  TVertex test_vertexes_camera_dir[] =
  {
    -0.05+DestCam.x+TempUnitEye.x*TARGETDIST,-0.05+DestCam.y+TempUnitEye.y*TARGETDIST, 0.05+DestCam.z+TempUnitEye.z*TARGETDIST, //vertex v0
     0.05+DestCam.x+TempUnitEye.x*TARGETDIST,-0.05+DestCam.y+TempUnitEye.y*TARGETDIST, 0.05+DestCam.z+TempUnitEye.z*TARGETDIST, //vertex v1
     0.05+DestCam.x+TempUnitEye.x*TARGETDIST,-0.05+DestCam.y+TempUnitEye.y*TARGETDIST,-0.05+DestCam.z+TempUnitEye.z*TARGETDIST, //vertex v2
    -0.05+DestCam.x+TempUnitEye.x*TARGETDIST,-0.05+DestCam.y+TempUnitEye.y*TARGETDIST,-0.05+DestCam.z+TempUnitEye.z*TARGETDIST, //vertex v3
    -0.05+DestCam.x+TempUnitEye.x*TARGETDIST, 0.05+DestCam.y+TempUnitEye.y*TARGETDIST, 0.05+DestCam.z+TempUnitEye.z*TARGETDIST, //vertex v4
     0.05+DestCam.x+TempUnitEye.x*TARGETDIST, 0.05+DestCam.y+TempUnitEye.y*TARGETDIST, 0.05+DestCam.z+TempUnitEye.z*TARGETDIST, //vertex v5
     0.05+DestCam.x+TempUnitEye.x*TARGETDIST, 0.05+DestCam.y+TempUnitEye.y*TARGETDIST,-0.05+DestCam.z+TempUnitEye.z*TARGETDIST, //vertex v6 
    -0.05+DestCam.x+TempUnitEye.x*TARGETDIST, 0.05+DestCam.y+TempUnitEye.y*TARGETDIST,-0.05+DestCam.z+TempUnitEye.z*TARGETDIST  //vertex v7  
  };

  TVertex test_vertexes_camera_dir2[] =
  {
    -0.005+DestCam.x+TempUnitEye.x*TARGETDIST2,-0.005+DestCam.y+TempUnitEye.y*TARGETDIST2, 0.005+DestCam.z+TempUnitEye.z*TARGETDIST2, //vertex v0
     0.005+DestCam.x+TempUnitEye.x*TARGETDIST2,-0.005+DestCam.y+TempUnitEye.y*TARGETDIST2, 0.005+DestCam.z+TempUnitEye.z*TARGETDIST2, //vertex v1
     0.005+DestCam.x+TempUnitEye.x*TARGETDIST2,-0.005+DestCam.y+TempUnitEye.y*TARGETDIST2,-0.005+DestCam.z+TempUnitEye.z*TARGETDIST2, //vertex v2
    -0.005+DestCam.x+TempUnitEye.x*TARGETDIST2,-0.005+DestCam.y+TempUnitEye.y*TARGETDIST2,-0.005+DestCam.z+TempUnitEye.z*TARGETDIST2, //vertex v3
    -0.005+DestCam.x+TempUnitEye.x*TARGETDIST2, 0.005+DestCam.y+TempUnitEye.y*TARGETDIST2, 0.005+DestCam.z+TempUnitEye.z*TARGETDIST2, //vertex v4
     0.005+DestCam.x+TempUnitEye.x*TARGETDIST2, 0.005+DestCam.y+TempUnitEye.y*TARGETDIST2, 0.005+DestCam.z+TempUnitEye.z*TARGETDIST2, //vertex v5
     0.005+DestCam.x+TempUnitEye.x*TARGETDIST2, 0.005+DestCam.y+TempUnitEye.y*TARGETDIST2,-0.005+DestCam.z+TempUnitEye.z*TARGETDIST2, //vertex v6 
    -0.005+DestCam.x+TempUnitEye.x*TARGETDIST2, 0.005+DestCam.y+TempUnitEye.y*TARGETDIST2,-0.005+DestCam.z+TempUnitEye.z*TARGETDIST2  //vertex v7  
  };

  TObjectList * ObjNode;
  ObjNode = (TObjectList *)AutoDrawWorld.ObjectListHead.Flink;

  AutoDrawWorld.ClearScene();

  C3DBox * testbox;
  C3DBox * testbox2;
  C3DBox * boxesptr;
  TColor c = {1.0,0.0,0.0};
  TColor c2 = {0.0,1.0,0.0};

  /*AutoDrawWorld.AddTrianglesToScene(test_triangles,12,test_vertexes_camera_dir,8,test_vertexes_colors_camera_dir);
  AutoDrawWorld.AddTrianglesToScene(test_triangles,12,test_vertexes_camera_dir2,8,test_vertexes_colors_camera_dir2);*/
  AutoDrawWorld.AddAllWorldObjectsToScene();
  
  TObjectList * SelTemp;
  TObjectList * Temp = ENG3D_BASE_FROM_FIELD(TObjectList,Link,AutoDrawWorld.ObjectListHead.Flink);

  unsigned int i=0;

  /*
  for(i=0;i<SelectedObject;i++)
  {
    SOLID_OBJECT(Temp->Object)->BBox->ApplyBoxToWorldMatrix(&Temp->Object->ObjectToWorldMatrix);

    Temp = ENG3D_BASE_FROM_FIELD(TObjectList,Link,Temp->Link.Flink);

    if((void*)Temp == (void*)&AutoDrawWorld.ObjectListHead)
    {
      Temp = ENG3D_BASE_FROM_FIELD(TObjectList,Link,AutoDrawWorld.ObjectListHead.Flink);
    }
  }

  SelTemp=Temp;
  
  SOLID_OBJECT(Temp->Object)->BBox->ApplyBoxToWorldMatrix(&SOLID_OBJECT(Temp->Object)->ObjectToWorldMatrix);
  AutoDrawWorld.DebugAddBoxToScene(SOLID_OBJECT(Temp->Object)->BBox->v0,
                                   SOLID_OBJECT(Temp->Object)->BBox->v1,
                                   SOLID_OBJECT(Temp->Object)->BBox->v2,
                                   SOLID_OBJECT(Temp->Object)->BBox->v3,
                                   SOLID_OBJECT(Temp->Object)->BBox->v4,
                                   SOLID_OBJECT(Temp->Object)->BBox->v5,
                                   SOLID_OBJECT(Temp->Object)->BBox->v6,
                                   SOLID_OBJECT(Temp->Object)->BBox->v7,
                                   &c2);


  Temp = ENG3D_BASE_FROM_FIELD(TObjectList,Link,AutoDrawWorld.ObjectListHead.Flink);

  while((void*)Temp != (void*)&AutoDrawWorld.ObjectListHead)
  {
    if(SelTemp!=Temp)
    {
      SOLID_OBJECT(Temp->Object)->BBox->ApplyBoxToWorldMatrix
        (&SOLID_OBJECT(Temp->Object)->ObjectToWorldMatrix);
      AutoDrawWorld.DebugAddBoxToScene(SOLID_OBJECT(Temp->Object)->BBox->v0,
                                       SOLID_OBJECT(Temp->Object)->BBox->v1,
                                       SOLID_OBJECT(Temp->Object)->BBox->v2,
                                       SOLID_OBJECT(Temp->Object)->BBox->v3,
                                       SOLID_OBJECT(Temp->Object)->BBox->v4,
                                       SOLID_OBJECT(Temp->Object)->BBox->v5,
                                       SOLID_OBJECT(Temp->Object)->BBox->v6,
                                       SOLID_OBJECT(Temp->Object)->BBox->v7,
                                       &c);

      if(!Eng3dIsListEmpty(&SOLID_OBJECT(Temp->Object)->CollisionInfoList))
      {
        ENG3D_LIST_ENTRY * link = SOLID_OBJECT(Temp->Object)->CollisionInfoList.Flink;
        
        while(link!=&SOLID_OBJECT(Temp->Object)->CollisionInfoList)
        {
          C3DSolidObject::tCollisionInfo * ci = ENG3D_BASE_FROM_FIELD(C3DSolidObject::tCollisionInfo,Link,link);           
          AutoDrawWorld.DebugAddLineToScene(&ci->IntersectionLinePO,&ci->IntersectionLinePD,&c);
          link = link->Flink;
        }        
      }
    }

    Temp = ENG3D_BASE_FROM_FIELD(TObjectList,Link,Temp->Link.Flink);
  }
  */

  AutoDrawWorld.ShowScene();
}

void Reshape(int width, int height)
{
  AutoDrawWorld.DoReshape(width,height);
}

void Keyboard(bool * keysstate,unsigned char key, int x, int y)
{ 
  TObjectList * ObjNode;
  ObjNode = (TObjectList *)AutoDrawWorld.ObjectListHead.Flink;
  unsigned int testcol=0;
  TObjectList * ObjCol1=NULL;


  switch(key)
  {
    case 0x26: //up
    {
      if(keysstate['H'])
        lf->Orig.y+=ControlAmpli;
      else
        Cam->RotateCameraOverOwnOriginX(-0.02);
    }
    break;
    
    case 0x28: //down
    {
      if(keysstate['H'])
        lf->Orig.y-=ControlAmpli;
      else
        Cam->RotateCameraOverOwnOriginX(0.02);
    }
    break;

    case 0x25: //left
    {
      if(keysstate['H'])
        lf->Orig.x-=ControlAmpli;
      else
        Cam->RotateCameraOverOwnOriginY(-0.02);
    }
    break;
    
    case 0x27: //right
    {
      if(keysstate['H'])
        lf->Orig.x+=ControlAmpli;
      else
        Cam->RotateCameraOverOwnOriginY(0.02);
    }
    break;

    case 'F':
    {
      static int fillin=1;
      
      if(!fillin)
      {
        glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
        fillin=1;
      }
      else
      {
        glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
        fillin=0;
      }
    }
    break;
    
    case 'H':
      {
        /*MATRIX m;
        C3DMatrixesTool::RotationYMatrix(&m,0.1);
        C3DMatrixesTool::ApplyMatrix(&m,lf->Orig.x,lf->Orig.y,lf->Orig.z,&lf->Orig.x,&lf->Orig.y,&lf->Orig.z);
        C3DMatrixesTool::ApplyMatrix(&m,lf->Dest.x,lf->Dest.y,lf->Dest.z,&lf->Dest.x,&lf->Dest.y,&lf->Dest.z);
        */
      }
      break;

    case 'k':
      if(keysstate['H'])
      {
        lf->ambient[0]+=0.02*ControlAmpli;
        lf->ambient[1]+=0.02*ControlAmpli;
        lf->ambient[2]+=0.02*ControlAmpli;
        lf->diffuse[0]+=0.02*ControlAmpli;
        lf->diffuse[1]+=0.02*ControlAmpli;
        lf->diffuse[2]+=0.02*ControlAmpli;
        lf->specular[0]+=0.02*ControlAmpli;
        lf->specular[1]+=0.02*ControlAmpli;
        lf->specular[2]+=0.02*ControlAmpli;
      }
      else
        ControlAmpli++;
      break;

    case 'm':

      if(keysstate['H'])
      {
        lf->ambient[0]-=0.02*ControlAmpli;
        lf->ambient[1]-=0.02*ControlAmpli;
        lf->ambient[2]-=0.02*ControlAmpli;
        lf->diffuse[0]-=0.02*ControlAmpli;
        lf->diffuse[1]-=0.02*ControlAmpli;
        lf->diffuse[2]-=0.02*ControlAmpli;
        lf->specular[0]-=0.02*ControlAmpli;
        lf->specular[1]-=0.02*ControlAmpli;
        lf->specular[2]-=0.02*ControlAmpli;
      }
      else
        if(ControlAmpli)ControlAmpli--;
      break;

    case 'S':
    {
      Cam->TraslateCameraUp(0.3*ControlAmpli); 
    }
    break;

    case 'X':
    {
      Cam->TraslateCameraUp(-0.3*ControlAmpli);
    }
    break;

    case 'Z':
    {
      Cam->TraslateCameraUVector(0.3*ControlAmpli); 
    }
    break;

    case 'C':
    {
      Cam->TraslateCameraUVector(-0.3*ControlAmpli); 
    }
    break;

    case 'G':
    {
      Cam->TraslateCameraEye(0.3*ControlAmpli);
    }
    break;

    case 'B':
    {
      Cam->TraslateCameraEye(-0.3*ControlAmpli);
    }
    break;

    case 'Q':
    {
      Cam->RotateCameraOverOwnOriginZ(0.005*ControlAmpli);
    }
    break;

    case 'W':
    {
      Cam->RotateCameraOverOwnOriginZ(-0.005*ControlAmpli);
    }
    break;

    case '9':
    case '6':
    case '7':
    case '8':
    case 'I':
    case 'K':
    case 'L':
    case 'J':
    case 'O':
    {

      TObjectList * Temp = ENG3D_BASE_FROM_FIELD(TObjectList,Link,AutoDrawWorld.ObjectListHead.Flink);

      unsigned int i=0;

      for(i=0;i<SelectedObject;i++)
      {
#ifndef DISABLE_COLLISIONS
        SOLID_OBJECT(Temp->Object)->BBox->ApplyBoxToWorldMatrix(&Temp->Object->ObjectToWorldMatrix);
#endif
        Temp = ENG3D_BASE_FROM_FIELD(TObjectList,Link,Temp->Link.Flink);

        if((void*)Temp == (void*)&AutoDrawWorld.ObjectListHead)
        {
          Temp = ENG3D_BASE_FROM_FIELD(TObjectList,Link,AutoDrawWorld.ObjectListHead.Flink);
        }
      }

#ifndef DISABLE_COLLISIONS
      Temp->Object->Snapshot();
#endif

      ObjCol1 = Temp;

      TVertex orig={0,0,0};
      TVertex CamOrig;
      TVertex u,v,w;
      Cam->GetCameraOrthoBasis(&u,&v,&w);
      Cam->GetCameraOrig(&CamOrig);
      TVertex oo;
      TVertex ou,ov,ow;
      Temp->Object->GetObjectOrthoBasis(&oo,&ou,&ov,&ow);
      TVertex oc;
#ifndef DISABLE_COLLISIONS
      oc.x=(SOLID_OBJECT(Temp->Object)->v5.x+SOLID_OBJECT(Temp->Object)->v3.x)/2;
      oc.y=(SOLID_OBJECT(Temp->Object)->v5.y+SOLID_OBJECT(Temp->Object)->v3.y)/2;
      oc.z=(SOLID_OBJECT(Temp->Object)->v5.z+SOLID_OBJECT(Temp->Object)->v3.z)/2;
      C3DMatrixesTool::ApplyMatrix(&Temp->Object->ObjectToWorldMatrix,
                                   oc.x,oc.y,oc.z,
                                   &oc.x,&oc.y,&oc.z);
#else
      oc.x=oo.x;
      oc.y=oo.y;
      oc.z=oo.z;
#endif
      switch(key)
      {
        case 'I':
          Temp->Object->Traslate(v.x*0.2*ControlAmpli,v.y*0.2*ControlAmpli,v.z*0.2*ControlAmpli);
          testcol=1;
          break;

        case 'K':
          Temp->Object->Traslate(v.x*-0.2*ControlAmpli,v.y*-0.2*ControlAmpli,v.z*-0.2*ControlAmpli);
          testcol=1;
          break;

        case 'L':
          Temp->Object->Traslate(u.x*0.2*ControlAmpli,u.y*0.2*ControlAmpli,u.z*0.2*ControlAmpli);
          testcol=1;
          break;

        case 'J':
          Temp->Object->Traslate(-u.x*0.2*ControlAmpli,-u.y*0.2*ControlAmpli,-u.z*0.2*ControlAmpli);
          testcol=1;
          break;

        case '9':
          Temp->Object->Traslate(w.x*0.2*ControlAmpli,w.y*0.2*ControlAmpli,w.z*0.2*ControlAmpli);
          testcol=1;
          break;

        case 'O':
          Temp->Object->Traslate(w.x*-0.2*ControlAmpli,w.y*-0.2*ControlAmpli,w.z*-0.2*ControlAmpli);
          testcol=1;
          break;

        case '6':
          u.x+=oc.x;
          u.y+=oc.y;
          u.z+=oc.z;
          Temp->Object->RotateOverVector(0.2*ControlAmpli,&oc,&u);
          testcol=1;
          break;

        case '7':
          v.x+=oc.x;
          v.y+=oc.y;
          v.z+=oc.z;
          Temp->Object->RotateOverVector(0.2*ControlAmpli,&oc,&v);
          testcol=1;
          break;

        case '8':
          w.x+=oc.x;
          w.y+=oc.y;
          w.z+=oc.z;
          Temp->Object->RotateOverVector(0.2*ControlAmpli,&oc,&w);
          testcol=1;
          break;
      };
    }

#ifdef DISABLE_COLLISIONS
  testcol=0;
#endif

    if(testcol)
    {
      TObjectList * ObjCol2 = 
        ENG3D_BASE_FROM_FIELD(TObjectList,Link,AutoDrawWorld.ObjectListHead.Flink);
      
      while((void*)ObjCol2 != (void*)&AutoDrawWorld.ObjectListHead)
      {
        if(ObjCol1->Object!=ObjCol2->Object)
        {
          if(SOLID_OBJECT(ObjCol2->Object)->Collide(SOLID_OBJECT(ObjCol1->Object)))
          {
            ObjCol1->Object->RevertToSnapshot();
            //Beep( 750, 300 );
          }
        }

        ObjCol2 = ENG3D_BASE_FROM_FIELD(TObjectList,Link,ObjCol2->Link.Flink);
      }
    }
    break;

    case 9: //TAB
    {
      SelectedObject++;
    }
    break;
  };
}

void Mouse(int flags, int x, int y)
{
  //flags:
  //MK_CONTROL Activo si la tecla CTRL está pulsada. 
  //MK_LBUTTON Activo si el botón izquierdo del ratón está pulsado. 
  //MK_MBUTTON Activo si el botón central del ratón está pulsado. 
  //MK_RBUTTON Activo si el botón derecho del ratón está pulsado. 
  //MK_SHIFT Activo si la tecla MAYÚSCULAS está pulsada. 

  static int lastx=-1;
  static int lasty=-1;

  if(lastx==-1 && lasty==-1)
  {
    lastx=x;
    lasty=y;
    return;
  }

  if(flags & MK_SHIFT  && flags & MK_CONTROL)
  {
    Cam->TraslateCameraEye(0.1*(lasty-y)*ControlAmpli);
    Cam->RotateCameraOverOwnOriginZ(0.005*(lastx-x)*ControlAmpli);
  }
  else
  {
    if(flags & MK_CONTROL)
    {
      Cam->RotateCameraOverOwnOriginX(-0.005*(lasty-y)*ControlAmpli);
      Cam->RotateCameraOverOwnOriginY(-0.005*(lastx-x)*ControlAmpli);
    }

    if(flags & MK_SHIFT)
    {
      Cam->TraslateCameraUp(0.1*(lasty-y)*ControlAmpli);
      Cam->TraslateCameraUVector(0.1*(lastx-x)*ControlAmpli);
    }
  }

  lastx=x;
  lasty=y;  
}

int __stdcall WinMain(HINSTANCE a,HINSTANCE b,LPSTR c,int dd)
{
  C3D3dsReader * Reader3ds = new C3D3dsReader("F:\\Documents and Settings\\Javier\\Desktop\\3dsloader\\lololo.3ds");
  
#ifndef DISABLE_COLLISIONS
  C3DPhysicObject Obj1,Obj2,Obj3,Obj4,Obj5,Obj6;
  C3DPhysicObject * TmpObj;
#else
  C3DObject Obj1,Obj2,Obj3,Obj4,Obj5,Obj6;
  C3DObject * TmpObj;
#endif

  Cam = new C3DCamera(&OrigCam,&DestCam,&UpCam);
  
  AutoDrawWorld.Initialize();

  ENG3D_LIST_ENTRY * TempLink;

  unsigned int aaaa=0;

  for(TempLink=Reader3ds->ObjectsListHead.Flink;TempLink!=&Reader3ds->ObjectsListHead;TempLink=TempLink->Flink)
  {
    
    aaaa++;

    tObjectInfo3ds * objinfo = ENG3D_BASE_FROM_FIELD(tObjectInfo3ds,Next,TempLink);

    if(objinfo->nObjectTriangles && objinfo->nObjectVertexes)
    {
      TColor * tempc = new TColor[objinfo->nObjectVertexes];
      
      unsigned int i=0;

      for(i=0;i<objinfo->nObjectVertexes;i++)
      {
        if(objinfo->nMaterials)
        {
          tempc[i].r=objinfo->Materials[0]->DiffuseColorRgba.r;tempc[i].r/=255;
          tempc[i].g=objinfo->Materials[0]->DiffuseColorRgba.g;tempc[i].g/=255;
          tempc[i].b=objinfo->Materials[0]->DiffuseColorRgba.b;tempc[i].b/=255;
          tempc[i].a=100-objinfo->Materials[0]->Transparency;tempc[i].a/=100;
        }
        else
        {
          tempc[i].r=1+i%3;
          tempc[i].g=0+2*i%5;
          tempc[i].b=0+3*i%2;
          tempc[i].a=0.5;
        }
      }

      unsigned int j=0;
      unsigned int k=0;
      unsigned int k2=0;

      for(i=0;i<objinfo->nObjectVertexes;i++)
      {
        for(j=i+1;j<objinfo->nObjectVertexes;j++)
        {
          if(objinfo->ObjectVertexes[i].x == objinfo->ObjectVertexes[j].x &&
            objinfo->ObjectVertexes[i].y == objinfo->ObjectVertexes[j].y &&
            objinfo->ObjectVertexes[i].z == objinfo->ObjectVertexes[j].z)
          {
            k++;
            __asm nop;
          }
        }
      }

      for(i=0;i<objinfo->nObjectVertexes;i++)
      {
        k=0;

        for(j=0;j<objinfo->nObjectTriangles;j++)
        {
          if(objinfo->ObjectTriangles[j].a==i ||
             objinfo->ObjectTriangles[j].b==i ||
             objinfo->ObjectTriangles[j].c==i)
          {
            k++;
          }
        }

        if(!k)
        {
          //unuseful vertex!!
          k2++;     
          __asm nop;
        }
      }


#ifndef DISABLE_COLLISIONS
      TmpObj = new C3DPhysicObject();
#else
      TmpObj = new C3DObject();
#endif
      TmpObj->Initialize(objinfo);

      AutoDrawWorld.AddObject(TmpObj);
    }
  }

  /*Obj1.Initialize(objinfo->ObjectTriangles,
                  objinfo->nObjectTriangles,
                  objinfo->ObjectVertexes,
                  objinfo->nObjectVertexes,
                  tempc);*/
  
  //Obj1.Initialize(test_triangles,12,test_vertexes,8,test_vertexes_colors);
  //Obj2.Initialize(test_triangles,12,test_vertexes2,8,test_vertexes_colors2);
  //Obj3.Initialize(test_triangles,12,test_vertexes3,8,test_vertexes_colors3);
  //Obj4.Initialize(test_triangles,12,test_vertexes4,8,test_vertexes_colors4);
  //Obj5.Initialize(test_triangles,12,test_vertexes5,8,test_vertexes_colors5);
  //Obj6.Initialize(test_complex_object_triangles,6,test_complex_object_vertexes,5,test_complex_object_colors);  

  //AutoDrawWorld.AddObject(&Obj1);
  //AutoDrawWorld.AddObject(&Obj2);
  //AutoDrawWorld.AddObject(&Obj3);
  //AutoDrawWorld.AddObject(&Obj4);
  //AutoDrawWorld.AddObject(&Obj5);
  //AutoDrawWorld.AddObject(&Obj6);

  GLfloat light_ambient[] = { 0.8, 0.8, 0.8, 1.0 };
  GLfloat light_diffuse[] = { 0.8, 0.8, 0.8,  1.0 };
  GLfloat light_specular[] = { 0.8, 0.8, 0.8,  1.0 };
  TVertex LightOrig = {0,0,0};
  TVertex LightDest = {0,0,2};

  GLfloat light_ambient2[] = { 0.2, 0.2, 0.2, 0.0 };
  GLfloat light_diffuse2[] = { 0.2, 0.2, 0.2,  0.0 };
  GLfloat light_specular2[] = { 0.2, 0.2, 0.2,  0.0 };
  TVertex LightOrig2 = {0,0,0};
  TVertex LightDest2 = {1,0,0};

  GLfloat light_ambient3[] = { 0.2, 0.2, 0.2, 0.0 };
  GLfloat light_diffuse3[] = { 0.2, 0.2, 0.2,  0.0 };
  GLfloat light_specular3[] = { 0.2, 0.2, 0.2,  0.0 };
  TVertex LightOrig3 = {0,0,0};
  TVertex LightDest3 = {-1,0,0};

  lf = new CLightFocus(light_ambient,light_diffuse,light_specular,&LightOrig,&LightDest,1);
  lf2 = new CLightFocus(light_ambient2,light_diffuse2,light_specular2,&LightOrig2,&LightDest2,0);
  lf3 = new CLightFocus(light_ambient3,light_diffuse3,light_specular3,&LightOrig3,&LightDest3,0);

  AutoDrawWorld.Start("hxxxxxx",640,480,0,0,Display,Display,Keyboard,Reshape,Mouse,Cam,lf,lf2,lf3);

  delete Cam;
}