#include "eng3dopengl.h"

#include <windows.h>
#include "glut.h"  
#include "eng3dpolygon.h"
#include "eng3dvertex.h"
#include "eng3dvector.h"
#include "eng3dcolor.h"
//#include <gl/gl.h>

#define GIVE_TO_OPENGL_AS_ARRAYS

#ifdef GIVE_TO_OPENGL_AS_ARRAYS
    typedef struct
    {
      TVertex Normala;
      TVertex Vertexa;
      TVertex Normalb;
      TVertex Vertexb;
      TVertex Normalc;
      TVertex Vertexc;
    } tInterleavedArrayElement;

    typedef struct
    {
      TVertex Vertexa;
      TVertex Vertexb;
      TVertex Vertexc;
    }tTriangleVertexArrayElement;

    typedef struct
    {
      TVertexIndex a,b,c;
    }tTrianglesArrayElement;

    typedef struct
    {
      TVertex v;
    }tVerticesArrayElement;
#endif

C3DOpenglGlut::C3DOpenglGlut()
{
	m_hDC=NULL;	
	m_hRC=NULL;	
	m_hWnd=NULL;	
	m_hInstance=NULL;	
}

C3DOpenglGlut::~C3DOpenglGlut()
{

}

void C3DOpenglGlut::DoReshape(int width, int height)
{
  WindowWidth=width; // We obtain the new screen width values and store it
  WindowHeight=height; // Height value

  glViewport(0,0,WindowWidth,WindowHeight); // Viewport transformation
  glMatrixMode(GL_PROJECTION); // Projection transformation
  glLoadIdentity(); // We initialize the projection matrix as identity
  gluPerspective(45.0f,(GLfloat)WindowWidth/(GLfloat)WindowHeight,1.0f,1000.0f);
  //glMatrixMode(GL_MODELVIEW);
  //gluLookAt(0,0,0,0,0,1,0,1,0);
}

ENG3DERR C3DOpenglGlut::ClearScene()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // This clear the background color to dark blue
  //glMatrixMode(GL_MODELVIEW); // Modeling transformation
  //glLoadIdentity(); // Initialize the model matrix as identity
  return ENG3DERR_NO_ERROR;
}

ENG3DERR C3DOpenglGlut::ShowScene()
{
  glFlush(); // This force the execution of OpenGL commands
  //glutSwapBuffers(); // In float buffered mode we invert the positions of the visible buffer and the writing buffer
  SwapBuffers(m_hDC);
  return ENG3DERR_NO_ERROR;
}

ENG3DERR C3DOpenglGlut::DebugAddLineToScene(TVertex * v0,TVertex * v1,TColor * color)
{
  if(Camera)
  {
    TVertex Camerav0;
    TVertex Camerav1;
    Camera->VertexFromCamera(v0,&Camerav0);v0=&Camerav0;
    Camera->VertexFromCamera(v1,&Camerav1);v1=&Camerav1;

    glBegin(GL_LINES);
    glColor4f(color->r,color->g,color->b,color->a);
    glVertex3f(v0->x,v0->y,v0->z);glVertex3f(v1->x,v1->y,v1->z);
    glEnd();  
  }
  return ENG3DERR_NO_ERROR;
}

ENG3DERR C3DOpenglGlut::DebugAddBoxToScene(TVertex * v0,TVertex * v1,TVertex * v2,TVertex * v3,
                                           TVertex * v4,TVertex * v5,TVertex * v6,TVertex * v7,
                                           TColor * color)
{
  if(Camera)
  {
    TVertex Camerav0;
    TVertex Camerav1;
    TVertex Camerav2;
    TVertex Camerav3;
    TVertex Camerav4;
    TVertex Camerav5;
    TVertex Camerav6;
    TVertex Camerav7;

    glBegin(GL_LINES);
       
    Camera->VertexFromCamera(v0,&Camerav0);v0=&Camerav0;
    Camera->VertexFromCamera(v1,&Camerav1);v1=&Camerav1;
    Camera->VertexFromCamera(v2,&Camerav2);v2=&Camerav2;
    Camera->VertexFromCamera(v3,&Camerav3);v3=&Camerav3;
    Camera->VertexFromCamera(v4,&Camerav4);v4=&Camerav4;
    Camera->VertexFromCamera(v5,&Camerav5);v5=&Camerav5;
    Camera->VertexFromCamera(v6,&Camerav6);v6=&Camerav6;
    Camera->VertexFromCamera(v7,&Camerav7);v7=&Camerav7;

    glColor4f(color->r,color->g,color->b,color->a);
    glVertex3f(v0->x,v0->y,v0->z);glVertex3f(v1->x,v1->y,v1->z);
    glVertex3f(v0->x,v0->y,v0->z);glVertex3f(v4->x,v4->y,v4->z);
    glVertex3f(v0->x,v0->y,v0->z);glVertex3f(v3->x,v3->y,v3->z);
    glVertex3f(v3->x,v3->y,v3->z);glVertex3f(v7->x,v7->y,v7->z);
    glVertex3f(v3->x,v3->y,v3->z);glVertex3f(v2->x,v2->y,v2->z);
    glVertex3f(v6->x,v6->y,v6->z);glVertex3f(v7->x,v7->y,v7->z);
    glVertex3f(v6->x,v6->y,v6->z);glVertex3f(v5->x,v5->y,v5->z);
    glVertex3f(v4->x,v4->y,v4->z);glVertex3f(v7->x,v7->y,v7->z);
    glVertex3f(v4->x,v4->y,v4->z);glVertex3f(v5->x,v5->y,v5->z);
    glVertex3f(v2->x,v2->y,v2->z);glVertex3f(v1->x,v1->y,v1->z);
    glVertex3f(v2->x,v2->y,v2->z);glVertex3f(v6->x,v6->y,v6->z);
    glVertex3f(v1->x,v1->y,v1->z);glVertex3f(v5->x,v5->y,v5->z);
    glEnd();
  }

  return ENG3DERR_NO_ERROR;
}

