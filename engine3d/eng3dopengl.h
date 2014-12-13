#ifndef __ENG3DOPENGL_H__
#define __ENG3DOPENGL_H__

#include <windows.h>
#include "eng3ddefs.h"
#include "eng3dpolygon.h"
#include "eng3dcamera.h"
#include "eng3dcolor.h"
#include "eng3dobject.h"
#include "eng3dlightfocus.h"

typedef void (*TDisplayCallback)(void); //optional callback for glutDisplayFunc (read opengl documentation)
typedef void (*TIdleCallback)(void); //optional callback for glutIdleFunc (read opengl documentation)
typedef void (*TKeyboardCallback)(bool * keysstate,unsigned char key, int x, int y); //optional callback for glutKeyboardFunc (read opengl documentation)
typedef void (*TReshapeCallback)(int width, int height); //optional callback for glutReshapeFunc (read opengl documentation)
typedef void (*TMouseCallback)(int flags, int x, int y); //optional callback for glutMouseFunc (read opengl documentation)

class C3DOpenglGlut
{

public:

  C3DOpenglGlut();
  ~C3DOpenglGlut();

  void AssignCamera(C3DCamera * CameraParam);

  void DoReshape(int width, int height);

  ENG3DERR ShowScene();
  ENG3DERR ClearScene();
  ENG3DERR AddTrianglesToScene(C3DObject*);

  ENG3DERR DebugAddBoxToScene(TVertex * v0,
                              TVertex * v1,
                              TVertex * v2,
                              TVertex * v3,
                              TVertex * v4,
                              TVertex * v5,
                              TVertex * v6,
                              TVertex * v7,
                              TColor * color);

  ENG3DERR DebugAddLineToScene(TVertex * v0,
                               TVertex * v1,
                               TColor * color);

  ENG3DERR Stop();

  ENG3DERR Start(char * WindowName,
                 unsigned int ParamWindowWidth,
                 unsigned int ParamWindowHeight,
                 unsigned int ParamWindowX,
                 unsigned int ParamWindowY,
                 TDisplayCallback _DisplayCallback=NULL,
                 TIdleCallback _IdleCallback=NULL,
                 TKeyboardCallback _KeyboardCallback=NULL,
                 TReshapeCallback _ReshapeCallback=NULL,
                 TMouseCallback _MouseCallback=NULL,
                 C3DCamera * CameraParam=NULL,
                 CLightFocus * Focus0=NULL,
                 CLightFocus * Focus1=NULL,
                 CLightFocus * Focus2=NULL,
                 CLightFocus * Focus3=NULL,
                 CLightFocus * Focus4=NULL,
                 CLightFocus * Focus5=NULL,
                 CLightFocus * Focus6=NULL,
                 CLightFocus * Focus7=NULL,
                 HWND parent=NULL);

  unsigned int WindowWidth;
  unsigned int WindowHeight;

  C3DCamera * Camera;
  CLightFocus * LightFocus[8];

  TDisplayCallback DisplayCallback;
  TIdleCallback IdleCallback;
  TKeyboardCallback KeyboardCallback;
  TReshapeCallback ReshapeCallback;
  TMouseCallback MouseCallback;

  bool bKeys[256];
  HWND			m_hWnd;

private:

  void DestroyEng3dWindow(void);
  int CreateEng3dWindow(char * , int , int ,int, int , int , int , HWND parent=NULL);

	int			m_bFullScreen;
	HDC				m_hDC;	
	HGLRC			m_hRC;	
	HINSTANCE	m_hInstance;
  unsigned int FinishApp;
  unsigned int StartRunning;
};

#endif