#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include "database.h"
#include "resource.h"

TCHAR * g_phonenumberbuf; 
unsigned int g_buflen;


INT_PTR CALLBACK GetDatabaseNumberDialogProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
  unsigned int retVal=-1;
  BOOL issuccess=FALSE;

  switch(uMsg) {
  
  case WM_COMMAND:

    switch(LOWORD(wParam))
    {
      case IDC_BUTTON_GETDATABASENUMBER_ACCEPT:
        retVal=GetDlgItemText(hwndDlg,IDC_EDIT_GETDATABASENUMBER_EDITBOX,g_phonenumberbuf,g_buflen);
        if(!issuccess)
          retVal=-1;
        EndDialog(hwndDlg,retVal);
  		  return TRUE;
  
      default:
        return DefWindowProc(hwndDlg,uMsg,wParam,lParam);  
    }    
    break;
  
	case WM_CLOSE:
    EndDialog(hwndDlg,retVal);
		return TRUE;
  
  default:
    return DefWindowProc(hwndDlg,uMsg,wParam,lParam);    
  }

  return TRUE;
}


unsigned int GetPhoneNumber(TCHAR * phonenumberbuf,unsigned int buflen)
{
  g_phonenumberbuf = phonenumberbuf;
  g_buflen = buflen;

  return DialogBox( GetModuleHandle(NULL),
                    MAKEINTRESOURCE(IDD_FORMVIEW_GETPHONE_NUMBER),
                    NULL,
                    (DLGPROC)GetDatabaseNumberDialogProc );
  
}


unsigned int CreateTsrDatabase(char * phonenumber)
{
  if(phonenumber && phonenumber[0])
  {
    TCHAR tempbuf[MAX_PATH];
    FILE * f;
    
    _tcsncpy(tempbuf,phonenumber,MAX_PATH);
    _tcsncat(tempbuf,".dat",MAX_PATH);
    
    f = fopen(tempbuf,"w+b");
    
    if(f)
    {
      unsigned int tempuint=DATABASE_MAGIC;
      fwrite(&tempuint,sizeof(unsigned int),1,f);
      tempuint=0;
      fwrite(&tempuint,sizeof(unsigned int),1,f);
      fclose(f);  

      return 1;
    }
  }

  return 0;
}