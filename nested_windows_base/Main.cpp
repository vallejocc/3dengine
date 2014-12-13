
/***************************************************************************************************************
  main.c

  In this module its managed the main window, container for other controls. In addition they r managed received 
  messages to the window, giving them to the bridge or anything. Here its initialized the bridge and other things.
  It will manage the menu of the main window and other characteristics related to this window.

****************************************************************************************************************/

#include <windows.h>
#include "util.h"
#include "main.h"
#include "resource.h"
#include "controls.h"

GLOBAL_DATA GlobalData={0};


CONTROLSNOTIFYMSGCALLBACK NotifyCallbacks[] = {CONTROLS_ListViewNotifyCallback,
                                               CONTROLS_EditBoxCommandLineNotifyCallback};

#define N_NOTIFY_CALLBACKS (sizeof(NotifyCallbacks)/sizeof(CONTROLSNOTIFYMSGCALLBACK))

typedef struct  
{
  int nothing;  

}ITEM_CALLBACK_STRUCT;

typedef unsigned int (*MENUITEM_CLICKEDCALLBACK)(ITEM_CALLBACK_STRUCT *);

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

unsigned int ConfigOptionACallback(ITEM_CALLBACK_STRUCT * s)
{

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

EXTRA_MENU_ITEM ExtraMenuConfigItems[] = 
{
  {"Option A",0,ConfigOptionACallback},
  {"Option B",0,ConfigOptionBCallback}
};

////////////////////////////////////////////////////////////////////////////////////

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

static unsigned int MAIN_CallBridgeMenuCallbacks(unsigned int itemid)
{
  unsigned int i=0,j=0;
  EXTRA_MENUS * ExtraMenus = GetExtraMenus();

  if(!ExtraMenus)
  {
    return 0;
  }

  for(i=0;i<ExtraMenus->nMenus;i++)
  {
    ExtraMenus->menus[i].hmenu = NULL;
    
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
    case CONTROL_LISTVIEW_ITEM_CLICKED_NOTIFICATION:
    {
      //ListViewItemClickedCallback(notifyinfo->iItem,notifyinfo->iSubItem,notifyinfo->uKeyFlags);
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

            /*////PRUEBA
            if(HIWORD(wParam)==EN_CHANGE)
            {
              char aaa[200]={0};
              char * aaaa[] = {aaa};
              
              *((short*)aaa) = 200;
              SendMessage(GlobalData.EditBoxWindow,(UINT) EM_GETLINE,0,(long)aaa);
              CONTROLS_AddElementToListViewControl(GlobalData.ListViewWindow,1,aaaa,200);
              break;
            }
            ////PRUEBA*/

            switch(LOWORD(wParam)) {

            case ID_MAINMENU_FILE_SELECT_FILE:
                
                GlobalData.LastFileSelected[0] = 0;
                SelectFileMenu(GlobalData.LastFileSelected);
                if(GlobalData.LastFileSelected[0])
                {
                  //BRIDGE_FileSelectedCallback(GlobalData.LastFileSelected);
                }
                break;

            case ID_MAINMENU_FILE_SELECT_DIRECTORY:

                GlobalData.LastDirectorySelected[0] = 0;
                SelectDirectoryMenu(hWnd,GlobalData.LastDirectorySelected);
                if(GlobalData.LastDirectorySelected[0])
                {
                  //BRIDGE_DirectorySelectedCallback(GlobalData.LastDirectorySelected);
                }
                break;

            default:

                if(MAIN_CallBridgeMenuCallbacks(LOWORD(wParam)))
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
           (5*GetSystemMetrics(SM_CXMAXIMIZED))/100,  //  X coordinate
           (5*GetSystemMetrics(SM_CXMAXIMIZED))/100,  //  Y coordinate
           (90*GetSystemMetrics(SM_CXMAXIMIZED))/100, //  Width
           (90*GetSystemMetrics(SM_CYMAXIMIZED))/100, //  Height
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

char * ListViewColums[]={"colum1","colum2"};
unsigned int ListViewColumsLens[]={250,250};

int __stdcall WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nShowCmd)
{
  do 
  {
    int err=0;
    MSG msg;

    // For that elements that we will create on the fly
    GlobalData.LastAssignedId = FIRST_ASSIGNED_ID;
    
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
                                                                 43,
                                                                 60,
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
                                                     80,
                                                     25,
                                                     10,
                                                     63,
                                                     "" )))
    {
      break;
    }


    if(!(GlobalData.EditBoxWindow = CONTROLS_AddEditBoxCommandLine ( GlobalData.MainWindow,
                                                     hInstance,
                                                     80,
                                                     3,
                                                     10,
                                                     90,
                                                     "Command Line..." )))
    {
      break;
    }


    if(!UpdateWindow(GlobalData.MainWindow))
      break;

    // main window loop
    while (err = GetMessage(&msg, NULL, 0, 0))  //  Put a message into MSG variable
    {   
        if(err != -1)
        {
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