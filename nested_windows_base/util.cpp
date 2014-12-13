#include <windows.h>
#include <Commctrl.h>
#include <stdio.h>
#include <shlobj.h>
#include <tchar.h>

TCHAR * SelectDirectoryMenu(HWND ownerwin,TCHAR * path)
{
    
  BROWSEINFO bi;
  LPITEMIDLIST lpidl;
  TCHAR ShowName[_MAX_PATH];

  bi.hwndOwner = ownerwin;
  bi.pidlRoot = NULL;
  bi.pszDisplayName = ShowName;
  bi.lpszTitle = "Select A Directory...";
  bi.ulFlags = BIF_EDITBOX|BIF_RETURNONLYFSDIRS;
  bi.lpfn = NULL;
  bi.lParam = 0;
  bi.iImage = 0;
  
  if(lpidl = SHBrowseForFolder(&bi))
  {
      SHGetPathFromIDList(lpidl,path);
  }    
  
  return path;
}



TCHAR * SelectFileMenu(TCHAR * pathname)
{
  OPENFILENAME ofn;
  TCHAR InitDir[MAX_PATH];
  TCHAR BufferForReceiving[_MAX_PATH];
  
  GetModuleFileNameA(NULL,BufferForReceiving,MAX_PATH);
  _splitpath(BufferForReceiving,InitDir,&InitDir[2],NULL,NULL);
  
  pathname[0]=0;
  memset(&ofn,0,sizeof(OPENFILENAME));
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.lpstrFile = BufferForReceiving;
  ofn.nMaxFile = _MAX_PATH;
  ofn.lpstrInitialDir = InitDir;
  ofn.lpstrTitle = "Select a file...";
  ofn.Flags = OFN_EXPLORER|OFN_LONGNAMES|OFN_PATHMUSTEXIST;

  GetOpenFileName(&ofn);

  _tcsncpy(pathname,BufferForReceiving,_MAX_PATH);
  
  return pathname;
}