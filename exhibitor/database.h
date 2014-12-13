#ifndef __DATABASE_H__
#define __DATABASE_H__

#define DATABASE_MAGIC 0xDA7ABA5E

typedef struct  
{
  unsigned int InfoType; 


} DATABASE_ENTRY, *PDATABASE_ENTRY;

unsigned int GetPhoneNumber(TCHAR * phonenumberbuf,unsigned int buflen);
unsigned int CreateTsrDatabase(char * phonenumber);
unsigned int SendAT(HANDLE hCom,TCHAR * at,unsigned int len);

#endif