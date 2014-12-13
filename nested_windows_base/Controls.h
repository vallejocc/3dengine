#ifndef __CONTROLS_H__
#define __CONTROLS_H__

#include <windows.h>
#include <tchar.h>


// Translated code given by notify callbacks for a notification code
#define CONTROL_UNKNOWN_NOTIFICATION                     0 
#define CONTROL_LISTVIEW_ITEM_DOUBLECLICKED_NOTIFICATION 1
#define CONTROL_LISTVIEW_ITEM_CLICKED_NOTIFICATION       2
#define CONTROL_EDITBOX_COMMANDLINE_CHANGED              3
#define CONTROL_EDITBOX_COMMANDLINE_NEWLINE              4


#define CONTROL_EDITBOX_COMMANDLINE_MAXLEN 500

typedef struct
{
  union
  {
    struct // listviews
    {
      int iItem;
      int iSubItem;
      int uKeyFlags; //LVKF_ALT/LVKF_CONTROL/LVKF_SHIFT
    };
  
    struct // edit box commandline
    {
      TCHAR text[CONTROL_EDITBOX_COMMANDLINE_MAXLEN];
      unsigned int len;      
    };
  };

} COMMON_CONTROL_NOTIFICATION_INFO,*PCOMMON_CONTROL_NOTIFICATION_INFO;


typedef unsigned int (*CONTROLSNOTIFYMSGCALLBACK)(UINT uMsg,WPARAM wParam,LPARAM lParam,LPNMHDR,PCOMMON_CONTROL_NOTIFICATION_INFO);



void CONTROLS_DeleteListViewControl( HWND hcontrol );

HWND CONTROLS_AddListViewControl( HWND         parent,
                                  HINSTANCE    hInstance,
                                  unsigned int WidthPercent,
                                  unsigned int HeightPercent,
                                  unsigned int XFromStartOfClientAreaPercent,
                                  unsigned int YFromStartOfClientAreaPercent,
                                  unsigned int nColumns,
                                  TCHAR**      ColumnsNames,
                                  unsigned int*ColumnsSizes,
                                  TCHAR *      WindowName );

int CONTROLS_AddElementToListViewControl(HWND hcontrol,
                                         unsigned int nColumns,
                                         TCHAR ** ColumnsText,
                                         unsigned int MaxLen);

unsigned int CONTROLS_ListViewNotifyCallback(UINT uMsg,WPARAM wParam,LPARAM lParam,LPNMHDR pnmhdr,PCOMMON_CONTROL_NOTIFICATION_INFO pinfo);

void CONTROLS_DeleteElementFromListViewControl(HWND hcontrol,int item);

HWND CONTROLS_AddEditBoxCommandLine    ( HWND           parent,
                              HINSTANCE      hInstance,
                              unsigned int   WidthPercent,
                              unsigned int   HeightPercent,
                              unsigned int   XFromStartOfClientAreaPercent,
                              unsigned int   YFromStartOfClientAreaPercent,
                              TCHAR *        WindowName);

void CONTROLS_DeleteEditBoxCommandLine (HWND hcontrol);

unsigned int CONTROLS_EditBoxCommandLineNotifyCallback (UINT uMsg,
                                             WPARAM wParam,
                                             LPARAM lParam,
                                             LPNMHDR pnmhdr,
                                             PCOMMON_CONTROL_NOTIFICATION_INFO pinfo);

void CONTROLS_DeleteStaticTextCommonInfo(HWND hcontrol);

HWND CONTROLS_AddStaticTextCommonInfo ( HWND           parent,
                                        HINSTANCE      hInstance,
                                        unsigned int   WidthPercent,
                                        unsigned int   HeightPercent,
                                        unsigned int   XFromStartOfClientAreaPercent,
                                        unsigned int   YFromStartOfClientAreaPercent,
                                        TCHAR *        WindowName);

void CONTROLS_SetStaticTextCommonInfoText(HWND hcontrol,char * text);

#endif