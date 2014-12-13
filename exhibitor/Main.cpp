
/***************************************************************************************************************
  main.c

  In this module its managed the main window, container for other controls. In addition they r managed received 
  messages to the window, giving them to the bridge or anything. Here its initialized the bridge and other things.
  It will manage the menu of the main window and other characteristics related to this window.

****************************************************************************************************************/

#include <windows.h>
#include <gl/gl.h>
#include <math.h>
#include "util.h"
#include "resource.h"
#include "controls.h"
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
#include "eng3dlightfocus.h"

#define FIRST_ASSIGNED_ID 60000

typedef struct  
{
  ATOM   RegisteredClassAtom; 
  HWND   MainWindow;
  HMENU  MainMenu;
  HWND   ListViewWindow;
  HWND   EditBoxWindow;
  HWND   StaticTextWindow;
  WORD   LastAssignedId;
  TCHAR  LastFileSelected[MAX_PATH];
  TCHAR  LastDirectorySelected[MAX_PATH];

  C3D3dsReader * Reader3ds;
  C3DCamera * Cam;
  C3DAutodrawWorld * AutoDrawWorld;
  CLightFocus * lf,*lf2,*lf3,*lf4,*lf5,*lf6,*lf7,*lf8;

  unsigned int ControlAmpli;
  unsigned int SelectedObject;
  
}GLOBAL_DATA,*PGLOBAL_DATA;

GLOBAL_DATA GlobalData={0};

CONTROLSNOTIFYMSGCALLBACK NotifyCallbacks[] = {CONTROLS_ListViewNotifyCallback,
                                               CONTROLS_EditBoxCommandLineNotifyCallback};

#define N_NOTIFY_CALLBACKS (sizeof(NotifyCallbacks)/sizeof(CONTROLSNOTIFYMSGCALLBACK))

typedef struct  
{
  int nothing;  

}ITEM_CALLBACK_STRUCT;

typedef unsigned int (*MENUITEM_CLICKEDCALLBACK)(ITEM_CALLBACK_STRUCT *);

typedef struct
{
  TCHAR * itemname; // item name assigned by iface client 
  unsigned int     itemid;   // item id assigned by iface server
  MENUITEM_CLICKEDCALLBACK itemcallback; // callback called when a item is clicked
} EXTRA_MENU_ITEM;

typedef struct
{
  TCHAR * menuname; // menu name assigned by iface client
  unsigned int nItems; // number of items 
  HMENU hmenu; //menu handle assigned by iface server
  EXTRA_MENU_ITEM * items; // items
} EXTRA_MENU;

typedef struct
{
  unsigned int nMenus; // number of menus 
  EXTRA_MENU * menus; // menus
} EXTRA_MENUS;

EXTRA_MENUS * GetExtraMenus();

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void KeepCrossSessionData()
{
  char thispath[MAX_PATH];
  GetModuleFileName(NULL,thispath,MAX_PATH);
  unsigned int i=0;
  while(thispath[i])i++;
  if(i<3)return;
  thispath[i-1]='t';thispath[i-2]='a';thispath[i-3]='d';
  FILE * f = fopen(thispath,"wb");
  if(f)
  {
    fwrite(GlobalData.LastFileSelected,1,MAX_PATH,f);
    fwrite(GlobalData.LastDirectorySelected,1,MAX_PATH,f);
    fclose(f);
  }
}

