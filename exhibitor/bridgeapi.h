#ifndef __BRIDGEAPI_H__
#define __BRIDGEAPI_H__

#include <tchar.h>
#include "errors.h"


typedef IFACE_STATUS (* BRIDGEAPI_INSERTLISTVIEWITEM)(TCHAR ** BridgeListViewControlColumnsNames,int maxlen);
typedef void (*BRIDGEAPI_DELETELISTVIEWITEM)(int item);
typedef void (*BRIDGEAPI_SETSTATICTEXT)(TCHAR * text);

typedef struct  
{
  BRIDGEAPI_INSERTLISTVIEWITEM InsertListViewItem;
  BRIDGEAPI_DELETELISTVIEWITEM DeleteListViewItem;
  BRIDGEAPI_SETSTATICTEXT      SetStaticText;

} BRIDGEAPI_FUNCS, *PBRIDGEAPI_FUNCS;


PBRIDGEAPI_FUNCS BRIDGEAPI_GetFuncs(); // used by the iface only. Bridge must not call it.

#endif