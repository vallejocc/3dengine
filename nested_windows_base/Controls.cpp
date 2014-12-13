/*
  controls.cpp

  This module contains functions for adding controls to the client area of a given window. 	

*/

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>
#include "controls.h"
#include "resource.h"


void CONTROLS_DeleteElementFromListViewControl(HWND hcontrol,int item)
{
  ListView_DeleteItem(hcontrol,item);
}


int CONTROLS_AddElementToListViewControl(HWND hcontrol,
                                         unsigned int nColumns,
                                         TCHAR ** ColumnsText,
                                         unsigned int MaxLen)
{
    LVITEM   item;
    unsigned int      i=0;
    
    item.mask=LVIF_TEXT;
    item.iItem=0;
    item.iSubItem=0;
    item.pszText = ColumnsText[0];
    item.cchTextMax=MaxLen;
    
    item.iItem = ListView_InsertItem(hcontrol,&item);
    
    for(i=0;i<nColumns;i++)
    {        
      if(item.iSubItem=i)
      {
        item.pszText = ColumnsText[i];
        ListView_SetItem(hcontrol,&item);
      }
    }

    return 0;
}

void CONTROLS_DeleteListViewControl(HWND hcontrol)
{
  DestroyWindow(hcontrol);
}


HWND CONTROLS_AddListViewControl( HWND           parent,
                                  HINSTANCE      hInstance,
                                  unsigned int   WidthPercent,
                                  unsigned int   HeightPercent,
                                  unsigned int   XFromStartOfClientAreaPercent,
                                  unsigned int   YFromStartOfClientAreaPercent,
                                  unsigned int   nColumns,
                                  TCHAR**        ColumnsNames,
                                  unsigned int * ColumnsSizes,
                                  TCHAR *        WindowName )
{
  RECT rectangle;
  HWND lvhwnd;
  LVCOLUMN col;
  unsigned int i;

  if(!parent || !WidthPercent || !HeightPercent || !WindowName || !GetClientRect(parent,&rectangle))
    return NULL;

  lvhwnd = CreateWindow( WC_LISTVIEW,
                         WindowName,
                         WS_BORDER|WS_CHILD|LVS_REPORT|LVS_EX_FULLROWSELECT /*|LVS_SINGLESEL*/,
                         ((rectangle.right-rectangle.left)*XFromStartOfClientAreaPercent)/100,
                         ((rectangle.bottom-rectangle.top)*YFromStartOfClientAreaPercent)/100,
                         ((rectangle.right-rectangle.left)*WidthPercent)/100,
                         ((rectangle.bottom-rectangle.top)*HeightPercent)/100,
                         parent,
                         NULL,
                         hInstance,
                         NULL );
  
  if(!lvhwnd)
    return NULL;

  ShowWindow(lvhwnd,SW_SHOW);
 
  if(!UpdateWindow(lvhwnd))
  {
    DestroyWindow(lvhwnd);
    lvhwnd = NULL;
  }

  for(i=0;i<nColumns;i++)
  {
    col.mask = LVCF_WIDTH|LVCF_TEXT;
    col.cx = ColumnsSizes[i];
    col.cchTextMax = strlen(ColumnsNames[i]);
    col.pszText = ColumnsNames[i];
    ListView_InsertColumn(lvhwnd,i,&col);
  }

  return lvhwnd;
}


unsigned int CONTROLS_ListViewNotifyCallback(UINT uMsg,WPARAM wParam,LPARAM lParam,LPNMHDR pnmhdr,PCOMMON_CONTROL_NOTIFICATION_INFO pinfo)
{
  int code=CONTROL_UNKNOWN_NOTIFICATION;

  if(uMsg != WM_NOTIFY)
  {
    return code;
  }
    
  switch(pnmhdr->code)
  {
    case NM_CLICK:
    {
      LPNMITEMACTIVATE pItemActivated = (LPNMITEMACTIVATE)pnmhdr;
      pinfo->iItem = pItemActivated->iItem;
      pinfo->iSubItem = pItemActivated->iSubItem;
      pinfo->uKeyFlags = pItemActivated->uKeyFlags;
      code = CONTROL_LISTVIEW_ITEM_CLICKED_NOTIFICATION;
    }
    break;

    case NM_DBLCLK:
    {
      LPNMITEMACTIVATE pItemActivated = (LPNMITEMACTIVATE)pnmhdr;
      pinfo->iItem = pItemActivated->iItem;
      pinfo->iSubItem = pItemActivated->iSubItem;
      pinfo->uKeyFlags = pItemActivated->uKeyFlags;
      code = CONTROL_LISTVIEW_ITEM_DOUBLECLICKED_NOTIFICATION;
    }
    break;
  }

  return code;
}


void CONTROLS_DeleteEditBoxCommandLine(HWND hcontrol)
{
  DestroyWindow(hcontrol);
}