ENG3DERR C3DOpenglGlut::AddTrianglesToScene(C3DObject * Obj)
{
  /*
  static int a=0;
  static unsigned int tick1=0;
  static unsigned int tick2=0;

  if(!tick1)
    tick1=GetTickCount();

  tick2=GetTickCount();

  if(tick2-tick1 > 15000)
    __asm nop;

  a++;
  */

  if(Camera)
  {
    unsigned int i;

    for(i=0;i<8;i++)
    {
      if(LightFocus[i])
      {
        TVertex CameraLightOrig,CameraLightDest;
        float TempLightPos[4];float mag;
        if(LightFocus[i]->ApplyCam)
        {
          Camera->VertexFromCamera(&LightFocus[i]->Orig,&CameraLightOrig);
          Camera->VertexFromCamera(&LightFocus[i]->Dest,&CameraLightDest);
        }
        else
        {
          CameraLightOrig.x=LightFocus[i]->Orig.x;
          CameraLightOrig.y=LightFocus[i]->Orig.y;
          CameraLightOrig.z=LightFocus[i]->Orig.z;
          CameraLightDest.x=LightFocus[i]->Dest.x;
          CameraLightDest.y=LightFocus[i]->Dest.y;
          CameraLightDest.z=LightFocus[i]->Dest.z;
        }
        CameraLightDest.x=CameraLightDest.x-CameraLightOrig.x;
        CameraLightDest.y=CameraLightDest.y-CameraLightOrig.y;
        CameraLightDest.z=CameraLightDest.z-CameraLightOrig.z;
        v_normalize(CameraLightDest,mag,CameraLightDest);
        
        if(LightFocus[i]->Type==1)
        {
          TempLightPos[0]=CameraLightOrig.x;
          TempLightPos[1]=CameraLightOrig.y;
          TempLightPos[2]=CameraLightOrig.z;
          TempLightPos[3]=1;
        }
        else if(LightFocus[i]->Type==0)
        {
          TempLightPos[0]=CameraLightDest.x;
          TempLightPos[1]=CameraLightDest.y;
          TempLightPos[2]=CameraLightDest.z;
          TempLightPos[3]=0;
        }

        glLightfv (GL_LIGHT0+i, GL_AMBIENT, LightFocus[i]->ambient);
        glLightfv (GL_LIGHT0+i, GL_DIFFUSE, LightFocus[i]->diffuse);
        glLightfv (GL_LIGHT0+i, GL_SPECULAR, LightFocus[i]->specular);
        glLightfv (GL_LIGHT0+i, GL_POSITION, TempLightPos);
        //glLightfv (GL_LIGHT0+i, GL_SPOT_DIRECTION, TempLightDir);
        glEnable (GL_LIGHTING);
        glEnable (GL_LIGHT0+i);

        //TColor ccc={1,0,0,0};
        //DebugAddLineToScene(&LightFocus[i]->Orig,&LightFocus[i]->Dest,&ccc);
      }
    }

#ifdef GIVE_TO_OPENGL_AS_ARRAYS
    if(Obj->ObjectInfo3ds)
    {
      unsigned int j=0;
      unsigned int k=0;

      tVerticesArrayElement * VerticesArray = new tVerticesArrayElement[Obj->nObjectVertexes];
      tVerticesArrayElement * NormalsArray = new tVerticesArrayElement[Obj->nObjectVertexes];

      for(i=0;i<Obj->nObjectVertexes;i++)
      { 
        TVertex Normal;
        Obj->GetWorldVertexNormal(i,&Normal);
        Normal.x+=Obj->WorldObjectVertexes[i].x;
        Normal.y+=Obj->WorldObjectVertexes[i].y;
        Normal.z+=Obj->WorldObjectVertexes[i].z;
        Camera->VertexFromCamera(&Obj->WorldObjectVertexes[i],&VerticesArray[i].v);
        Camera->VertexFromCamera(&Normal,&NormalsArray[i].v);
        NormalsArray[i].v.x-=VerticesArray[i].v.x;
        NormalsArray[i].v.y-=VerticesArray[i].v.y;
        NormalsArray[i].v.z-=VerticesArray[i].v.z;
      }

      for(i=0;i<Obj->nObjectTriangles;)
      {
        if(Obj->ObjectInfo3ds->FaceMaterial)
        {
          for(j=i; 
              j+1<Obj->nObjectTriangles && 
              Obj->ObjectInfo3ds->FaceMaterial[j]==Obj->ObjectInfo3ds->FaceMaterial[j+1];
              j++);
          j++;
        }
        else
        {
          j=Obj->nObjectTriangles;
        }

        //  i                                      j
        //  {triangles with same current material} {triangles with other material or end of triangles}
     
        k=j-i;i=j;

        unsigned int bTransparent=0;
      
        if(Obj->ObjectInfo3ds->FaceMaterial && 
           Obj->ObjectInfo3ds->Materials)
        {
          i--; //for taking material

          float mat_ambient[]=
          {((float)Obj->ObjectInfo3ds->Materials[Obj->ObjectInfo3ds->FaceMaterial[i]]->AmbientColorRgba.r)/0xff, 
           ((float)Obj->ObjectInfo3ds->Materials[Obj->ObjectInfo3ds->FaceMaterial[i]]->AmbientColorRgba.g)/0xff, 
           ((float)Obj->ObjectInfo3ds->Materials[Obj->ObjectInfo3ds->FaceMaterial[i]]->AmbientColorRgba.b)/0xff,
           ((float)(100-((float)Obj->ObjectInfo3ds->Materials[Obj->ObjectInfo3ds->FaceMaterial[i]]->Transparency)))/100}; 
          float mat_diffuse[]=
          {((float)Obj->ObjectInfo3ds->Materials[Obj->ObjectInfo3ds->FaceMaterial[i]]->DiffuseColorRgba.r)/0xff, 
           ((float)Obj->ObjectInfo3ds->Materials[Obj->ObjectInfo3ds->FaceMaterial[i]]->DiffuseColorRgba.g)/0xff, 
           ((float)Obj->ObjectInfo3ds->Materials[Obj->ObjectInfo3ds->FaceMaterial[i]]->DiffuseColorRgba.b)/0xff,
           ((float)(100-((float)Obj->ObjectInfo3ds->Materials[Obj->ObjectInfo3ds->FaceMaterial[i]]->Transparency)))/100};  
          float mat_specular[]=
          {((float)Obj->ObjectInfo3ds->Materials[Obj->ObjectInfo3ds->FaceMaterial[i]]->SpecularColorRgba.r)/0xff, 
           ((float)Obj->ObjectInfo3ds->Materials[Obj->ObjectInfo3ds->FaceMaterial[i]]->SpecularColorRgba.g)/0xff, 
           ((float)Obj->ObjectInfo3ds->Materials[Obj->ObjectInfo3ds->FaceMaterial[i]]->SpecularColorRgba.b)/0xff,
           ((float)(100-((float)Obj->ObjectInfo3ds->Materials[Obj->ObjectInfo3ds->FaceMaterial[i]]->Transparency)))/100}; 
          float mat_selfillum[]=
          {((float)Obj->ObjectInfo3ds->Materials[Obj->ObjectInfo3ds->FaceMaterial[i]]->SelfIllumColorRgba.r)/0xff, 
           ((float)Obj->ObjectInfo3ds->Materials[Obj->ObjectInfo3ds->FaceMaterial[i]]->SelfIllumColorRgba.g)/0xff, 
           ((float)Obj->ObjectInfo3ds->Materials[Obj->ObjectInfo3ds->FaceMaterial[i]]->SelfIllumColorRgba.b)/0xff,
           ((float)(100-((float)Obj->ObjectInfo3ds->Materials[Obj->ObjectInfo3ds->FaceMaterial[i]]->Transparency)))/100};

          if(Obj->ObjectInfo3ds->Materials[Obj->ObjectInfo3ds->FaceMaterial[i]]->AmbientColorOn)
            glMaterialfv (GL_FRONT, GL_AMBIENT, mat_ambient);
          if(Obj->ObjectInfo3ds->Materials[Obj->ObjectInfo3ds->FaceMaterial[i]]->DiffuseColorOn)
            glMaterialfv (GL_FRONT, GL_DIFFUSE, mat_diffuse);
          if(Obj->ObjectInfo3ds->Materials[Obj->ObjectInfo3ds->FaceMaterial[i]]->SpecularColorOn)
            glMaterialfv (GL_FRONT, GL_SPECULAR, mat_specular);
          if(Obj->ObjectInfo3ds->Materials[Obj->ObjectInfo3ds->FaceMaterial[i]]->SelfIllumColorOn)
            glMaterialfv (GL_FRONT, GL_EMISSION, mat_selfillum);
          glMaterialf (GL_FRONT, GL_SHININESS,1.28*((float)Obj->ObjectInfo3ds->Materials[Obj->ObjectInfo3ds->FaceMaterial[i]]->Shininess));

          if(Obj->ObjectInfo3ds->Materials[Obj->ObjectInfo3ds->FaceMaterial[i]]->Transparency)
          {
            bTransparent=1;
            glEnable(GL_BLEND); 
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glDepthMask(GL_FALSE); 
          }

          i++; //we had decreased it for taking material
        }

        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT,0,NormalsArray);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3,GL_FLOAT,0,VerticesArray);
        glDrawElements(GL_TRIANGLES,k*3,GL_UNSIGNED_INT,&Obj->ObjectTriangles[i-k]);
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);

        if(bTransparent)
        {
          glDepthMask(GL_TRUE);
          glDisable(GL_BLEND);
        }
      }

      delete NormalsArray;
      delete VerticesArray;
    }
