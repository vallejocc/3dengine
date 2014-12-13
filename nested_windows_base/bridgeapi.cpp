#include <windows.h>
#include "bridgeapi.h"
#include "controls.h"
#include "main.h"
#include "errors.h"
#include "Bridge.h"


IFACE_STATUS BRIDGEAPI_InsertListViewItem(TCHAR ** BridgeListViewControlColumnsNames,int maxlen)
{
  CONTROLS_AddElementToListViewControl(GlobalData.ListViewWindow,
                                       BRIDGE_GetListViewControlColumnsNumber(),
                                       BridgeListViewControlColumnsNames,
                                       maxlen);
  return IFACE_STATUS_SUCCESS;
}


void BRIDGEAPI_DeleteListViewItem(int item)
{
  CONTROLS_DeleteElementFromListViewControl(GlobalData.ListViewWindow,item);
}

void BRIDGEAPI_SetStaticText(TCHAR * text)
{
  CONTROLS_SetStaticTextCommonInfoText(GlobalData.StaticTextWindow,text);
}

/////////PUBLIC//////////////////////////////////

BRIDGEAPI_FUNCS g_bridgeapistaticpointerfuncs =
{
  BRIDGEAPI_InsertListViewItem,
  BRIDGEAPI_DeleteListViewItem,
  BRIDGEAPI_SetStaticText
};

PBRIDGEAPI_FUNCS BRIDGEAPI_GetFuncs()
{  
  return &g_bridgeapistaticpointerfuncs;
}