void GetCrossSessionData()
{
  char thispath[MAX_PATH];
  GetModuleFileName(NULL,thispath,MAX_PATH);
  unsigned int i=0;
  while(thispath[i])i++;
  if(i<3)return;
  thispath[i-1]='t';thispath[i-2]='a';thispath[i-3]='d';
  FILE * f = fopen(thispath,"rb");
  if(f)
  {
    fread(GlobalData.LastFileSelected,1,MAX_PATH,f);
    fread(GlobalData.LastDirectorySelected,1,MAX_PATH,f);
    fclose(f);
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////

void ResetIface()
{
  GlobalData.lf->ApplyCam=GlobalData.lf2->ApplyCam=GlobalData.lf3->ApplyCam=0;
 
  unsigned int i=0,j=0;
  EXTRA_MENUS * ExtraMenus = GetExtraMenus();

  if(!ExtraMenus)
  {
    return;
  }

  for(i=0;i<ExtraMenus->nMenus;i++)
  {
    if(!strcmp(ExtraMenus->menus[i].menuname,"Configuration"))
    {
      for(j=0;j<ExtraMenus->menus[i].nItems;j++)
      {
        if(!strcmp(ExtraMenus->menus[i].items[j].itemname,"Light fixed to camera"))
        {
          CheckMenuItem(ExtraMenus->menus[i].hmenu,ExtraMenus->menus[i].items[j].itemid,MF_UNCHECKED);
        }
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////

unsigned int ConfigLightFixedToCameraCallback(ITEM_CALLBACK_STRUCT * s)
{
  GlobalData.lf->ApplyCam=GlobalData.lf2->ApplyCam=GlobalData.lf3->ApplyCam=0;
 
  unsigned int i=0,j=0;
  EXTRA_MENUS * ExtraMenus = GetExtraMenus();

  if(!ExtraMenus)
  {
    return 0;
  }

  for(i=0;i<ExtraMenus->nMenus;i++)
  {
    if(!strcmp(ExtraMenus->menus[i].menuname,"Configuration"))
    {
      for(j=0;j<ExtraMenus->menus[i].nItems;j++)
      {
        if(!strcmp(ExtraMenus->menus[i].items[j].itemname,"Light fixed to camera"))
        {
          if(MF_CHECKED==CheckMenuItem(ExtraMenus->menus[i].hmenu,ExtraMenus->menus[i].items[j].itemid,MF_CHECKED))
          {
            CheckMenuItem(ExtraMenus->menus[i].hmenu,ExtraMenus->menus[i].items[j].itemid,MF_UNCHECKED);
            GlobalData.lf->ApplyCam=GlobalData.lf2->ApplyCam=GlobalData.lf3->ApplyCam=1;
          }
          else
          {
            GlobalData.lf->ApplyCam=GlobalData.lf2->ApplyCam=GlobalData.lf3->ApplyCam=0;
          }
        }
      }
    }
  }

  return 0;
}

unsigned int ConfigOptionBCallback(ITEM_CALLBACK_STRUCT * s)
{
  return 0;
}

unsigned int CommandsOptionACallback(ITEM_CALLBACK_STRUCT * s)
{
  return 0;
}

unsigned int CommandsOptionBCallback(ITEM_CALLBACK_STRUCT * s)
{
  return 0;
}

////////////////////////////////////////////////////////////////////////////////////

void Display(void)
{
  GlobalData.AutoDrawWorld->ClearScene();
  GlobalData.AutoDrawWorld->AddAllWorldObjectsToScene();
  GlobalData.AutoDrawWorld->ShowScene();
}

void Reshape(int width, int height)
{
  GlobalData.AutoDrawWorld->DoReshape(width,height);
}

void Keyboard(bool * keysstate,unsigned char key, int x, int y)
{ 
  TObjectList * ObjNode;
  ObjNode = (TObjectList *)GlobalData.AutoDrawWorld->ObjectListHead.Flink;
  unsigned int testcol=0;
  TObjectList * ObjCol1=NULL;


  switch(key)
  {
    case 0x26: //up
    {
      if(keysstate['H'])
        GlobalData.lf->Orig.y+=GlobalData.ControlAmpli;
      else
        GlobalData.Cam->RotateCameraOverOwnOriginX(-0.02);
    }
    break;
    
    case 0x28: //down
    {
      if(keysstate['H'])
        GlobalData.lf->Orig.y-=GlobalData.ControlAmpli;
      else
        GlobalData.Cam->RotateCameraOverOwnOriginX(0.02);
    }
    break;

    case 0x25: //left
    {
      if(keysstate['H'])
        GlobalData.lf->Orig.x-=GlobalData.ControlAmpli;
      else
        GlobalData.Cam->RotateCameraOverOwnOriginY(-0.02);
    }
    break;
    
    case 0x27: //right
    {
      if(keysstate['H'])
        GlobalData.lf->Orig.x+=GlobalData.ControlAmpli;
      else
        GlobalData.Cam->RotateCameraOverOwnOriginY(0.02);
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
        GlobalData.lf->ambient[0]+=0.02*GlobalData.ControlAmpli;
        GlobalData.lf->ambient[1]+=0.02*GlobalData.ControlAmpli;
        GlobalData.lf->ambient[2]+=0.02*GlobalData.ControlAmpli;
        GlobalData.lf->diffuse[0]+=0.02*GlobalData.ControlAmpli;
        GlobalData.lf->diffuse[1]+=0.02*GlobalData.ControlAmpli;
        GlobalData.lf->diffuse[2]+=0.02*GlobalData.ControlAmpli;
        GlobalData.lf->specular[0]+=0.02*GlobalData.ControlAmpli;
        GlobalData.lf->specular[1]+=0.02*GlobalData.ControlAmpli;
        GlobalData.lf->specular[2]+=0.02*GlobalData.ControlAmpli;
      }
      else
        GlobalData.ControlAmpli++;
      break;

    case 'm':

      if(keysstate['H'])
      {
        GlobalData.lf->ambient[0]-=0.02*GlobalData.ControlAmpli;
        GlobalData.lf->ambient[1]-=0.02*GlobalData.ControlAmpli;
        GlobalData.lf->ambient[2]-=0.02*GlobalData.ControlAmpli;
        GlobalData.lf->diffuse[0]-=0.02*GlobalData.ControlAmpli;
        GlobalData.lf->diffuse[1]-=0.02*GlobalData.ControlAmpli;
        GlobalData.lf->diffuse[2]-=0.02*GlobalData.ControlAmpli;
        GlobalData.lf->specular[0]-=0.02*GlobalData.ControlAmpli;
        GlobalData.lf->specular[1]-=0.02*GlobalData.ControlAmpli;
        GlobalData.lf->specular[2]-=0.02*GlobalData.ControlAmpli;
      }
      else
        if(GlobalData.ControlAmpli)GlobalData.ControlAmpli--;
      break;

    case 'S':
    {
      GlobalData.Cam->TraslateCameraUp(0.3*GlobalData.ControlAmpli); 
    }
    break;

    case 'X':
    {
      GlobalData.Cam->TraslateCameraUp(-0.3*GlobalData.ControlAmpli);
    }
    break;

    case 'Z':
    {
      GlobalData.Cam->TraslateCameraUVector(0.3*GlobalData.ControlAmpli); 
    }
    break;

    case 'C':
    {
      GlobalData.Cam->TraslateCameraUVector(-0.3*GlobalData.ControlAmpli); 
    }
    break;

    case 'G':
    {
      GlobalData.Cam->TraslateCameraEye(0.3*GlobalData.ControlAmpli);
    }
    break;

    case 'B':
    {
      GlobalData.Cam->TraslateCameraEye(-0.3*GlobalData.ControlAmpli);
    }
    break;

    case 'Q':
    {
      GlobalData.Cam->RotateCameraOverOwnOriginZ(0.005*GlobalData.ControlAmpli);
    }
    break;

    case 'W':
    {
      GlobalData.Cam->RotateCameraOverOwnOriginZ(-0.005*GlobalData.ControlAmpli);
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
      if(GlobalData.SelectedObject==-1)
      {
        CONTROLS_SetStaticTextCommonInfoText(GlobalData.StaticTextWindow,"Select a object first!");
        break;
      }

      TObjectList * Temp = ENG3D_BASE_FROM_FIELD(TObjectList,Link,GlobalData.AutoDrawWorld->ObjectListHead.Flink);

      unsigned int i=0;

      for(i=0;i<GlobalData.SelectedObject;i++)
      {
        Temp = ENG3D_BASE_FROM_FIELD(TObjectList,Link,Temp->Link.Flink);

        if((void*)Temp == (void*)&GlobalData.AutoDrawWorld->ObjectListHead)
        {
          Temp = ENG3D_BASE_FROM_FIELD(TObjectList,Link,GlobalData.AutoDrawWorld->ObjectListHead.Flink);
        }
      }

      ObjCol1 = Temp;

      TVertex orig={0,0,0};
      TVertex CamOrig;
      TVertex u,v,w;
      GlobalData.Cam->GetCameraOrthoBasis(&u,&v,&w);
      GlobalData.Cam->GetCameraOrig(&CamOrig);
      TVertex oo;
      TVertex ou,ov,ow;
      Temp->Object->GetObjectOrthoBasis(&oo,&ou,&ov,&ow);
      TVertex oc;
      oc.x=oo.x;
      oc.y=oo.y;
      oc.z=oo.z;

      switch(key)
      {
        case 'I':
          Temp->Object->Traslate(v.x*0.2*GlobalData.ControlAmpli,
                                 v.y*0.2*GlobalData.ControlAmpli,
                                 v.z*0.2*GlobalData.ControlAmpli);
          testcol=1;
          break;

        case 'K':
          Temp->Object->Traslate(v.x*-0.2*GlobalData.ControlAmpli,
                                 v.y*-0.2*GlobalData.ControlAmpli,
                                 v.z*-0.2*GlobalData.ControlAmpli);
          testcol=1;
          break;

        case 'L':
          Temp->Object->Traslate(u.x*0.2*GlobalData.ControlAmpli,
                                 u.y*0.2*GlobalData.ControlAmpli,
                                 u.z*0.2*GlobalData.ControlAmpli);
          testcol=1;
          break;

        case 'J':
          Temp->Object->Traslate(-u.x*0.2*GlobalData.ControlAmpli,
                                 -u.y*0.2*GlobalData.ControlAmpli,
                                 -u.z*0.2*GlobalData.ControlAmpli);
          testcol=1;
          break;

        case '9':
          Temp->Object->Traslate(w.x*0.2*GlobalData.ControlAmpli,
                                 w.y*0.2*GlobalData.ControlAmpli,
                                 w.z*0.2*GlobalData.ControlAmpli);
          testcol=1;
          break;

        case 'O':
          Temp->Object->Traslate(w.x*-0.2*GlobalData.ControlAmpli,
                                 w.y*-0.2*GlobalData.ControlAmpli,
                                 w.z*-0.2*GlobalData.ControlAmpli);
          testcol=1;
          break;

        case '6':
          u.x+=oc.x;
          u.y+=oc.y;
          u.z+=oc.z;
          Temp->Object->RotateOverVector(0.2*GlobalData.ControlAmpli,&oc,&u);
          testcol=1;
          break;

        case '7':
          v.x+=oc.x;
          v.y+=oc.y;
          v.z+=oc.z;
          Temp->Object->RotateOverVector(0.2*GlobalData.ControlAmpli,&oc,&v);
          testcol=1;
          break;

        case '8':
          w.x+=oc.x;
          w.y+=oc.y;
          w.z+=oc.z;
          Temp->Object->RotateOverVector(0.2*GlobalData.ControlAmpli,&oc,&w);
          testcol=1;
          break;
      };
    }
    break;

    case 9: //TAB
    {
      //GlobalData.SelectedObject++;
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
    GlobalData.Cam->TraslateCameraEye(0.1*(lasty-y)*GlobalData.ControlAmpli);
    GlobalData.Cam->RotateCameraOverOwnOriginZ(0.005*(lastx-x)*GlobalData.ControlAmpli);
  }
  else
  {
    if(flags & MK_CONTROL)
    {
      GlobalData.Cam->RotateCameraOverOwnOriginX(-0.005*(lasty-y)*GlobalData.ControlAmpli);
      GlobalData.Cam->RotateCameraOverOwnOriginY(-0.005*(lastx-x)*GlobalData.ControlAmpli);
    }

    if(flags & MK_SHIFT)
    {
      GlobalData.Cam->TraslateCameraUp(0.1*(lasty-y)*GlobalData.ControlAmpli);
      GlobalData.Cam->TraslateCameraUVector(0.1*(lastx-x)*GlobalData.ControlAmpli);
    }
  }

  lastx=x;
  lasty=y;  
}

void DestroyCurrentOpenglSubwindow()
{
  CONTROLS_EmptyListViewControl(GlobalData.ListViewWindow);

  if(GlobalData.AutoDrawWorld)
  {
    GlobalData.AutoDrawWorld->Stop();
    delete GlobalData.AutoDrawWorld;
  }

  if(GlobalData.Reader3ds)
    delete GlobalData.Reader3ds;
  if(GlobalData.lf)
    delete GlobalData.lf;
  if(GlobalData.lf2)
    delete GlobalData.lf2;
  if(GlobalData.lf3)
    delete GlobalData.lf3;
  if(GlobalData.lf4)
    delete GlobalData.lf4;
  if(GlobalData.lf5)
    delete GlobalData.lf5;
  if(GlobalData.lf6)
    delete GlobalData.lf6;
  if(GlobalData.lf7)
    delete GlobalData.lf7;
  if(GlobalData.lf8)
    delete GlobalData.lf8;
  if(GlobalData.Cam)
    delete GlobalData.Cam;

  GlobalData.AutoDrawWorld=NULL;
  GlobalData.Cam=NULL;
  GlobalData.Reader3ds=NULL;
  GlobalData.lf=NULL;GlobalData.lf2=NULL;GlobalData.lf3=NULL;GlobalData.lf4=NULL;
  GlobalData.lf5=NULL;GlobalData.lf6=NULL;GlobalData.lf7=NULL;GlobalData.lf8=NULL;
}

unsigned int CreateOpenglSubwindowFrom3dsModel(char * modelpath)
{
  DestroyCurrentOpenglSubwindow();

  TVertex OrigCam = {0,0,0};
  TVertex DestCam = {0,0,1};
  TVertex UpCam = {0,1,0};
  GlobalData.Reader3ds = new C3D3dsReader(modelpath);
  GlobalData.Cam = new C3DCamera(&OrigCam,&DestCam,&UpCam);
  GlobalData.Cam->FixCamera(&OrigCam,&DestCam,&UpCam);

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

  GlobalData.lf = new CLightFocus(light_ambient,light_diffuse,light_specular,&LightOrig,&LightDest,1);
  GlobalData.lf2 = new CLightFocus(light_ambient2,light_diffuse2,light_specular2,&LightOrig2,&LightDest2,0);
  GlobalData.lf3 = new CLightFocus(light_ambient3,light_diffuse3,light_specular3,&LightOrig3,&LightDest3,0);
  
  GlobalData.AutoDrawWorld = new C3DAutodrawWorld;
  GlobalData.AutoDrawWorld->Initialize();

  ENG3D_LIST_ENTRY * TempLink;
  for(TempLink=GlobalData.Reader3ds->ObjectsListHead.Flink;
      TempLink!=&GlobalData.Reader3ds->ObjectsListHead;
      TempLink=TempLink->Flink)
  {
    tObjectInfo3ds * objinfo = ENG3D_BASE_FROM_FIELD(tObjectInfo3ds,Next,TempLink);
    if(objinfo->nObjectTriangles && objinfo->nObjectVertexes)
    {
      C3DObject* TmpObj = new C3DObject();
      TmpObj->Initialize(objinfo);
      GlobalData.AutoDrawWorld->AddObject(TmpObj);
      char * tempdata[] = {"Object",objinfo->name};
      CONTROLS_AddElementToListViewControl(GlobalData.ListViewWindow,2,tempdata,20);
    }
  }

  RECT rectangle;
  GetClientRect(GlobalData.MainWindow,&rectangle);
  GlobalData.AutoDrawWorld->Start("hxxxxxx",
                                  ((rectangle.right-rectangle.left)*58)/100,
                                  ((rectangle.bottom-rectangle.top)*82)/100,
                                  ((rectangle.right-rectangle.left)*41)/100,
                                  ((rectangle.bottom-rectangle.top)*1)/100,
                                  Display,Display,Keyboard,Reshape,Mouse,GlobalData.Cam,
                                  GlobalData.lf,GlobalData.lf2,GlobalData.lf3,NULL,NULL,NULL,NULL,NULL,
                                  GlobalData.MainWindow);

  return 0;
}

DWORD WINAPI CreateOpenglSubwindowFrom3dsModelThreadProc(LPVOID lpParameter)
{
  CreateOpenglSubwindowFrom3dsModel(GlobalData.LastFileSelected);
  return 0;
}

////////////////////////////////////////////////////////////////////////////////////

EXTRA_MENU_ITEM ExtraMenuConfigItems[] = 
{
  {"Light fixed to camera",0,ConfigLightFixedToCameraCallback},
  {"Option B",0,ConfigOptionBCallback}
};

EXTRA_MENU_ITEM ExtraMenuCommandsItems[] = 
{
  {"Option A",0,CommandsOptionACallback},
  {"Option B",0,CommandsOptionBCallback}
};

EXTRA_MENU ExtraMenusArray[] = 
{
  {
    "Configuration",
    sizeof(ExtraMenuConfigItems)/sizeof(EXTRA_MENU_ITEM),
    NULL,
    ExtraMenuConfigItems
  },

  {
    "Commands",
    sizeof(ExtraMenuCommandsItems)/sizeof(EXTRA_MENU_ITEM),
    NULL,
    ExtraMenuCommandsItems      
  },
};

EXTRA_MENUS ExtraMenusStruct = 
{
  sizeof(ExtraMenusArray)/sizeof(EXTRA_MENU),
  ExtraMenusArray
};

EXTRA_MENUS * GetExtraMenus()
{
  return &ExtraMenusStruct;
};

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

static unsigned int MAIN_AddBridgeMenus()
{
  unsigned int i=0,j=0;
  EXTRA_MENUS * ExtraMenus = GetExtraMenus();

  if(!ExtraMenus)
  {
    return -1;
  }

  for(i=0;i<ExtraMenus->nMenus;i++)
  {
    ExtraMenus->menus[i].hmenu = NULL;
    
    for(j=0;j<ExtraMenus->menus[i].nItems;j++)
    {
      ExtraMenus->menus[i].items[j].itemid = 0;      
    }
  }

  for(i=0;i<ExtraMenus->nMenus;i++)
  {
    ExtraMenus->menus[i].hmenu = CreatePopupMenu();
    
    if(!ExtraMenus->menus[i].hmenu)
    {
      return -1;
    }

    for(j=0;j<ExtraMenus->menus[i].nItems;j++)
    {
      ExtraMenus->menus[i].items[j].itemid = GlobalData.LastAssignedId;
      
      InsertMenu(ExtraMenus->menus[i].hmenu,
                 ExtraMenus->menus[i].items[j].itemid,
                 MF_BYCOMMAND|MF_STRING,
                 ExtraMenus->menus[i].items[j].itemid,
                 ExtraMenus->menus[i].items[j].itemname);

      GlobalData.LastAssignedId++;
    }

    AppendMenu(GlobalData.MainMenu,
               MF_POPUP,
               (unsigned int)ExtraMenus->menus[i].hmenu,
               ExtraMenus->menus[i].menuname);
  }

  DrawMenuBar(GlobalData.MainWindow);

  return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

static void MAIN_DeleteBridgeMenus()
{
  unsigned int i=0;
  EXTRA_MENUS * ExtraMenus = GetExtraMenus();

  if(!ExtraMenus)
  {
    return;
  }

  for(i=0;i<ExtraMenus->nMenus;i++)
  {
    if(ExtraMenus->menus[i].hmenu)
    {
      DestroyMenu(ExtraMenus->menus[i].hmenu);
    }
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

static unsigned int MAIN_CallMenuCallbacks(unsigned int itemid)
{
  unsigned int i=0,j=0;
  EXTRA_MENUS * ExtraMenus = GetExtraMenus();

  if(!ExtraMenus)
  {
    return 0;
  }

  for(i=0;i<ExtraMenus->nMenus;i++)
  {  
    for(j=0;j<ExtraMenus->menus[i].nItems;j++)
    {
      if(ExtraMenus->menus[i].items[j].itemid == itemid)
      {
        ITEM_CALLBACK_STRUCT callbackstruct;
        ExtraMenus->menus[i].items[j].itemcallback(&callbackstruct);
        return 1;
      }
    }
  }
  return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

static void MAIN_DistributeNotifications(int code,PCOMMON_CONTROL_NOTIFICATION_INFO notifyinfo)
{
  switch(code)
  {
    case CONTROL_LISTVIEW_KILLFOCUS_NOTIFICATION:
    {
      GlobalData.SelectedObject=-1;
      CONTROLS_SetStaticTextCommonInfoText(GlobalData.StaticTextWindow,"");
    }
    break;

    case CONTROL_LISTVIEW_ITEM_CLICKED_NOTIFICATION:
    {
      char TempText[300];
      CONTROLS_GetListViewText(GlobalData.ListViewWindow,
                               notifyinfo->iItem,
                               0,
                               TempText,
                               300);

      if(!strcmp(TempText,"Object"))
      {
        CONTROLS_GetListViewText(GlobalData.ListViewWindow,
                                 notifyinfo->iItem,
                                 1,
                                 TempText,
                                 300);
        
        ENG3D_LIST_ENTRY * Temp;
        unsigned int i=0;

        for(Temp = GlobalData.AutoDrawWorld->ObjectListHead.Flink;
            Temp != &GlobalData.AutoDrawWorld->ObjectListHead;
            Temp = Temp->Flink,i++)
        {
          TObjectList * TempObj = ENG3D_BASE_FROM_FIELD(TObjectList,Link,Temp);
          if(!strcmp(TempObj->Object->ObjectInfo3ds->name,TempText))
          {
            CONTROLS_SetStaticTextCommonInfoText(GlobalData.StaticTextWindow,TempObj->Object->ObjectInfo3ds->name);
            break;
          }
        }

        GlobalData.SelectedObject=i;
      }
      else
      {
        GlobalData.SelectedObject=-1;
        CONTROLS_SetStaticTextCommonInfoText(GlobalData.StaticTextWindow,"");
      }

    }
    break;
  
    case CONTROL_LISTVIEW_ITEM_DOUBLECLICKED_NOTIFICATION:
    {
      //ListViewItemDoubleClickedCallback(notifyinfo->iItem,notifyinfo->iSubItem,notifyinfo->uKeyFlags);
    }
    break;

    case CONTROL_EDITBOX_COMMANDLINE_CHANGED:
    {
      //EditBoxCommandLineChangedCallback(notifyinfo->text,notifyinfo->len);
    }
    break;

    case CONTROL_EDITBOX_COMMANDLINE_NEWLINE:
    {
      //EditBoxCommandLineNewLineCallback(notifyinfo->text,notifyinfo->len);
    }

    break;
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

static LRESULT CALLBACK MAIN_MainWndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
    ULONG i=0;
    COMMON_CONTROL_NOTIFICATION_INFO notifyinfo;
    ULONG code;

    if(message == WM_NOTIFY || message == WM_COMMAND) 
    {
      for(i=0;i<N_NOTIFY_CALLBACKS;i++)
      {
        if(CONTROL_UNKNOWN_NOTIFICATION != (code = NotifyCallbacks[i](message,wParam,lParam,(LPNMHDR)lParam,&notifyinfo)))
        {
          break;
        }
      }
  
      if(code)
      {
        MAIN_DistributeNotifications(code,&notifyinfo);
        return DefWindowProc(hWnd, message, wParam, lParam);
      }
    }

    switch (message) 
    {	
        case WM_CLOSE:	
          DestroyMenu(GlobalData.MainMenu);
          GlobalData.MainMenu = NULL;
          DestroyWindow(GlobalData.MainWindow); //  Calls WM_DESTROY
          GlobalData.MainWindow = NULL;
          break;

        case WM_DESTROY:					          //  If termination is imminent
          PostQuitMessage(0);			        //  Give up time share/give OS control
          break;

        case WM_COMMAND:

            switch(LOWORD(wParam)) {

            case ID_MAINMENU_FILE_SELECT_FILE:
                
                if(GlobalData.LastFileSelected[0])
                  SelectFileMenu(GlobalData.LastFileSelected,GlobalData.LastFileSelected);
                else
                  SelectFileMenu(GlobalData.LastFileSelected,"c:\\");
                ResetIface();
                KeepCrossSessionData();
                if(GlobalData.LastFileSelected[0])
                {
                  DWORD ThreadId;
                  CreateThread(NULL,
                               0,
                               CreateOpenglSubwindowFrom3dsModelThreadProc,
                               NULL,
                               0,
                               &ThreadId);
                }
                break;

            case ID_MAINMENU_FILE_SELECT_DIRECTORY:

                GlobalData.LastDirectorySelected[0] = 0;
                SelectDirectoryMenu(hWnd,GlobalData.LastDirectorySelected);
                KeepCrossSessionData();
                if(GlobalData.LastDirectorySelected[0])
                {
                  //BRIDGE_DirectorySelectedCallback(GlobalData.LastDirectorySelected);
                }
                break;

            default:

                if(MAIN_CallMenuCallbacks(LOWORD(wParam)))
                {
                  break;
                }

                return DefWindowProc(hWnd, message, wParam, lParam);
            
            };
        
            break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }


    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

static ATOM MAIN_RegisterIfaceClass(HINSTANCE hInstance)
{
   WNDCLASS wc;
   TCHAR * temp=NULL;

   
   wc.style         = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;		
   wc.lpfnWndProc   = (LRESULT (CALLBACK *)(HWND,UINT,WPARAM,LPARAM))MAIN_MainWndProc;
   wc.cbClsExtra    = 0;
   wc.cbWndExtra    = 0;
   wc.hInstance     = hInstance;
   wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
   wc.hCursor       = LoadCursor(NULL, IDC_ARROW);	
   wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
   wc.lpszMenuName  = "IFACE_DEFAULT_MENU_NAME";
   wc.lpszClassName = "IFACE_DEFAULT_CLASS_NAME";

   return RegisterClass(&wc);
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

static HWND MAIN_InitInstanceIfaceWindow(HINSTANCE hInstance)
{
   HMENU hmenu;   
   HWND  res;

   hmenu = LoadMenu(NULL,MAKEINTRESOURCE(IDR_MAIN_MENU));

   if(!hmenu)
   {
       return NULL;
   }
   
   res =   CreateWindow(
           "IFACE_DEFAULT_CLASS_NAME",            //  Name of registered class
           "IFACE_DEFAULT_MAIN_WINDOW_NAME", //  Window name/title
           WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX|WS_VISIBLE,
           (0*GetSystemMetrics(SM_CXMAXIMIZED))/100,  //  X coordinate
           (0*GetSystemMetrics(SM_CXMAXIMIZED))/100,  //  Y coordinate
           (99*GetSystemMetrics(SM_CXMAXIMIZED))/100, //  Width
           (99*GetSystemMetrics(SM_CYMAXIMIZED))/100, //  Height
           NULL,    				                          //  Handle to this window's parent
           hmenu,     				                        //  Handle to a menu
           hInstance,     			                      //  Handle to hInstance
           NULL);				                              //  Optional parameter to window
  
   //DestroyMenu(hmenu);

   GlobalData.MainMenu   = hmenu;
   GlobalData.MainWindow = res;

   return res;
}


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

char * ListViewColums[]={"Element type","Characteristics"};
unsigned int ListViewColumsLens[]={50,50};

int __stdcall WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nShowCmd)
{
  do 
  {
    int err=0;
    MSG msg;

    // Some initializations
    GlobalData.ControlAmpli=1;
    GlobalData.SelectedObject=-1;

    // For that elements that we will create on the fly
    GlobalData.LastAssignedId = FIRST_ASSIGNED_ID;
    
    // Recover data from other session
    GetCrossSessionData();

    // Register window class for the main window
    GlobalData.RegisteredClassAtom = MAIN_RegisterIfaceClass(hInstance);
    
    if(!GlobalData.RegisteredClassAtom)
      break;

    // Create the window for the main window
    if(!MAIN_InitInstanceIfaceWindow(hInstance))
      break;

    // Show the main window
    ShowWindow(GlobalData.MainWindow,SW_SHOW);

    if(0 != MAIN_AddBridgeMenus())
    {
      break;
    }

    // Add a listview control to the client area of the main window
    if(!(GlobalData.ListViewWindow = CONTROLS_AddListViewControl(GlobalData.MainWindow,
                                                                 hInstance,
                                                                 39,
                                                                 82,
                                                                 1,
                                                                 1,
                                                                 2, //nColums
                                                                 ListViewColums, //Colums
                                                                 ListViewColumsLens, //Colums lens
                                                                 "list view control")))
    {
      break;
    }


    if(!(GlobalData.StaticTextWindow = CONTROLS_AddStaticTextCommonInfo ( GlobalData.MainWindow,
                                                     hInstance,
                                                     98,
                                                     10,
                                                     1,
                                                     85,
                                                     "" )))
    {
      break;
    }


    if(!(GlobalData.EditBoxWindow = CONTROLS_AddEditBoxCommandLine ( GlobalData.MainWindow,
                                                     hInstance,
                                                     98,
                                                     3,
                                                     1,
                                                     95,
                                                     "Command Line..." )))
    {
      break;
    }
    
    if(!UpdateWindow(GlobalData.MainWindow))
      break;

    if(GlobalData.LastFileSelected[0])
    {
      DWORD ThreadId;
      CreateThread(NULL,
                   0,
                   CreateOpenglSubwindowFrom3dsModelThreadProc,
                   NULL,
                   0,
                   &ThreadId);
    }

    // main window loop
    while (err = GetMessage(&msg, NULL, 0, 0))  //  Put a message into MSG variable
    {   
        if(err != -1)
        {
          if(GlobalData.AutoDrawWorld && 
             GlobalData.AutoDrawWorld->m_hWnd && 
             (msg.message==WM_KEYDOWN || msg.message==WM_KEYUP))
          {
            RECT wr;
            POINT cursor;
            GetWindowRect(GlobalData.AutoDrawWorld->m_hWnd,&wr);
            GetCursorPos(&cursor);
            if(cursor.x>wr.left && cursor.x<wr.right && cursor.y>wr.top && cursor.y<wr.bottom)
            {
              PostMessage(GlobalData.AutoDrawWorld->m_hWnd,msg.message,msg.wParam,msg.lParam);
              continue;
            }
          }
          TranslateMessage(&msg);				//  Translate keyboard-specific messages
          DispatchMessage(&msg);				//  Send it off to the window procedure
        }
    }

  }while(0);

  MAIN_DeleteBridgeMenus();

  if(GlobalData.EditBoxWindow)
  {
    CONTROLS_DeleteEditBoxCommandLine(GlobalData.EditBoxWindow);
  }

  if(GlobalData.StaticTextWindow)
  {
    CONTROLS_DeleteStaticTextCommonInfo(GlobalData.StaticTextWindow);
  }

  if(GlobalData.ListViewWindow)
  {
    CONTROLS_DeleteListViewControl(GlobalData.ListViewWindow);
  }

  // if the window for the main window was created destroy it
  DestroyWindow(GlobalData.MainWindow);

  // if the window class of the main window was registered unregister it
  if(GlobalData.RegisteredClassAtom)
  {
    UnregisterClass("IFACE_DEFAULT_CLASS_NAME",hInstance);
    GlobalData.RegisteredClassAtom = 0;
  }

  return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////