#else
    unsigned int LastFaceMaterial=-1;
    TVertex CameraVertexa,CameraVertexb,CameraVertexc;
    TVertex Normal;

    for (i=0;i<Obj->nObjectTriangles;i++)
    {
      unsigned int bTransparent=0;

      Camera->VertexFromCamera(&Obj->WorldObjectVertexes[Obj->ObjectTriangles[i].a],&CameraVertexa);
      Camera->VertexFromCamera(&Obj->WorldObjectVertexes[Obj->ObjectTriangles[i].b],&CameraVertexb);
      Camera->VertexFromCamera(&Obj->WorldObjectVertexes[Obj->ObjectTriangles[i].c],&CameraVertexc);

      if(Obj->ObjectInfo3ds)
      {
        if(Obj->ObjectInfo3ds->FaceMaterial && 
           Obj->ObjectInfo3ds->Materials)
        {
          if(Obj->ObjectInfo3ds->FaceMaterial[i]!=LastFaceMaterial || LastFaceMaterial==-1)
          {
            LastFaceMaterial=Obj->ObjectInfo3ds->FaceMaterial[i];

            float mat_ambient[]=
            {((float)Obj->ObjectInfo3ds->Materials[Obj->ObjectInfo3ds->FaceMaterial[i]]->AmbientColorRgba.r)/0xff, 
             ((float)Obj->ObjectInfo3ds->Materials[Obj->ObjectInfo3ds->FaceMaterial[i]]->AmbientColorRgba.g)/0xff, 
             ((float)Obj->ObjectInfo3ds->Materials[Obj->ObjectInfo3ds->FaceMaterial[i]]->AmbientColorRgba.b)/0xff,
             ((float)(100-((float)Obj->ObjectInfo3ds->Materials[Obj->ObjectInfo3ds->FaceMaterial[i]]->Transparency)))/100}; 
            float mat_diffuse[]=
            {((float)Obj->ObjectInfo3ds->Materials[Obj->ObjectInfo3ds->FaceMaterial[i]]->DiffuseColorRgba.r)/0xff, 
             ((float)Obj->ObjectInfo3ds->Materials[Obj->ObjectInfo3ds->FaceMaterial[i]]->DiffuseColorRgba.g)/0xff, 
             ((float)Obj->ObjectInfo3ds->Materials[Obj->ObjectInfo3ds->FaceMaterial[i]]->DiffuseColorRgba.b)/0xff,
             ((float)(100-((float)Obj->ObjectInfo3ds->Materials[Obj->ObjectInfo3ds->FaceMaterial[i]]->Transparency)))/100};  
            float mat_specular[]=
            {((float)Obj->ObjectInfo3ds->Materials[Obj->ObjectInfo3ds->FaceMaterial[i]]->SpecularColorRgba.r)/0xff, 
             ((float)Obj->ObjectInfo3ds->Materials[Obj->ObjectInfo3ds->FaceMaterial[i]]->SpecularColorRgba.g)/0xff, 
             ((float)Obj->ObjectInfo3ds->Materials[Obj->ObjectInfo3ds->FaceMaterial[i]]->SpecularColorRgba.b)/0xff,
             ((float)(100-((float)Obj->ObjectInfo3ds->Materials[Obj->ObjectInfo3ds->FaceMaterial[i]]->Transparency)))/100}; 
            float mat_selfillum[]=
            {((float)Obj->ObjectInfo3ds->Materials[Obj->ObjectInfo3ds->FaceMaterial[i]]->SelfIllumColorRgba.r)/0xff, 
             ((float)Obj->ObjectInfo3ds->Materials[Obj->ObjectInfo3ds->FaceMaterial[i]]->SelfIllumColorRgba.g)/0xff, 
             ((float)Obj->ObjectInfo3ds->Materials[Obj->ObjectInfo3ds->FaceMaterial[i]]->SelfIllumColorRgba.b)/0xff,
             ((float)(100-((float)Obj->ObjectInfo3ds->Materials[Obj->ObjectInfo3ds->FaceMaterial[i]]->Transparency)))/100};

            if(Obj->ObjectInfo3ds->Materials[Obj->ObjectInfo3ds->FaceMaterial[i]]->AmbientColorOn)
              glMaterialfv (GL_FRONT, GL_AMBIENT, mat_ambient);
            if(Obj->ObjectInfo3ds->Materials[Obj->ObjectInfo3ds->FaceMaterial[i]]->DiffuseColorOn)
              glMaterialfv (GL_FRONT, GL_DIFFUSE, mat_diffuse);
            if(Obj->ObjectInfo3ds->Materials[Obj->ObjectInfo3ds->FaceMaterial[i]]->SpecularColorOn)
              glMaterialfv (GL_FRONT, GL_SPECULAR, mat_specular);
            if(Obj->ObjectInfo3ds->Materials[Obj->ObjectInfo3ds->FaceMaterial[i]]->SelfIllumColorOn)
              glMaterialfv (GL_FRONT, GL_EMISSION, mat_selfillum);
            glMaterialf (GL_FRONT, GL_SHININESS,1.28*((float)Obj->ObjectInfo3ds->Materials[Obj->ObjectInfo3ds->FaceMaterial[i]]->Shininess));
          } 

          if(Obj->ObjectInfo3ds->Materials[Obj->ObjectInfo3ds->FaceMaterial[i]]->Transparency)
          {
            bTransparent=1;
            glEnable(GL_BLEND); 
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glDepthMask(GL_FALSE); 
          }
        }
      }

      glBegin(GL_TRIANGLES);

      Obj->GetWorldVertexNormal(Obj->ObjectTriangles[i].a,&Normal);
      Normal.x+=Obj->WorldObjectVertexes[Obj->ObjectTriangles[i].a].x;
      Normal.y+=Obj->WorldObjectVertexes[Obj->ObjectTriangles[i].a].y;
      Normal.z+=Obj->WorldObjectVertexes[Obj->ObjectTriangles[i].a].z;
      Camera->VertexFromCamera(&Normal,&Normal);
      Normal.x-=CameraVertexa.x;
      Normal.y-=CameraVertexa.y;
      Normal.z-=CameraVertexa.z;
      glNormal3f(Normal.x,Normal.y,Normal.z);
      glVertex3f( CameraVertexa.x,    
                  CameraVertexa.y,    
                  CameraVertexa.z );

      Obj->GetWorldVertexNormal(Obj->ObjectTriangles[i].b,&Normal);
      Normal.x+=Obj->WorldObjectVertexes[Obj->ObjectTriangles[i].b].x;
      Normal.y+=Obj->WorldObjectVertexes[Obj->ObjectTriangles[i].b].y;
      Normal.z+=Obj->WorldObjectVertexes[Obj->ObjectTriangles[i].b].z;
      Camera->VertexFromCamera(&Normal,&Normal);
      Normal.x-=CameraVertexb.x;
      Normal.y-=CameraVertexb.y;
      Normal.z-=CameraVertexb.z;
      glNormal3f(Normal.x,Normal.y,Normal.z);
      glVertex3f( CameraVertexb.x,    
                  CameraVertexb.y,    
                  CameraVertexb.z );

      Obj->GetWorldVertexNormal(Obj->ObjectTriangles[i].c,&Normal);
      Normal.x+=Obj->WorldObjectVertexes[Obj->ObjectTriangles[i].c].x;
      Normal.y+=Obj->WorldObjectVertexes[Obj->ObjectTriangles[i].c].y;
      Normal.z+=Obj->WorldObjectVertexes[Obj->ObjectTriangles[i].c].z;
      Camera->VertexFromCamera(&Normal,&Normal);
      Normal.x-=CameraVertexc.x;
      Normal.y-=CameraVertexc.y;
      Normal.z-=CameraVertexc.z;
      glNormal3f(Normal.x,Normal.y,Normal.z);
      glVertex3f( CameraVertexc.x,    
                  CameraVertexc.y,    
                  CameraVertexc.z );
      
      glEnd();

      if(bTransparent)
      {
        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
      }
    }
