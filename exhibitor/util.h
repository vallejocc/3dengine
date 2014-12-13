#ifndef __UTIL_H__
#define __UTIL_H__

TCHAR * SelectFileMenu(TCHAR * pathname,char * InitDirParam=NULL);
TCHAR * SelectDirectoryMenu(HWND ownerwin,TCHAR * path);

#endif