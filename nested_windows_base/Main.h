#ifndef __MAIN_H__
#define __MAIN_H__

#include <windows.h>
#include <tchar.h>
#include "controls.h"

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
  
}GLOBAL_DATA,*PGLOBAL_DATA;

extern GLOBAL_DATA GlobalData;

#endif