#endif
  }
  return ENG3DERR_NO_ERROR;
}

void C3DOpenglGlut::AssignCamera(C3DCamera * CameraParam)
{
  Camera = CameraParam;
}

ENG3DERR C3DOpenglGlut::Stop()
{
  FinishApp=1;
  while(StartRunning);
  return ENG3DERR_NO_ERROR;
}

ENG3DERR C3DOpenglGlut::Start(char * WindowName,
                          unsigned int ParamWindowWidth,
                          unsigned int ParamWindowHeight,
                          unsigned int ParamWindowX,
                          unsigned int ParamWindowY,
                          TDisplayCallback _DisplayCallback,
                          TIdleCallback _IdleCallback,
                          TKeyboardCallback _KeyboardCallback,
                          TReshapeCallback _ReshapeCallback,
                          TMouseCallback _MouseCallback,
                          C3DCamera * CameraParam,
                          CLightFocus * Focus0,
                          CLightFocus * Focus1,
                          CLightFocus * Focus2,
                          CLightFocus * Focus3,
                          CLightFocus * Focus4,
                          CLightFocus * Focus5,
                          CLightFocus * Focus6,
                          CLightFocus * Focus7,
                          HWND parent)
{
  int _argc=1;
  char * _argv[]={""};
  
  StartRunning=1;

  Camera = CameraParam;

  LightFocus[0]=Focus0;
  LightFocus[1]=Focus1;
  LightFocus[2]=Focus2;
  LightFocus[3]=Focus3;
  LightFocus[4]=Focus4;
  LightFocus[5]=Focus5;
  LightFocus[6]=Focus6;
  LightFocus[7]=Focus7;

  DisplayCallback =_DisplayCallback;
  IdleCallback =_IdleCallback;
  KeyboardCallback =_KeyboardCallback;
  ReshapeCallback =_ReshapeCallback;
  MouseCallback = _MouseCallback;

  WindowWidth=ParamWindowWidth;
  WindowHeight=ParamWindowHeight;
  
  /*
  glutInit(&_argc, _argv);    
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(WindowWidth,WindowHeight);
  glutInitWindowPosition(0,0);
  glutCreateWindow(WindowName);
  
  if(DisplayCallback)glutDisplayFunc(DisplayCallback);
  if(IdleCallback)glutIdleFunc(IdleCallback);
  if(ReshapeCallback)glutReshapeFunc(ReshapeCallback);
  if(KeyboardCallback)glutKeyboardFunc(KeyboardCallback);
  if(SpecialCallback)glutSpecialFunc(SpecialCallback);
  if(MouseCallback)glutMouseFunc(MouseCallback);
  */

  CreateEng3dWindow(WindowName,ParamWindowWidth,ParamWindowHeight,ParamWindowX,ParamWindowY,16,0,parent);

  glClearColor(0.0, 0.0, 0.0, 0.0); // Clear the background color to dark blue

	glShadeModel(GL_SMOOTH);

  glHint(GL_POLYGON_SMOOTH_HINT,GL_NICEST);
  
  unsigned int i=0;
  for(i=0;i<8;i++)
  {
    if(LightFocus[i])
    {
      glLightfv (GL_LIGHT0+i, GL_AMBIENT, LightFocus[i]->ambient);
      glLightfv (GL_LIGHT0+i, GL_DIFFUSE, LightFocus[i]->diffuse);
      glLightfv (GL_LIGHT0+i, GL_SPECULAR, LightFocus[i]->specular);
      glLightfv (GL_LIGHT0+i, GL_POSITION, LightFocus[i]->position);
      glEnable (GL_LIGHTING);
      glEnable (GL_LIGHT0+i);
    }
  }
  
  glDepthFunc(GL_LESS);
  glEnable(GL_DEPTH_TEST); // We enable the depth test (also called z buffer)

  // Polygon rasterization mode (polygon filled)
  glPolygonMode (GL_FRONT_AND_BACK, GL_FILL); 
  // Viewport transformation 
  glViewport(0,0,WindowWidth,WindowHeight); 
  // Projection transformation
  glMatrixMode(GL_PROJECTION); // Specifies which matrix stack is the target for matrix operations 
  glLoadIdentity(); // We initialize the projection matrix as identity
  gluPerspective(45.0f,(GLfloat)WindowWidth/(GLfloat)WindowHeight,1.0f,1000.0f); // We define the "viewing volume"
 
  glMatrixMode(GL_MODELVIEW);
  //gluLookAt(0,0,0,0,0,1,0,1,0); 
  glLoadIdentity();

  //glutMainLoop();
  
  FinishApp=0;
  MSG msg;

  while(!FinishApp)                  // Bucle que se ejecuta mientras bAplicacionFinalizada=FALSE
  {
    if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))   // ¿Hay un mensaje esperando? 
    {
      if (msg.message==WM_QUIT)               // ¿Hemos recibido el mansaje de SALIDA?
      {
         FinishApp=1;         // Si es, bAplicacionFinalizada=TRUE
      }
      else                                    // Sino lo pasamas al gestor de mensajes
      {
         TranslateMessage(&msg);             // Traducimos el mensaje
         DispatchMessage(&msg);              // Lo procesamos
      }
        
      if(!FinishApp && DisplayCallback)DisplayCallback();
    }
    else                                        // Sino hay mensajes llama a Display
    {
      if(DisplayCallback)DisplayCallback();
    }
  }

  DestroyEng3dWindow();
  StartRunning=0;
  return ENG3DERR_NO_ERROR;
}