HWND CONTROLS_AddEditBoxCommandLine    ( HWND           parent,
                              HINSTANCE      hInstance,
                              unsigned int   WidthPercent,
                              unsigned int   HeightPercent,
                              unsigned int   XFromStartOfClientAreaPercent,
                              unsigned int   YFromStartOfClientAreaPercent,
                              TCHAR *        WindowName)
{
  RECT rectangle;
  HWND edhwnd;

  if(!parent || !WidthPercent || !HeightPercent || !WindowName || !GetClientRect(parent,&rectangle))
    return NULL;

  edhwnd = CreateWindow( "EDIT",
                         WindowName,
                         WS_BORDER|WS_CHILD|ES_LOWERCASE|ES_MULTILINE|ES_AUTOVSCROLL,
                         ((rectangle.right-rectangle.left)*XFromStartOfClientAreaPercent)/100,
                         ((rectangle.bottom-rectangle.top)*YFromStartOfClientAreaPercent)/100,
                         ((rectangle.right-rectangle.left)*WidthPercent)/100,
                         ((rectangle.bottom-rectangle.top)*HeightPercent)/100,
                         parent,
                         NULL,
                         hInstance,
                         NULL );
  
  if(!edhwnd)
    return NULL;

  SendMessage(edhwnd,EM_SETLIMITTEXT,CONTROL_EDITBOX_COMMANDLINE_MAXLEN,0);

  ShowWindow(edhwnd,SW_SHOW);
 
  if(!UpdateWindow(edhwnd))
  {
    DestroyWindow(edhwnd);
    edhwnd = NULL;
  }

  return edhwnd;
}


unsigned int CONTROLS_EditBoxCommandLineNotifyCallback (UINT uMsg,
                                             WPARAM wParam,
                                             LPARAM lParam,
                                             LPNMHDR pnmhdr,
                                             PCOMMON_CONTROL_NOTIFICATION_INFO pinfo)
{

  if(uMsg == WM_COMMAND && HIWORD(wParam)==EN_CHANGE)
  {
    pinfo->len=GetWindowText((HWND)lParam,pinfo->text,CONTROL_EDITBOX_COMMANDLINE_MAXLEN);
    
    if(pinfo->text[pinfo->len-1]==0x0a || pinfo->text[pinfo->len-1]==0x0d)
    {
      pinfo->text[pinfo->len-1]=0;
      pinfo->len--;
      
      if(pinfo->text[pinfo->len-1]==0x0a || pinfo->text[pinfo->len-1]==0x0d)
      {
        pinfo->text[pinfo->len-1]=0;
        pinfo->len--;
      }

      SetWindowText((HWND)lParam,"");
      return CONTROL_EDITBOX_COMMANDLINE_NEWLINE;
    }
    
    return CONTROL_EDITBOX_COMMANDLINE_CHANGED;
  }

  return CONTROL_UNKNOWN_NOTIFICATION;
}






///////////////////////////////////////////////////////////////////////


void CONTROLS_SetStaticTextCommonInfoText(HWND hcontrol,char * text)
{
  SetWindowText(hcontrol,text);
}

void CONTROLS_DeleteStaticTextCommonInfo(HWND hcontrol)
{
  DestroyWindow(hcontrol);
}


HWND CONTROLS_AddStaticTextCommonInfo ( HWND           parent,
                                        HINSTANCE      hInstance,
                                        unsigned int   WidthPercent,
                                        unsigned int   HeightPercent,
                                        unsigned int   XFromStartOfClientAreaPercent,
                                        unsigned int   YFromStartOfClientAreaPercent,
                                        TCHAR *        WindowName)
{
  RECT rectangle;
  HWND edhwnd;

  if(!parent || !WidthPercent || !HeightPercent || !WindowName || !GetClientRect(parent,&rectangle))
    return NULL;

  edhwnd = CreateWindow( "EDIT",
                         WindowName,
                         WS_BORDER|WS_CHILD|ES_LOWERCASE|ES_MULTILINE|ES_AUTOVSCROLL|ES_READONLY,
                         ((rectangle.right-rectangle.left)*XFromStartOfClientAreaPercent)/100,
                         ((rectangle.bottom-rectangle.top)*YFromStartOfClientAreaPercent)/100,
                         ((rectangle.right-rectangle.left)*WidthPercent)/100,
                         ((rectangle.bottom-rectangle.top)*HeightPercent)/100,
                         parent,
                         NULL,
                         hInstance,
                         NULL );
  
  if(!edhwnd)
    return NULL;

  ShowWindow(edhwnd,SW_SHOW);
 
  if(!UpdateWindow(edhwnd))
  {
    DestroyWindow(edhwnd);
    edhwnd = NULL;
  }

  return edhwnd;
}