//---------------------------------------------------------------
// Nombre: EliminaVentanaGL
// Descripcion: Destruye toda la información sobre la ventana GL
// Parametros: Ninguno
//---------------------------------------------------------------
void C3DOpenglGlut::DestroyEng3dWindow(void)				
{
	if (m_bFullScreen)							// ¿Estamos en modo pantalla completa?
	{
		ChangeDisplaySettings(NULL,0);					// Volvemos al escritorio 
		ShowCursor(TRUE);								// Muestra el cursor 
	}

	if (m_hRC)											// ¿Hay un Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// ¿Podemos eliminar el DC Context y el RC Context?
		{
			//MessageBox(NULL,"Fallo al eliminar el DC y el RC.","ERROR DE SALIDA",MB_OK | MB_ICONINFORMATION);
      __asm nop;
		}

		if (!wglDeleteContext(m_hRC))					// ¿Podemos eliminar el RC?
		{
			//MessageBox(NULL,"Fallo al eliminar el RC.","ERROR DE SALIDA",MB_OK | MB_ICONINFORMATION);
      __asm nop;
		}
		m_hRC=NULL;									
	}

	if (m_hDC && !ReleaseDC(m_hWnd,m_hDC))				// ¿Podemos eliminar el DC?
	{
		//MessageBox(NULL,"Fallo al eliminar Release Device Context.","ERROR DE SALIDA",MB_OK | MB_ICONINFORMATION);
		m_hDC=NULL;									
	}

	if (m_hWnd && !DestroyWindow(m_hWnd))				// ¿Podemos eliminar la ventana?
	{
		//MessageBox(NULL,"No podemos eliminar m_hWnd.","ERROR DE SALIDA",MB_OK | MB_ICONINFORMATION);
    __asm nop;
  }

  m_hWnd=NULL;

	if (!UnregisterClass("OpenGLxx",m_hInstance))			// ¿Podemos "desregistrar" la clase
	{
    GetLastError();
		//MessageBox(NULL,"No podemos 'desregistrar' la clase.","ERROR DE SALIDA",MB_OK | MB_ICONINFORMATION);
		m_hInstance=NULL;							
	}
}

C3DOpenglGlut * g_self=NULL;

LRESULT CALLBACK MessagesManager(HWND h, UINT m,	WPARAM w, LPARAM l)
{
  switch(m)
  {
    case WM_CREATE:
    {
      CREATESTRUCT * cs = (CREATESTRUCT*)l;
      g_self=(C3DOpenglGlut *)cs->lpCreateParams;
      return 1;
    }

		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;						
		}

    case WM_SIZE:
    {
      if(g_self && g_self->ReshapeCallback)g_self->ReshapeCallback(LOWORD(l),HIWORD(l));   
      return 1;		
    }

    case WM_KEYDOWN:
    {
      POINT p;
      GetCursorPos(&p);
      if(g_self)g_self->bKeys[w]=1;
      if(g_self && g_self->KeyboardCallback)g_self->KeyboardCallback(g_self->bKeys,w,p.x,p.y);
      return 1;
    }

    case WM_KEYUP:
    {
      if(g_self)g_self->bKeys[w]=0;
      return 1;
    }

    case WM_MOUSEMOVE:
    {
      if(g_self && g_self->MouseCallback) g_self->MouseCallback(w,LOWORD(l),HIWORD(l));
    }  
  };

  return DefWindowProc(h,m,w,l);
}

WINGDIAPI int   WINAPI wglChoosePixelFormat( __in HDC hdc, __in CONST PIXELFORMATDESCRIPTOR *ppfd);

int C3DOpenglGlut::CreateEng3dWindow(char * Titulo, 
                                      int ancho, 
                                      int alto,
                                      int x,
                                      int y, 
                                      int bits, 
                                      int fullscreen,
                                      HWND parent)
{
  g_self=this;

	GLuint		PixelFormat;			
	WNDCLASS	wc;						// Estructura de la clase ventana (Windows Class Structure)
	DWORD		dwExStyle;				// Estilo extendido de ventana 
	DWORD		dwStyle;				// Estilo de ventana 
	RECT		WindowRect;				// Graba el rectangulo de valores Arriba-izquierda/Abajo-derecha
	WindowRect.left=(long)0;			// Pone el valor Left a 0
	WindowRect.right=(long)ancho;		// Pone el valor Right al valor pasado en ancho
	WindowRect.top=(long)0;				// Pone el valor Top a 0
	WindowRect.bottom=(long)alto;		// Pone el valor Bottom al valor pasado como alto

  memset(bKeys,0,sizeof(bKeys));

	m_bFullScreen=fullscreen;			// Establece el flag global de pantalla completa

	m_hInstance			= GetModuleHandle(NULL);				// Graba una instancia para nuestra ventana 
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;										// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) MessagesManager;				// Manejador de mensajes WndProc
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= m_hInstance;							// Establece la instancia
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Carga del Icono por defecto
	
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Carga del puntero de flecha
	wc.hbrBackground	= NULL;									// No hace falta un fondo para GL
	wc.lpszMenuName		= NULL;									// No queremos un menú
	wc.lpszClassName	= "OpenGLxx";								// Establece el nombre de la clasae

	if (!RegisterClass(&wc))									// Intentamos registrar la clase
	{
		//MessageBox(NULL,"Fallo al registrar la clase ventana.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// Devolvemos FALSE
	}
	
	if (fullscreen)												// Intentamos modo pantalla completa
	{
		DEVMODE dmScreenSettings;								// Modo del dispositivo
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Nos aseguramos de la memoria
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Tamaño de la estructura Devmode
		dmScreenSettings.dmPelsWidth	= ancho;				// Ancho de la pantalla
		dmScreenSettings.dmPelsHeight	= alto;					// Altura de la pantalla
		dmScreenSettings.dmBitsPerPel	= bits;					// Bits por pixel seleccionados
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// Intentamos cargar el modo seleccionado y obtener los resultados.
		// NOTA: CDS_FULLSCREEN es usado para eliminar la barra de inicio
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// Si el modo falla. Hay dos opciones: Salir o poner modo ventana
			if (MessageBoxA(NULL,"El modo pantalla completa no es soportado por\nSu tarjeta gráfica. ¿Desea usar modo ventana?","¿Ventana? :-)",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;		// Modo ventana seleccionado. Fullscreen = FALSE
			}
			else
			{
				// Mostramos un mensaje y cerramos
				//MessageBox(NULL,"El programa cerrará.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;									// Devolver FALSE
			}
		}
	}
	
	//ShowCursor(FALSE);   
	
	if (fullscreen)												// Estamos en modo pantalla completa?
	{
		dwExStyle=WS_EX_APPWINDOW;								// Estilo extendido de ventana (Window Extended Style)
		dwStyle=WS_POPUP;										// Estilo de ventana
		ShowCursor(FALSE);										// Ocultamos el cursor del ratón
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Estilo extendido de ventana (Window Extended Style)
		
    if(!parent)  // Estilo de ventana
      dwStyle=WS_OVERLAPPEDWINDOW;
    else
      dwStyle=WS_CHILDWINDOW;							
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Ajustamos la ventana al tamaño solicitado

	// Creando la ventana
	m_hWnd=CreateWindowEx(	dwExStyle,						// Estilo extendido de ventana (Window Extended Style)
								"OpenGLxx",							// Nombre de la clase
								Titulo,								// Titulo de la ventana
								dwStyle |							// Estilo de ventana definido
								WS_CLIPSIBLINGS |					// Estilo de ventana requerido
								WS_CLIPCHILDREN,					// Estilo de ventana requerido
								x, y,								// Posición de la ventana
								WindowRect.right-WindowRect.left,	// Calculamos el ancho de la ventana
								WindowRect.bottom-WindowRect.top,	// Calculamos el alto de la ventana
								parent,								// ventana padre si es que tiene
								NULL,								// sin menu
								m_hInstance,						// instancia
								this);								// This to WM_CREATE
	if (!m_hWnd)
  {
		GetLastError();
    DestroyEng3dWindow();							
		//MessageBox(NULL,"Error en la creación de la ventana.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Devolver FALSE
	}
		
	static	PIXELFORMATDESCRIPTOR pfd=				
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Tamaño del descriptor del formato de pixel
		1,											// Numero de version
		PFD_DRAW_TO_WINDOW |						// El formato debe soportar Ventanas
		PFD_SUPPORT_OPENGL |						// El formato debe soportar OpenGL
		PFD_DOUBLEBUFFER,							// Soporta doble buffer
		PFD_TYPE_RGBA,								// Pedimos un formato RGBA
		bits,										// Seleccionamos la profundidad del color
		0, 0, 0, 0, 0, 0,							// Ignoramos los color bits
		0,											// No queremos buffer alpha
		0,											// Ignoramos Shift Bit
		0,											// No queremos buffer de Acumulación
		0, 0, 0, 0,									// Ignoramos Accumulation Bits
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No usamos buffer de pincel
		0,											// No usamos buffer auxiliar
		PFD_MAIN_PLANE,								// Layer principal de dibujo
		0,											// Reservado
		0, 0, 0										// Ignoramos Layer Masks
	};
	
	if (!(m_hDC=GetDC(m_hWnd)))						// Hemos obtenido el contexto del dispositivo?
	{
		DestroyEng3dWindow();							
		//MessageBox(NULL,"No podemos crear un GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Devolver FALSE
	}
	
	if (!(PixelFormat=ChoosePixelFormat(m_hDC,&pfd)))	// Hemos obtenido el formato de pixel?
	{
		DestroyEng3dWindow();							
		//MessageBox(NULL,"No podemos encontrar un formato de pixel aceptable.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Devolver FALSE
	}
	
	if(!SetPixelFormat(m_hDC,PixelFormat,&pfd))		// ¿Podemos establecer el formato de pixel?
	{
		DestroyEng3dWindow();						
		//MessageBox(NULL,"No se puede establacer el formato de Pixel.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							
	}
	
	if (!(m_hRC=wglCreateContext(m_hDC)))			// Podemos obtener un contexto de renderización?
	{
		DestroyEng3dWindow();						
		//MessageBox(NULL,"No se puede crear un GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							
	}
	
	if(!wglMakeCurrent(m_hDC,m_hRC))				// Vamos a activar el contexto de rend.
	{
		DestroyEng3dWindow();						
		//MessageBox(NULL,"No se puede activar el GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							
	}

	ShowWindow(m_hWnd,SW_SHOW);						// Mostrar la ventana
	SetForegroundWindow(m_hWnd);					// Prioridad total
	SetFocus(m_hWnd);								// Establecemos el foco del teclado a la ventana
	
  //InicializaEscenaGL(ancho, alto);				// Inicializamos la escena en perspectiva GL

	/*
  if (!InicializaGL())							// Inicializamos ventana GL
	{
		DestroyEng3dWindow();						
		MessageBox(NULL,"Fallo en la inicialización.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;					
	}
  */

	return TRUE;									// Todo correcto
}